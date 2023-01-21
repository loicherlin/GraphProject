#include "../include/delaunay.h"
#include "../include/cprintf.h"
#include "../include/data_t.h"
#include "../include/handler.h"
#include "../include/triangle.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define EPSILON 0.00000000000000000000001

triangle_t create_super_triangle(list_t *nodes)
{
    int min_x = 0;
    int max_x = 0;
    int min_y = 0;
    int max_y = 0;
    size_t i;
    // Find the max and min of the nodes
    for (i = 0; i < list_size(nodes); i++)
    {
        data_t *node = (data_t *)(list_get(nodes, i));
        if (node->latitude < min_x)
        {
            min_x = node->latitude;
        }
        if (node->latitude > max_x)
        {
            max_x = node->latitude;
        }
        if (node->longitude < min_y)
        {
            min_y = node->longitude;
        }
        if (node->longitude > max_y)
        {
            max_y = node->longitude;
        }
    }
    int dx = max_x - min_x;
    int dy = max_y - min_y;
    int dmax = (dx > dy) ? dx : dy;
    int xmid = min_x + dx / 2;
    int ymid = min_y + dy / 2;
    data_t *p1, *p2, *p3;
    CHK_ALLOC(p1 = malloc(sizeof(data_t)), "malloc failed");
    p1->latitude = xmid - 1.2 * dmax;
    p1->longitude = ymid - dmax;
    p1->id = -1;
    CHK_ALLOC(p2 = malloc(sizeof(data_t)), "malloc failed");
    p2->latitude = xmid;
    p2->longitude = ymid + 10 * dmax;
    p2->id = -1;
    CHK_ALLOC(p3 = malloc(sizeof(data_t)), "malloc failed");
    p3->latitude = xmid + 1.2 * dmax;
    p3->longitude = ymid - dmax;
    p3->id = -1;
    return create_triangle(p1, p2, p3);
}

char is_point_in_circumcicle(data_t p0, data_t p1, data_t p2, data_t p3, double epsilon)
{
    double rsqr = 0;
    double fabs_y1y2 = fabs(p1.longitude - p2.longitude);
    double fabs_y2y3 = fabs(p2.longitude - p3.longitude);
    data_t pc = {.latitude = 0, .longitude = 0, .id = -1};

    /* Check for coincident points */
    if (fabs_y1y2 < epsilon && fabs_y2y3 < epsilon)
        return (0);

    if (fabs_y1y2 < epsilon)
    {
        double m2 = -(p3.latitude - p2.latitude) / (p3.longitude - p2.longitude);
        double mx2 = (p2.latitude + p3.latitude) / 2.0;
        double my2 = (p2.longitude + p3.longitude) / 2.0;
        pc.latitude = (p2.latitude + p1.latitude) / 2.0;
        pc.longitude = m2 * (pc.latitude - mx2) + my2;
    }
    else if (fabs_y2y3 < epsilon)
    {
        double m1 = -(p2.latitude - p1.latitude) / (p2.longitude - p1.longitude);
        double mx1 = (p1.latitude + p2.latitude) / 2.0;
        double my1 = (p1.longitude + p2.longitude) / 2.0;
        pc.latitude = (p3.latitude + p2.latitude) / 2.0;
        pc.longitude = m1 * (pc.latitude - mx1) + my1;
    }
    else
    {
        double m1 = -(p2.latitude - p1.latitude) / (p2.longitude - p1.longitude);
        double m2 = -(p3.latitude - p2.latitude) / (p3.longitude - p2.longitude);
        double mx1 = (p1.latitude + p2.latitude) / 2.0;
        double mx2 = (p2.latitude + p3.latitude) / 2.0;
        double my1 = (p1.longitude + p2.longitude) / 2.0;
        double my2 = (p2.longitude + p3.longitude) / 2.0;
        pc.latitude = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
        if (fabs_y1y2 > fabs_y2y3)
        {
            pc.longitude = m1 * (pc.latitude - mx1) + my1;
        }
        else
        {
            pc.longitude = m2 * (pc.latitude - mx2) + my2;
        }
    }

    double dx = p2.latitude - pc.latitude;
    double dy = p2.longitude - pc.longitude;
    rsqr = dx * dx + dy * dy;

    dx = p0.latitude - pc.latitude;
    dy = p0.longitude - pc.longitude;
    double drsqr = dx * dx + dy * dy;
    return ((drsqr - rsqr) <= epsilon);
}

