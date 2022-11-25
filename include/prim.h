#pragma once
#include "serializer.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct node_adj {
    int id;
    data_t* data;
    float weight;
    struct node_adj *next;
}node_adj_t;

typedef struct adj_list {
    struct node_adj *head;
}adj_list_t;

typedef struct graph{
    int size_vertices;
    struct adj_list* arr;
}graph_t;

float dist(data_t p1, data_t p2);

void free_graph(graph_t* graph);

graph_t* create_graph(int size_vertices);

node_adj_t* create_node_adj(int id, data_t* data, float weight);

void add_edge(graph_t* graph, data_t* data1, data_t* data2);

void show_graph_ajd(graph_t* g);

