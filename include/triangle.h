#pragma once
#include "data_t.h"

/**
 * @defgroup TriangleModule
 * @{
 * @brief Triangle structure and functions to handle it
 */

typedef struct triangle
{
    data_t *s1;
    data_t *s2;
    data_t *s3;
} triangle_t;

/**
 * Structure for edge
 */
typedef struct edge
{
    data_t *org;
    data_t *dest;
} edge_t;

/**
 * Create a triangle object with 3 points (data_t*)
 * @param a
 * @param b
 * @param c
 * @return triangle_t
 */
triangle_t create_triangle(data_t *a, data_t *b, data_t *c);

/**
 * Check if current edge is shared with 3 other edges
 * @param bad_t_a
 * @param bad_t_b
 * @param bad_t_c
 * @param current
 * @param EPSILON
 * @return int return 1 if they share a same edge else 0
 */
int edge_shared(edge_t bad_t_a, edge_t bad_t_b, edge_t bad_t_c, edge_t current, double ESPILON);

/**
 * Compare two triangles if they share a same point
 * @param a
 * @param b
 * @param EPSILON
 * @return int return 1 if they share a same point else 0
 */
int compare_triangle_point(triangle_t a, triangle_t b, double EPSILON);

/**
 * Compare two triangles if they are the same triangle
 * @param a triangle
 * @param b triangle
 * @param EPSILON double
 * @return int return 1 if they are the same triangle else 0
 */
int compare_triangle(triangle_t a, triangle_t b, double EPSILON);
/** @} */
