#include "../include/serializer.h"
#include "../include/array_list.h"
#include <stdio.h>
#include <stdlib.h>
#pragma once

typedef struct triangle
{
    data_t* s1;
    data_t* s2;
    data_t* s3;
} triangle;

typedef struct edge{
    data_t* org;
    data_t* dest;
} edge_t;


triangle** delaunay_bowyer_watson(list_t* nodes);