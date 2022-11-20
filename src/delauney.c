#include "../include/delauney.h"
#include <math.h>

/* create_triangle */

triangle create_triangle(data_t a, data_t b, data_t c){
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
        data_t* node = (data_t*)(list_get(nodes, i));
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
    data_t p1;
    p1.latitude = xmid - 20 * dmax;
    p1.longitude = ymid - dmax;
    data_t p2;
    p2.latitude = xmid;
    p2.longitude = ymid + 20 * dmax;
    data_t p3;
    p3.latitude = xmid + 20 * dmax;
    p3.longitude = ymid - dmax;
    return create_triangle(p1,p2,p3);
}

double determine_triangle(data_t A, data_t B, data_t C, data_t D){
    double x1 = A.latitude-D.latitude;
    double x2 = B.latitude-D.latitude;
    double x3 = C.latitude-D.latitude;
    double x4 = A.longitude-D.longitude;
    double x5 = B.longitude-D.longitude;
    double x6 = C.longitude-D.longitude;
    double x7 = (pow(A.latitude,2)-pow(D.latitude,2))+(pow(A.longitude,2)-pow(D.longitude,2));
    double x8 = (pow(B.latitude,2)-pow(D.latitude,2))+(pow(B.longitude,2)-pow(D.longitude,2));
    double x9 = (pow(C.latitude,2)-pow(D.latitude,2))+(pow(C.longitude,2)-pow(D.longitude,2));
    //printf("x1:%f x2:%f x3:%f x4:%f x5:%f x6:%f x7:%f x8:%f x9:%f\n",x1,x2,x3,x4,x5,x6,x7,x8,x9);
    double det = x1*x5*x9+x4*x8*x3+x7*x2*x6-x4*x2*x9-x1*x8*x6-x7*x5*x3;
    return det;
}
/* implement Bowyer–Watson algorithm */
void triangulate(list_t* nodes){
    list_t* triangles =list_create();
    triangle a = create_super_triangle(nodes);
    list_append(triangles,&a);
    for(int i = 0 ; i < list_size(nodes);i++){

        list_t* badTriangles =list_create();
        data_t* a = list_get(nodes,i);

        for(int j = 0 ; j < list_size(triangles) ; j++){

            triangle* t_tempo = list_get(triangles,j);

            if(determine_triangle(t_tempo->s1,t_tempo->s2,t_tempo->s3,*a)>0){
                list_append(badTriangles,(void *)t_tempo);
            }
        }
        /*function BowyerWatson (pointList)
   // pointList is a set of coordinates defining the points to be triangulated
   triangulation := empty triangle mesh data structure
   add super-triangle to triangulation // must be large enough to completely contain all the points in pointList
   for each point in pointList do // add all the points one at a time to the triangulation
      badTriangles := empty set
      for each triangle in triangulation do // first find all the triangles that are no longer valid due to the insertion
         if point is inside circumcircle of triangle
            add triangle to badTriangles
------------------------------------------------------------------------------------------------------
      polygon := empty set
      for each triangle in badTriangles do // find the boundary of the polygonal hole
         for each edge in triangle do
            if edge is not shared by any other triangles in badTriangles
               add edge to polygon
      for each triangle in badTriangles do // remove them from the data structure
         remove triangle from triangulation
      for each edge in polygon do // re-triangulate the polygonal hole
         newTri := form a triangle from edge to point
         add newTri to triangulation
   for each triangle in triangulation // done inserting points, now clean up
      if triangle contains a vertex from original super-triangle
         remove triangle from triangulation
   return triangulation*/
    }
    printf("s1 : %f %f s2 : %f %f s3 : %f %f \n",a.s1.latitude,a.s1.longitude,a.s2.latitude,a.s2.longitude,a.s3.latitude,a.s3.longitude);
}