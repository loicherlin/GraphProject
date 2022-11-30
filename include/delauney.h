#include "../include/serializer.h"
#include "../include/array_list.h"
#include <stdio.h>
#include <stdlib.h>
#pragma once

typedef struct triangle
{
    node_t* s1;
    node_t* s2;
    node_t* s3;
} triangle;

typedef struct edge{
    node_t* org;
    node_t* dest;
} edge_t;


triangle** delaunay_bowyer_watson(list_t* nodes);