#include "../include/serializer.h"
#include "../include/array_list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct triangle
{
    node_t s1;
    node_t s2;
    node_t s3;
}triangle;

triangle create_triangle(node_t a, node_t b, node_t c);
triangle create_super_triangle(list_t* nodes);
list_t* delaunay_bowyer_watson(list_t* nodes);