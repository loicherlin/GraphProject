#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
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

#include "tps_unit_test.h"

void test_graph_dist(void){
    data_t p1, p2;
    p1.latitude = 0.0;
    p1.longitude = 0.0;
    p2.latitude = 1.0;
    p2.longitude = 1.0;
    double euclidian_dist = dist(p1, p2, EUCLIDEAN);
    double haversine_dist = dist(p1, p2, HAVESINE);
    tps_assert(euclidian_dist == 1.4142135623730951);
    tps_assert((int)haversine_dist == 157);
}

void test_create_graph(void){
    graph_t* graph = create_graph(10);
    tps_assert(graph->size_vertices == 10);
    for (int i = 0; i < graph->size_vertices; i++){
        tps_assert(graph->arr[i].head == NULL);
    }
    free_graph(graph);
}

void test_create_node_adj(void){
    data_t data;
    data.id = 1;
    data.latitude = 1.0;
    data.longitude = 1.0;
    node_adj_t* node = create_node_adj(2, &data, 1.5);
    tps_assert(node->id == 2);
    tps_assert(node->data == &data);
    tps_assert(node->weight == 1.5);
    tps_assert(node->next == NULL);
    free(node);
}

void test_add_edge(void){
    data_t data1, data2;
    data1.id = 1;
    data1.latitude = 0.0;
    data1.longitude = 0.0;
    data2.id = 2;
    data2.latitude = 1.0;
    data2.longitude = 1.0;
    
    set_distn(HAVESINE);
    graph_t* graph = create_graph(3);

    add_edge(graph, &data1, &data2);

    node_adj_t* node = graph->arr[1].head;
    tps_assert(node != NULL);
    tps_assert(node->id == 2);
    tps_assert(node->data == &data2);
    tps_assert((int)(node->weight) == 157);
    tps_assert(node->next == NULL);
    node = graph->arr[2].head;
    tps_assert(node != NULL);
    tps_assert(node->id == 1);
    tps_assert(node->data == &data1);
    tps_assert((int)(node->weight) == 157);
    tps_assert(node->next == NULL);

    free_graph(graph);
}

void test_prim_mst(void){
    data_t data1, data2, data3, data4, data5;
    data1.id = 0;
    data1.latitude = 0.0;
    data1.longitude = 0.0;
    data2.id = 1;
    data2.latitude = 1.0;
    data2.longitude = 1.0;
    data3.id = 2;
    data3.latitude = 2.0;
    data3.longitude = 2.0;
    data4.id = 3;
    data4.latitude = 3.0;
    data4.longitude = 3.0;
    data5.id = 4;
    data5.latitude = 4.0;
    data5.longitude = 4.0;

    set_distn(EUCLIDEAN);
    graph_t* graph = create_graph(5);

    add_edge(graph, &data1, &data2);
    add_edge(graph, &data1, &data3);
    add_edge(graph, &data1, &data4);
    add_edge(graph, &data1, &data5);
    add_edge(graph, &data2, &data3);
    add_edge(graph, &data2, &data4);
    add_edge(graph, &data2, &data5);
    add_edge(graph, &data3, &data4);
    add_edge(graph, &data3, &data5);
    add_edge(graph, &data4, &data5);
    show_graph_adj(graph);

    int* mst = prim_mst(graph, "");
    
    for(int i = 1; i < graph->size_vertices; i++){
        printf("%d - %d\n", i, mst[i]);
    }

    free_graph(graph);
    free(mst);
}

void unit_test_graph(void){
    TEST(test_graph_dist);
    TEST(test_create_graph);
    TEST(test_create_node_adj);
    TEST(test_add_edge);
    TEST(test_prim_mst);
}