#pragma once
#include "data_t.h"
#include "min_heap.h"
#include "delaunay.h"
#include <stdbool.h>

enum DISTANCE_TYPE{
    EUCLIDEAN = 0,
    HAVESINE = 1 // http://villemin.gerard.free.fr/aGeograp/Distance.htm
};

extern enum DISTANCE_TYPE _distance_type;

/**
 * Node of an adjacent list
 */
typedef struct node_adj {
    int id;
    data_t* data;
    double weight;
    struct node_adj *next;
}node_adj_t;

/**
 * Adjacent list
 */
typedef struct adj_list {
    struct node_adj *head;
}adj_list_t;

/**
 * Graph structure
 */
typedef struct graph{
    int size_vertices;
    struct adj_list* arr;
    int size_edges;
}graph_t;

/**
 * Set the distance norm for distance calculation
 * @param type (EUCLIDEAN or HAVESINE)
 */
void set_distn(enum DISTANCE_TYPE type);

/**
 * Calculate the distance between two points
 * @param p1 point 1
 * @param p2 point 2
 * @param type distance type (EUCLIDEAN or HAVESINE)
 */
double dist(data_t p1, data_t p2, enum DISTANCE_TYPE type);

/**
 * free memory of a graph
 * @param graph
 */
void free_graph(graph_t* graph);

/**
 * Create a graph with size_vertices vertices
 * @param size_vertices
 * @return graph
 */
graph_t* create_graph(int size_vertices);

/**
 * Create a node of an adjacent list
 * @param id
 * @param data
 * @param weight
 * @return node
 */
node_adj_t* create_node_adj(int id, data_t* data, double weight);

/**
 * Add an edge to an undirected graph
 * @param graph
 * @param data1
 * @param data2
 */
void add_edge(graph_t* graph, data_t* data1, data_t* data2);

/**
 * Show nodes of a graph and their adjacent nodes
 * @param g
 */
void show_graph_adj(graph_t* g);

/**
 * Save a MST to a file
 * @param parent
 * @param size_vertices
 * @param path
 * @note if path is incorrect, the function exit with EXIT_FAILURE
 */
void save_mst(int* parent, int size_vertices, char* path);

/**
 * Create a MST using Prim's algorithm and a min heap, and save it to a file if 
 * filename is not empty
 * @param graph
 * @param filename
 * @return parent array
 */
int* prim_mst(graph_t* graph, char* filename);


/**
 * Check vertices d1 and d2 are connected by an edge in graph
 * @param graph
 * @param d1 vertex 1
 * @param d2 vertex 2
 * @return true if d1 and d2 are connected by an edge in graph
 */
bool is_edge_in_graph(graph_t* graph, data_t* d1, data_t* d2);

/**
 * Convert delaunay triangulation to graph
 * @param triangles
 * @param graph
 */
void delaunay_to_graph(delaunay_t* triangles, graph_t* graph);

/**
 * Calculate the sum of the weights of the edges in the MST
 * @param mst array of parents
 * @param nodes list of nodes
 * @param size_vertices
 * @return sum of the weights of the edges in the MST
 */
double sum_weight_graph(int* mst, list_t* nodes, int size_vertices);
