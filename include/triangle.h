#pragma once
#include "data_t.h"

typedef struct triangle
{
    data_t* s1;
    data_t* s2;
    data_t* s3;
} triangle_t;

typedef struct edge{
    data_t* org;
    data_t* dest;
} edge_t;

triangle_t create_triangle(data_t* a, data_t* b, data_t* c);

int edge_shared(edge_t bad_t_a, edge_t bad_t_b, edge_t bad_t_c, edge_t current, double ESPILON);

int compare_triangle_point(triangle_t a, triangle_t b, double EPSILON);

int compare_triangle(triangle_t a, triangle_t b, double EPSILON);