#pragma once
#include "serializer.h"
#include <stdlib.h>
#include <stdio.h>

// helped from 
// https://sites.google.com/site/rajboston1951/minimum-spanning-tree/2

typedef struct min_heap_node {
    int v;
    float key;
}min_heap_node_t;

typedef struct min_heap {
    int size;
    int capacity;
    int* pos;
    struct min_heap_node** array;
}min_heap_t;

min_heap_node_t* create_min_heap_node(int v, float key);

min_heap_t* create_min_heap(int capacity);

void swap_min_heap_node(min_heap_node_t** a, min_heap_node_t** b);

void min_heapify(min_heap_t* min_heap, int index);

int is_empty(min_heap_t* min_heap);

min_heap_node_t* extract_min(min_heap_t* min_heap);

void decrease_key(min_heap_t* min_heap, int v, float key);

int is_in_min_heap(min_heap_t* min_heap, int v);

void free_min_heap(min_heap_t* min_heap);