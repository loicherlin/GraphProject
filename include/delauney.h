#include "../include/serializer.h"
#include "../include/array_list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct triangle
{
    data_t s1;
    data_t s2;
    data_t s3;
}triangle;

triangle create_triangle(data_t a, data_t b, data_t c);
triangle create_super_triangle(list_t* nodes);
void triangulate(list_t* nodes);