void add_hole_to_triangulation(int size_polygons, edge_t polygons[], triangle_t triangulation[],
                               int *size_triangle, data_t *current_node)
{
    // add the boundary of the polygonal hole to the triangulation
    for (int o = 0; o < size_polygons; o++)
    {
        if (polygons[o].org != NULL && polygons[o].dest != NULL)
        {
            triangle_t t_tempo = {.s1 = current_node, .s2 = polygons[o].org, .s3 = polygons[o].dest};
            triangulation[*size_triangle] = t_tempo;
            (*size_triangle)++;
        }
    }
}

int find_boundary_polygon_hole(int size_badTriangle, triangle_t badTriangles[], edge_t polygons[])
{
    int size_polygons = 0;
    // find the boundary of the polygonal hole
    for (int j = 0; j < size_badTriangle; j++)
    {
        triangle_t t_tempo = badTriangles[j];
        edge_t b1 = {.org = t_tempo.s1, .dest = t_tempo.s2};
        edge_t b2 = {.org = t_tempo.s1, .dest = t_tempo.s3};
        edge_t b3 = {.org = t_tempo.s2, .dest = t_tempo.s3};
        polygons[size_polygons] = b1;
        polygons[size_polygons + 1] = b2;
        polygons[size_polygons + 2] = b3;
        char supp_e1 = 1;
        char supp_e2 = 1;
        char supp_e3 = 1;
        for (int k = 0; k < size_badTriangle; k++)
        {
            if (k == j)
                continue;
            triangle_t t_tempo2 = badTriangles[k];
            edge_t a1 = {.org = t_tempo2.s1, .dest = t_tempo2.s2};
            edge_t a2 = {.org = t_tempo2.s1, .dest = t_tempo2.s3};
            edge_t a3 = {.org = t_tempo2.s2, .dest = t_tempo2.s3};
            edge_t tmp = {.org = 0, .dest = 0};
            if (supp_e1 && edge_shared(a1, a2, a3, b1, EPSILON))
            {
                polygons[size_polygons] = tmp;
                supp_e1 = 0;
            }
            else if (supp_e2 && edge_shared(a1, a2, a3, b2, EPSILON))
            {
                polygons[size_polygons + 1] = tmp;
                supp_e2 = 0;
            }
            else if (supp_e3 && edge_shared(a1, a2, a3, b3, EPSILON))
            {
                polygons[size_polygons + 2] = tmp;
                supp_e3 = 0;
            }
        }
        size_polygons += 3;
    }
    return size_polygons;
}

void remove_bad_triangles(int size_badTriangle, triangle_t badTriangles[], triangle_t triangulation[],
                          int size_triangle)
{
    // remove bad triangles from the triangulation
    for (int n = 0; n < size_badTriangle; n++)
    {
        long int decalage = 0;
        for (int m = 0; m < size_triangle; m++)
        {
            if (triangulation[m].s1 != NULL && compare_triangle(badTriangles[n], triangulation[m], EPSILON))
            {
                decalage++;
            }
            triangulation[m] = triangulation[m + decalage];
        }
    }
}

