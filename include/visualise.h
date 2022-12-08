#pragma once
#include "./delaunay.h"
#include "../include/array_list.h"
#include "../include/serializer.h"
#include "../include/prim.h"


typedef struct screen_t{
    int width;
    int height;
    double x_max; 
    double x_min; 
    double y_max; 
    double y_min;
} screen_t;

enum TXT{
    TXT_DEFAULT = 0,
    TXT_DELAUNAY = 1,
    TXT_PRIM =  2 
};

/*
    Return à normalize value based on
    a coordinate and his max and min value.
*/
double normalize_to_screen(double coord, double coord_max, double coord_min);

/* 
    Based on a node_list, get x=lattitude y=longitude min max value.
    return 0 if no problems, otherwise 1.
*/
void get_xy_min_max(list_t* node_list, double* x_max, double* x_min, double* y_max, double* y_min);


void visualize(int width, int height, list_t* node_list, int* mst, triangle_t** delaunay, graph_t* g);

/* 
    Show a visualisation of nodes in a SDL window 
*/
void show_mst(screen_t s, list_t* node_list, int* mst);

/*
    Show Delaunay triangulation in a SDL window
*/
void show_delaunay(screen_t s, list_t* node_list, triangle_t** triangles);

void onKeyDown(int keyPressed);