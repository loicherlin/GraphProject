#include "../include/delauney.h"
#include <math.h>
#include <stdbool.h>

/* create_triangle */

triangle create_triangle(node_t a, node_t b, node_t c){
    triangle t = {.s1=a, .s2=b, .s3=c};
    return t;
}

triangle create_super_triangle(list_t* nodes){
    int min_x = 0;
    int max_x = 0;
    int min_y = 0;
    int max_y = 0;
    int i;
    for(i = 0; i < list_size(nodes); i++){
        node_t* node = (node_t*)(list_get(nodes, i));
        if(node->latitude < min_x){
            min_x = node->latitude;
        }
        if(node->latitude > max_x){
            max_x = node->latitude;
        }
        if(node->longitude < min_y){
            min_y = node->longitude;
        }
        if(node->longitude > max_y){
            max_y = node->longitude;
        }
    }
    int dx = max_x - min_x;
    int dy = max_y - min_y;
    int dmax = (dx > dy) ? dx : dy;
    int xmid = min_x + dx / 2;
    int ymid = min_y + dy / 2;
    node_t p1;
    p1.latitude = xmid - 20 * dmax;
    p1.longitude = ymid - dmax;
    node_t p2;
    p2.latitude = xmid;
    p2.longitude = ymid + 20 * dmax;
    node_t p3;
    p3.latitude = xmid + 20 * dmax;
    p3.longitude = ymid - dmax;
    return create_triangle(p1,p2,p3);
}

float in_circle(node_t A, node_t B, node_t C, node_t D){
    float x1 = A.latitude-D.latitude;
    float x2 = B.latitude-D.latitude;
    float x3 = C.latitude-D.latitude;
    float x4 = A.longitude-D.longitude;
    float x5 = B.longitude-D.longitude;
    float x6 = C.longitude-D.longitude;
    float x7 = (pow(A.latitude,2)-pow(D.latitude,2))+(pow(A.longitude,2)-pow(D.longitude,2));
    float x8 = (pow(B.latitude,2)-pow(D.latitude,2))+(pow(B.longitude,2)-pow(D.longitude,2));
    float x9 = (pow(C.latitude,2)-pow(D.latitude,2))+(pow(C.longitude,2)-pow(D.longitude,2));
    //printf("x1:%f x2:%f x3:%f x4:%f x5:%f x6:%f x7:%f x8:%f x9:%f\n",x1,x2,x3,x4,x5,x6,x7,x8,x9);
    float det = x1*x5*x9+x4*x8*x3+x7*x2*x6-x4*x2*x9-x1*x8*x6-x7*x5*x3;
    return det;
}


float dist(node_t p1, node_t p2)
{
    return sqrt((p1.latitude - p2.latitude)*(p1.latitude - p2.latitude) +
                (p1.longitude - p2.longitude)*(p1.longitude - p2.longitude));
}

list_t* delaunay_bowyer_watson(list_t* nodes){
    list_t* triangulation = list_create();
    triangle super_triangle = create_super_triangle(nodes);
    list_append(triangulation, &super_triangle);
    for(int i = 0 ; i < list_size(nodes);i++){
        list_t* badTriangles = list_create();
        node_t* a = list_get(nodes, i);
        for(int j = 0 ; j < list_size(triangulation) ; j++){
            triangle* t_tempo = list_get(triangulation, j);
            // Check if the point is inside the circumcircle of the triangle
            if(in_circle(t_tempo->s1,t_tempo->s2,t_tempo->s3,*a)>0){
                list_append(badTriangles, t_tempo);
            }
        }
        list_t* polygon = list_create();
        // find the boundary of the polygonal hole
        for(int j = 0 ; j < list_size(badTriangles) ; j++){
            triangle* t_tempo = list_get(badTriangles, j);

        }
        // remove bad triangles from the triangulation
        for(int n = 0; n < list_size(badTriangles); n++){
            list_remove(triangulation, list_get(badTriangles, n));
        }
        // re-triangulate the polygonal hole
        for(int o = 0; o < list_size(polygon); o++){
            // newTri := form a triangle from edge to point
            // ?????????????????????????????????
            /*
            float* edge = list_get(polygon, o);
            triangle* newTri = malloc(sizeof(triangle));
            newTri->s1 = edge[0];
            newTri->s2 = edge[1];
            newTri->s3 = *a;
            */
            // ????????????????????????????????
            // add newTri to triangulation
            //list_append(triangulation, newTri);
        }
        for(int p = 0; p < list_size(triangulation); p++){
          triangle* t = list_get(triangulation, p);
          // if triangle contains a vertex from original super-triangle
          // remove triangle from triangulation
          
          // ??????????????????????????????????
        }

    }
    //printf("triangles : %ld", list_size(triangles));
    return triangulation;
}