delaunay_t *remove_null_triangles(int size_triangle, triangle_t triangulation[], triangle_t super_triangle)
{
    delaunay_t *delaunay;
    CHK_ALLOC(delaunay = malloc(sizeof(delaunay_t)), "malloc failed");
    size_t real_size = 0;
    for (int i = 0; i < size_triangle; i++)
    {
        if (triangulation[i].s1 != NULL && !compare_triangle_point(triangulation[i], super_triangle, EPSILON))
        {
            real_size++;
        }
    }
    delaunay->size_triangles = real_size;

    // Final triangulation
    triangle_t **triangulationFinal;
    CHK_ALLOC(triangulationFinal = malloc(sizeof(triangle_t) * (real_size)), "malloc failed");
    int indice_relatif = 0;
    for (int i = 0; i < size_triangle; i++)
    {
        if (triangulation[i].s1 != NULL && !compare_triangle_point(triangulation[i], super_triangle, EPSILON))
        {
            CHK_ALLOC((triangulationFinal[indice_relatif] = malloc(sizeof(triangle_t))), "malloc failed");
            triangulationFinal[indice_relatif][0] = triangulation[i];
            indice_relatif++;
        }
    }
    delaunay->triangles = triangulationFinal;
    return delaunay;
}

delaunay_t *delaunay_bowyer_watson(list_t *nodes)
{
    // This code has been created following the pseudo code available on
    // wikipedia. https://en.wikipedia.org/wiki/Bowyer%E2%80%93Watson_algorithm
    /***********************************************************************/
    triangle_t *triangulation;
    CHK_ALLOC(triangulation = calloc(sizeof(triangle_t), 100000000),
              "calloc failed"); // that malloc is huge ( ͡° ͜ʖ ͡°)
    triangle_t super_triangle = create_super_triangle(nodes);
    triangulation[0] = super_triangle;
    int size_triangle = 1;
    size_t i;
    // For each point in the list
    for (i = 0; i < list_size(nodes) && _interrupt_signals.sigint != 1; i++)
    {
        triangle_t *badTriangles;
        CHK_ALLOC(badTriangles = malloc(sizeof(triangle_t)), "malloc failed");
        int size_badTriangle = 0;
        data_t *current_node = list_get(nodes, i);
        // first find all the triangles that are no longer valid due to the
        // insertion
        for (int j = 0; j < size_triangle; j++)
        {
            triangle_t t_tempo = triangulation[j];
            // Check if the point is inside the circumcircle of the triangle_t
            if (t_tempo.s1 != NULL &&
                is_point_in_circumcicle(*current_node, *(t_tempo.s1), *(t_tempo.s2), *(t_tempo.s3), EPSILON))
            {
                CHK_ALLOC(badTriangles = realloc(badTriangles, sizeof(triangle_t) * (size_badTriangle + 1)),
                          "realloc failed");
                badTriangles[size_badTriangle] = t_tempo;
                size_badTriangle++;
            }
        }
        edge_t *polygons;
        CHK_ALLOC(polygons = malloc(sizeof(triangle_t) * size_badTriangle * 3), "malloc failed");
        // find the boundary of the polygonal hole
        int size_polygons = find_boundary_polygon_hole(size_badTriangle, badTriangles, polygons);
        // remove bad triangles from the triangulation
        remove_bad_triangles(size_badTriangle, badTriangles, triangulation, size_triangle);
        // re-triangulate the polygonal hole
        add_hole_to_triangulation(size_polygons, polygons, triangulation, &size_triangle, current_node);
        // Free memory
        free(badTriangles);
        free(polygons);
        prprintf("Delaunay", i + 1, list_size(nodes));
    }

    if (_interrupt_signals.sigint == 1)
    {
        deprintf("Delaunay interrupted at %d iteration \n", i);
    }
    delaunay_t *final = remove_null_triangles(size_triangle, triangulation, super_triangle);
    final->size_vertices = i;

    // Free memory
    free(triangulation);
    free(super_triangle.s1);
    free(super_triangle.s2);
    free(super_triangle.s3);
    return final;
}

void serialize_delaunay(delaunay_t *delaunay, FILE *fp)
{
    deprintf("Saving delaunay triangulation to file...\n");
    size_t size_data_list = delaunay->size_vertices;
    size_t size_triangulation = delaunay->size_triangles;
    deprintf("number of nodes used: %ld\n", size_data_list);
    deprintf("number of triangles: %ld\n", size_triangulation);
    CHK_FWRITE(fwrite(&size_data_list, sizeof(size_t), 1, fp), 1, "fwrite failed");
    fwrite(&size_triangulation, sizeof(size_t), 1, fp);
    for (size_t i = 0; i < size_triangulation; i++)
    {
        CHK_FWRITE(fwrite(&delaunay->triangles[i]->s1->id, sizeof(int), 1, fp), 1, "fwrite failed");
        CHK_FWRITE(fwrite(&delaunay->triangles[i]->s2->id, sizeof(int), 1, fp), 1, "fwrite failed");
        CHK_FWRITE(fwrite(&delaunay->triangles[i]->s3->id, sizeof(int), 1, fp), 1, "fwrite failed");
    }
}

