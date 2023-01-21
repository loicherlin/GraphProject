#pragma once
#include "array_list.h"
#include "data_t.h"
#include "triangle.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @defgroup DelaunayModule
 * @{
 * @brief To get the delaunay triangulation of a set of points, load and save
 * it.
 * @note Bowyer-Watson algorithm is only implemented.
 */

typedef struct
{
    triangle_t **triangles;
    size_t size_triangles;
    size_t size_vertices;
} delaunay_t;

/**
 * Give a triangulation following the Bowyer-Watson algorithm.
 * @param nodes The list of nodes to triangulate.
 * @return delaunay triangulation.
 * @note Running time of this function is O(n²) and could be improved.
 * https://en.wikipedia.org/wiki/Delaunay_triangulation
 */
delaunay_t *delaunay_bowyer_watson(list_t *nodes);

/**
 * @brief Save the delaunay triangles in a binary file
 * @param delaunay
 * @param fp
 * @note Save id of the points of each triangle
 */
void serialize_delaunay(delaunay_t *delaunay, FILE *fp);

/**
 * @brief Get the delaunay triangles from a binary file
 * @param fp
 * @param data_list
 * @return delaunay_t*
 * @note fp has to be closed after this function
 */
delaunay_t *deserialize_delaunay(FILE *fp, list_t *data_list);

/**
 * @brief Initiate the delaunay triangles, based on the data_list
 * @note The delaunay triangles are saved in a binary file, if it already
 * exists, it will be loaded.
 * @param data_list
 * @param path_to_save
 * @param path_to_load
 * @return delaunay_t*
 */
delaunay_t *initiate_delaunay(list_t *data_list, char *path_to_save, char *path_to_load);

/**
 * @brief Free the memory allocated for the delaunay triangles
 * @param delaunay
 */
void free_delaunay(delaunay_t *delaunay);
/** @} */