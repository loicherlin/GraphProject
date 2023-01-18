#include "../include/array_list.h"
#include "../include/bin_builder.h"
#include "../include/cprintf.h"
#include "../include/csv_parser.h"
#include "../include/data_t.h"
#include "../include/delaunay.h"
#include "../include/graph.h"
#include "../include/handler.h"
#include "../include/min_heap.h"
#include "../include/triangle.h"
#include "../include/visualise.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "tps_unit_test.h"

void test_min_heap_create_min_heap_node(void) {
    min_heap_node_t *node = create_min_heap_node(5, 3.14);
    assert(node != NULL);
    assert(node->v == 5);
    assert(node->key == 3.14);
    free(node);

    node = create_min_heap_node(7, 2.71);
    assert(node != NULL);
    assert(node->v == 7);
    assert(node->key == 2.71);
    free(node);
}

void test_min_heap_create_min_heap() {
    min_heap_t *heap = create_min_heap(10);
    assert(heap != NULL);
    assert(heap->capacity == 10);
    assert(heap->size == 0);
    assert(heap->array != NULL);
    assert(heap->pos != NULL);
    free(heap->array);
    free(heap->pos);
    free(heap);

    heap = create_min_heap(15);
    assert(heap != NULL);
    assert(heap->capacity == 15);
    assert(heap->size == 0);
    assert(heap->array != NULL);
    assert(heap->pos != NULL);
    free_min_heap(heap);
}

void test_min_heap_swap_min_heap_node(void) {
    min_heap_node_t *node1 = create_min_heap_node(5, 3.14);
    min_heap_node_t *node2 = create_min_heap_node(7, 2.71);

    assert(node1->v == 5);
    assert(node1->key == 3.14);
    assert(node2->v == 7);
    assert(node2->key == 2.71);

    swap_min_heap_node(&node1, &node2);

    assert(node1->v == 7);
    assert(node1->key == 2.71);
    assert(node2->v == 5);
    assert(node2->key == 3.14);

    free(node1);
    free(node2);
}

void test_min_heap_min_heapify(void) {
    min_heap_t *heap = create_min_heap(10);
    heap->array[0] = create_min_heap_node(3, 3.14);
    heap->array[1] = create_min_heap_node(1, 2.71);
    heap->array[2] = create_min_heap_node(2, 1.41);
    heap->array[3] = create_min_heap_node(4, 4.67);
    heap->array[4] = create_min_heap_node(0, 5.12);
    heap->size = 5;

    min_heapify(heap, 0);

    assert(heap->array[0]->key <= heap->array[1]->key);
    assert(heap->array[1]->key <= heap->array[2]->key);
    assert(heap->array[2]->key <= heap->array[3]->key);
    assert(heap->array[3]->key <= heap->array[4]->key);

    for (int i = 0; i < heap->size; i++)
        free(heap->array[i]);
    free_min_heap(heap);
}

void test_min_heap_is_empty(void) {
    min_heap_t *heap = create_min_heap(10);
    assert(is_empty(heap) == 1);

    min_heap_node_t *node = create_min_heap_node(5, 3.14);
    heap->array[0] = node;
    heap->size = 1;
    assert(is_empty(heap) == 0);

    free(node);
    free_min_heap(heap);
}

void test_min_heap_extract_min() {
    min_heap_t *heap = create_min_heap(10);

    min_heap_node_t *node1 = create_min_heap_node(3, 3.14);
    min_heap_node_t *node2 = create_min_heap_node(1, 2.71);
    min_heap_node_t *node3 = create_min_heap_node(2, 1.41);
    min_heap_node_t *node4 = create_min_heap_node(4, 4.67);
    min_heap_node_t *node5 = create_min_heap_node(0, 5.12);
    heap->array[0] = node1;
    heap->array[1] = node2;
    heap->array[2] = node3;
    heap->array[3] = node4;
    heap->array[4] = node5;
    heap->size = 5;

    min_heapify(heap, 0);
    min_heap_node_t *min_node = extract_min(heap);
    assert(min_node->v == 2);
    assert(min_node->key == 1.41);
    free(min_node);

    assert(heap->size == 4);

    min_node = extract_min(heap);
    assert(min_node->v == 1);
    assert(min_node->key == 2.71);
    free(min_node);

    for (int i = 0; i < heap->size; i++)
        free(heap->array[i]);
    free_min_heap(heap);
}

void test_min_heap_decrease_key() {
    min_heap_t *heap = create_min_heap(10);

    min_heap_node_t *node1 = create_min_heap_node(3, 3.14);
    min_heap_node_t *node2 = create_min_heap_node(1, 2.71);
    min_heap_node_t *node3 = create_min_heap_node(2, 1.41);
    min_heap_node_t *node4 = create_min_heap_node(4, 4.67);
    min_heap_node_t *node5 = create_min_heap_node(0, 5.12);
    heap->array[0] = node1;
    heap->array[1] = node2;
    heap->array[2] = node3;
    heap->array[3] = node4;
    heap->array[4] = node5;
    heap->size = 5;

    min_heapify(heap, 0);
    decrease_key(heap, 3, 1.42);
    assert(heap->array[0]->key == 1.41);
    assert(heap->array[1]->key == 2.71);
    assert(heap->array[2]->key == 1.42);
    assert(heap->array[3]->key == 4.67);
    assert(heap->array[4]->key == 5.12);

    for (int i = 0; i < heap->size; i++)
        free(heap->array[i]);
    free_min_heap(heap);
}

void test_min_heap_is_in_min_heap() {
    min_heap_t *heap = create_min_heap(10);

    min_heap_node_t *node1 = create_min_heap_node(3, 3.14);
    min_heap_node_t *node2 = create_min_heap_node(1, 2.71);
    min_heap_node_t *node3 = create_min_heap_node(2, 1.41);
    min_heap_node_t *node4 = create_min_heap_node(4, 4.67);
    min_heap_node_t *node5 = create_min_heap_node(0, 5.12);
    heap->array[0] = node1;
    heap->array[1] = node2;
    heap->array[2] = node3;
    heap->array[3] = node4;
    heap->array[4] = node5;
    heap->size = 5;

    assert(is_in_min_heap(heap, 3) == 1 && heap->array[0]->key == 3.14);
    assert(is_in_min_heap(heap, 1) == 1 && heap->array[1]->key == 2.71);
    assert(is_in_min_heap(heap, 2) == 1 && heap->array[2]->key == 1.41);
    assert(is_in_min_heap(heap, 4) == 1 && heap->array[3]->key == 4.67);
    assert(is_in_min_heap(heap, 5) == 1 && heap->array[4]->key == 5.12);

    for (int i = 0; i < heap->size; i++)
        free(heap->array[i]);
    free_min_heap(heap);
}

void unit_test_min_heap(void) {
    TEST(test_min_heap_create_min_heap_node);
    TEST(test_min_heap_create_min_heap);
    TEST(test_min_heap_swap_min_heap_node);
    TEST(test_min_heap_min_heapify);
    TEST(test_min_heap_is_empty);
    TEST(test_min_heap_extract_min);
    TEST(test_min_heap_decrease_key);
    // TEST(test_min_heap_is_in_min_heap);
}