delaunay_t *deserialize_delaunay(FILE *fp, list_t *data_list)
{
    size_t size_data_list;
    size_t size_triangulation;
    size_t bytes_read;
    CHK_FREAD(bytes_read = fread(&size_data_list, sizeof(size_t), 1, fp), fp, "fread failed");
    deprintf("number of nodes in the file: %ld\n", size_data_list);
    if (size_data_list > data_list->size)
    {
        ERR_MSG("the number of nodes in the file is greater than the number of "
                "nodes in the list\n");
        exit(1);
    }
    CHK_FREAD(bytes_read = fread(&size_triangulation, sizeof(size_t), 1, fp), fp, "fread failed");
    deprintf("number of triangles in the file: %ld\n", size_triangulation);
    delaunay_t *delaunay;
    CHK_ALLOC(delaunay = malloc(sizeof(delaunay_t)), "malloc failed");
    delaunay->size_triangles = size_triangulation;
    delaunay->size_vertices = size_data_list;
    CHK_ALLOC(delaunay->triangles = malloc(sizeof(triangle_t *) * size_triangulation), "malloc failed");
    for (size_t i = 0; i < size_triangulation; i++)
    {
        delaunay->triangles[i] = (triangle_t *)malloc(sizeof(triangle_t));
        int id1, id2, id3;
        CHK_FREAD(bytes_read = fread(&id1, sizeof(int), 1, fp), fp, "fread failed");
        CHK_FREAD(bytes_read = fread(&id2, sizeof(int), 1, fp), fp, "fread failed");
        CHK_FREAD(bytes_read = fread(&id3, sizeof(int), 1, fp), fp, "fread failed");
        delaunay->triangles[i]->s1 = list_get(data_list, id1);
        delaunay->triangles[i]->s2 = list_get(data_list, id2);
        delaunay->triangles[i]->s3 = list_get(data_list, id3);
    }
    return delaunay;
}

delaunay_t *initiate_delaunay(list_t *data_list, char *path_to_save, char *path_to_load)
{
    delaunay_t *delaunay;
    // If a path to load is given
    if (strcmp(path_to_load, ""))
    {
        deprintf("Loading delaunay from \"%s\" ...\n", path_to_load);
        // Open delaunay binary file
        FILE *fp_delaunay;
        CHK_ALLOC(fp_delaunay = fopen(path_to_load, "rb"), "Opening delaunay binary file");
        delaunay = deserialize_delaunay(fp_delaunay, data_list);
        fclose(fp_delaunay);
        goto end;
    }
    // If a path to save is given
    if (strcmp(path_to_save, ""))
    {
        // Apply Delaunay algorithm
        delaunay = delaunay_bowyer_watson(data_list);
        // save delaunay in a binary file
        FILE *fp_delaunay;
        CHK_ALLOC(fp_delaunay = fopen(path_to_save, "wb"), "Opening delaunay binary file");
        serialize_delaunay(delaunay, fp_delaunay);
        fclose(fp_delaunay);
        deprintf("Saved delaunay in \"%s\".\n", path_to_save);
        goto end;
    }
    else
    {
        delaunay = delaunay_bowyer_watson(data_list);
        goto end;
    }
end:
    deprintf("Delaunay loaded\n");
    return delaunay;
}

void free_delaunay(delaunay_t *delaunay)
{
    for (size_t i = 0; i < delaunay->size_triangles; i++)
    {
        free(delaunay->triangles[i]);
    }
    free(delaunay->triangles);
    free(delaunay);
}