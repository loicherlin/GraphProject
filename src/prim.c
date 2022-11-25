#include "../include/prim.h"

float dist(data_t p1, data_t p2)
{
    return sqrt((p1.latitude - p2.latitude)*(p1.latitude - p2.latitude) +
                (p1.longitude - p2.longitude)*(p1.longitude - p2.longitude));
}

void free_graph(graph_t* graph)
{
    for (int i = 0; i < graph->size_vertices; i++)
    {
        node_adj_t* node = graph->arr[i].head;
        while(node != NULL)
        {
            node_adj_t* temp = node;
            node = node->next;
            free(temp);
        }
    }
    free(graph->arr);
    free(graph);
}

graph_t* create_graph(int size_vertices)
{
    graph_t* graph = (graph_t*)malloc(sizeof(graph_t));
    graph->size_vertices = size_vertices;
    graph->arr = (adj_list_t*)malloc(size_vertices * sizeof(adj_list_t));
    for (int i = 0; i < size_vertices; i++)
        graph->arr[i].head = NULL;
    return graph;
}

node_adj_t* create_node_adj(int id, data_t* data, float weight)
{
    node_adj_t* node = (node_adj_t*)malloc(sizeof(node_adj_t));
    node->id = id;
    node->data = data;
    node->weight = weight;
    node->next = NULL;
    return node;
}
// add edge between data1 and data2 on undirected graph
void add_edge(graph_t* graph, data_t* data1, data_t* data2){
    int id1 = data1->id;
    int id2 = data2->id;
    float weight = dist(*data1, *data2);
    // add edge from data1 to data2
    node_adj_t* node1 = create_node_adj(id2, data2, weight);
    node1->next = graph->arr[id1].head;
    graph->arr[id1].head = node1;
    // add edge from data2 to data1
    node_adj_t* node2 = create_node_adj(id1, data1, weight);
    node2->next = graph->arr[id2].head;
    graph->arr[id2].head = node2;
}

void show_graph_ajd(graph_t* g){
    for (int i = 0; i < g->size_vertices; i++)
    {
        node_adj_t* node = g->arr[i].head;
        printf("Vertex %d: ", i);
        while(node != NULL)
        {
            printf("%d(%f) ", node->id, node->weight);
            node = node->next;
        }
        printf("\n");
    }
}