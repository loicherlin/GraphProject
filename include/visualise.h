#pragma once
#include "./delaunay.h"


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


/* 
    Show a visualisation of nodes in a SDL window 
*/
void show_mst(int width, int height, list_t* node_list, int* mst);

/*
    Show Delaunay triangulation in a SDL window
*/
void show_delaunay(int width, int height, list_t* node_list, triangle** triangles);