#include "../include/min_heap.h"

min_heap_node_t *create_min_heap_node(int v, double key)
{
    min_heap_node_t *min_heap_node;
    CHK_ALLOC(min_heap_node = malloc(sizeof(min_heap_node_t)), "malloc failed");
    min_heap_node->v = v;
    min_heap_node->key = key;
    return min_heap_node;
}

min_heap_t *create_min_heap(int capacity)
{
    min_heap_t *min_heap;
    CHK_ALLOC(min_heap = malloc(sizeof(min_heap_t)), "malloc failed");
    CHK_ALLOC(min_heap->pos = malloc(capacity * sizeof(int)), "malloc failed");
    min_heap->size = 0;
    min_heap->capacity = capacity;
    CHK_ALLOC(min_heap->array = malloc(capacity * sizeof(min_heap_node_t *)), "malloc failed");
    return min_heap;
}

void swap_min_heap_node(min_heap_node_t **a, min_heap_node_t **b)
{
    min_heap_node_t *t = *a;
    *a = *b;
    *b = t;
}

void min_heapify(min_heap_t *min_heap, int index)
{
    int smallest, left, right;
    smallest = index;
    left = 2 * index + 1;
    right = 2 * index + 2;

    if (left < min_heap->size && min_heap->array[left]->key < min_heap->array[smallest]->key)
        smallest = left;

    if (right < min_heap->size && min_heap->array[right]->key < min_heap->array[smallest]->key)
        smallest = right;

    if (smallest != index)
    {
        min_heap_node_t *smallest_node = min_heap->array[smallest];
        min_heap_node_t *index_node = min_heap->array[index];

        min_heap->pos[smallest_node->v] = index;
        min_heap->pos[index_node->v] = smallest;

        swap_min_heap_node(&min_heap->array[smallest], &min_heap->array[index]);

        min_heapify(min_heap, smallest);
    }
}

int is_empty(min_heap_t *min_heap) { return min_heap->size == 0; }

min_heap_node_t *extract_min(min_heap_t *min_heap)
{
    if (is_empty(min_heap))
        return NULL;

    min_heap_node_t *root = min_heap->array[0];

    min_heap_node_t *last_node = min_heap->array[min_heap->size - 1];
    min_heap->array[0] = last_node;

    min_heap->pos[root->v] = min_heap->size - 1;
    min_heap->pos[last_node->v] = 0;

    --min_heap->size;
    min_heapify(min_heap, 0);

    return root;
}

void decrease_key(min_heap_t *min_heap, int v, double key)
{
    int i = min_heap->pos[v];
    min_heap->array[i]->key = key;
    while (i && min_heap->array[i]->key < min_heap->array[(i - 1) / 2]->key)
    {
        min_heap->pos[min_heap->array[i]->v] = (i - 1) / 2;
        min_heap->pos[min_heap->array[(i - 1) / 2]->v] = i;
        swap_min_heap_node(&min_heap->array[i], &min_heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int is_in_min_heap(min_heap_t *min_heap, int v)
{
    if (min_heap->pos[v] < min_heap->size)
        return 1;
    return 0;
}

void free_min_heap(min_heap_t *min_heap)
{
    free(min_heap->pos);
    free(min_heap->array);
    free(min_heap);
}