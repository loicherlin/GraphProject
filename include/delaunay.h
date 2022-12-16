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
} triangle_t;

typedef struct edge{
    data_t* org;
    data_t* dest;
} edge_t;

/**
 * Give a triangulation following the Bowyer-Watson algorithm.
 * @param nodes The list of nodes to triangulate.
 * @return The list of triangles.
 * @note first index of the array contains a triangle_t where his first node contains
 * the size of the array (in lattitude) and total number of unique nodes used (in longitude).
 * @note This functions works in O(n²) and is not efficient and could be improved.
 * https://en.wikipedia.org/wiki/Delaunay_triangulation
 */
triangle_t** delaunay_bowyer_watson(list_t* nodes);

/**
 * @brief Save the delaunay triangles in a binary file
 * @param delaunay
 * @param fp
 * @param data_list
 * @note Save id of the points of each triangle
 */
void save_delaunay(triangle_t** delaunay, FILE* fp, list_t* data_list);

/**
 * @brief Get the delaunay triangles from a binary file
 * @param fp
 * @param data_list
 * @return triangle_t**
 * @note fp has to be closed after this function
 */
triangle_t** get_delaunay(FILE* fp, list_t* data_list);

/**
 * @brief Initiate the delaunay triangles, based on the data_list
 * @note The delaunay triangles are saved in a binary file, if it already exists, it will be loaded.
 * @param data_list
 * @param path_to_save
 * @param path_to_load
 * @return triangle_t**
 */
triangle_t** initiate_delaunay(list_t* data_list, char* path_to_save, char* path_to_load);

/**
 * @brief Free the memory allocated for the delaunay triangles
 * @param triangles
 * @param size
 */
void free_list_t(triangle_t** triangles, size_t size);