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

/***
 * @brief Create a triangle object with 3 points
 * 
 * @param a 
 * @param b 
 * @param c 
 * @return triangle_t 
*/
triangle_t create_triangle(data_t* a, data_t* b, data_t* c);

/***
 * @brief Compare a edge with a triangle if they share a same edge
 * 
 * @param bad_t_a 
 * @param bad_t_b
 * @param bad_t_c
 * @param current
 * @param EPSILON
 * @return int return 1 if they share a same edge else 0
*/
int edge_shared(edge_t bad_t_a, edge_t bad_t_b, edge_t bad_t_c, edge_t current, double ESPILON);

/***
 * @brief Compare two triangles if they share a same point
 * 
 * @param a
 * @param b
 * @param EPSILON
 * @return int return 1 if they share a same point else 0
*/

int compare_triangle_point(triangle_t a, triangle_t b, double EPSILON);

/***
 * @brief Compare two triangles if they are the same triangle return 1 else 0
 * 
 * @param a triangle
 * @param b triangle
 * @param EPSILON double
 * @return int return 1 if they are the same triangle else 0
*/
int compare_triangle(triangle_t a, triangle_t b, double EPSILON);