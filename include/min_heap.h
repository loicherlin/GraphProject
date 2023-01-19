#pragma once
#include "cprintf.h"
#include "data_t.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @defgroup MinHeapModule
 * @{
 * @brief Binary min heap structure and functions to handle it
 * @note helped from
 * https://sites.google.com/site/rajboston1951/minimum-spanning-tree/2
 */

/**
 * Node of an adjacent list
 */
typedef struct min_heap_node
{
    int v;
    double key;
} min_heap_node_t;

/**
 * Min heap structure
 */
typedef struct min_heap
{
    int size;
    int capacity;
    int *pos;
    struct min_heap_node **array;
} min_heap_t;

/**
 * Create a min heap node
 * @param v
 * @param key
 * @return min heap node
 */
min_heap_node_t *create_min_heap_node(int v, double key);

/**
 * Create a min heap
 * @param capacity
 * @return min heap
 */
min_heap_t *create_min_heap(int capacity);

/**
 * Swap two min heap nodes
 * @param a
 * @param b
 */
void swap_min_heap_node(min_heap_node_t **a, min_heap_node_t **b);

/**
 * Restructure min heap at index to make it hold min heap property
 * @param min_heap
 * @param index
 */
void min_heapify(min_heap_t *min_heap, int index);

/**
 * Check if min heap is empty
 * @param min_heap
 * @return 1 if min heap is empty, 0 otherwise
 */
int is_empty(min_heap_t *min_heap);

/**
 * Extract the min node from min heap
 * @param min_heap
 * @return min node
 */
min_heap_node_t *extract_min(min_heap_t *min_heap);

void decrease_key(min_heap_t *min_heap, int v, double key);

/**
 * Check if a vertex is in min heap
 * @param min_heap
 * @param v
 * @return 1 if v is in min heap, 0 otherwise
 */
int is_in_min_heap(min_heap_t *min_heap, int v);

/**
 * free memory of a min heap
 * @param min_heap
 */
void free_min_heap(min_heap_t *min_heap);
/** @} */