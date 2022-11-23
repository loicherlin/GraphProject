#pragma once
#include "./array_list.h"


/*
    Return à normalize value based on
    a coordinate and his max and min value.
*/
double normalize_to_screen(float coord, float coord_max, float coord_min);

/* 
    Based on a node_list, get x=lattitude y=longitude min max value.
    return 0 if no problems, otherwise 1.
*/
void get_xy_min_max(list_t* node_list, float* x_max, float* x_min, float* y_max, float* y_min);


/* 
    Show a visualisation of nodes in a SDL window 
*/
void show_data(int width, int height, list_t* node_list);