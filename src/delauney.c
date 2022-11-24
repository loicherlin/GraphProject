#include "../include/delauney.h"
#include <math.h>
#include <stdbool.h>

/* create_triangle */

triangle* create_triangle(node_t* a, node_t* b, node_t* c){
    triangle* t = malloc(sizeof(triangle));
    t->s1=a;
    t->s2=b;
    t->s3=c;
    return t;
}

triangle* create_super_triangle(list_t* nodes){
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
    node_t* p1 = malloc(sizeof(node_t));
    p1->latitude = 0;
    p1->longitude = 0;
    node_t* p2 =  malloc(sizeof(node_t));
    p2->latitude = 500;
    p2->longitude = 0;
    node_t* p3 =  malloc(sizeof(node_t));;
    p3->latitude = 250;
    p3->longitude = 500;
    return create_triangle(p1,p2,p3);
}

float in_circle(node_t* A, node_t* B, node_t* C, node_t* D){
    float x1 = A->latitude-D->latitude;
    float x2 = B->latitude-D->latitude;
    float x3 = C->latitude-D->latitude;
    float x4 = A->longitude-D->longitude;
    float x5 = B->longitude-D->longitude;
    float x6 = C->longitude-D->longitude;
    //printf("x1:%f x2:%f x3:%f x4:%f x5:%f x6:%f\n",x1,x2,x3,x4,x5,x6);
    float x7 = (pow(A->latitude,2)-pow(D->latitude,2))+(pow(A->longitude,2)-pow(D->longitude,2));
    float x8 = (pow(B->latitude,2)-pow(D->latitude,2))+(pow(B->longitude,2)-pow(D->longitude,2));
    float x9 = (pow(C->latitude,2)-pow(D->latitude,2))+(pow(C->longitude,2)-pow(D->longitude,2));
    //printf("x1:%f x2:%f x3:%f x4:%f x5:%f x6:%f x7:%f x8:%f x9:%f\n",x1,x2,x3,x4,x5,x6,x7,x8,x9);
    float det = x1*x5*x9+x4*x8*x3+x7*x2*x6-x4*x2*x9-x1*x8*x6-x7*x5*x3;
    return det;
}


float dist(node_t p1, node_t p2)
{
    return sqrt((p1.latitude - p2.latitude)*(p1.latitude - p2.latitude) +
                (p1.longitude - p2.longitude)*(p1.longitude - p2.longitude));
}

int compare_node_t(node_t a, node_t b){
    return (a.latitude == b.latitude) && (a.longitude == b.longitude) ? 1 : 0;
}

// check if current is equals to bad_t_a, ... , bad_t_c.
int edge_shared(edge_t bad_t_a, edge_t bad_t_b, edge_t bad_t_c, edge_t current){
    if((compare_node_t(*(current.org), *(bad_t_a.org)) && compare_node_t(*(current.dest), *(bad_t_a.dest))) || 
       (compare_node_t(*(current.org), *(bad_t_b.org)) && compare_node_t(*(current.dest), *(bad_t_b.dest))) || 
       (compare_node_t(*(current.org), *(bad_t_c.org)) && compare_node_t(*(current.dest), *(bad_t_c.dest))) || 
       
       (compare_node_t(*(current.org), *(bad_t_a.dest)) && compare_node_t(*(current.dest), *(bad_t_a.org))) || 
       (compare_node_t(*(current.org), *(bad_t_b.dest)) && compare_node_t(*(current.dest), *(bad_t_b.org))) || 
       (compare_node_t(*(current.org), *(bad_t_c.dest)) && compare_node_t(*(current.dest), *(bad_t_c.org)))){
        return 1;
       }
       else{
        return 0;
       }
}


int compare_triangle_node(triangle a, triangle b){
    return (
    compare_node_t(*(a.s1), *(b.s1)) || compare_node_t(*(a.s1), *(b.s2)) || compare_node_t(*(a.s1), *(b.s3)) || 
    compare_node_t(*(a.s2), *(b.s1)) || compare_node_t(*(a.s2), *(b.s2)) || compare_node_t(*(a.s2), *(b.s3)) ||
    compare_node_t(*(a.s3), *(b.s1)) || compare_node_t(*(a.s3), *(b.s2)) || compare_node_t(*(a.s3), *(b.s3)) ) ? 1 : 0;
}

list_t* delaunay_bowyer_watson(list_t* nodes){
    list_t* triangulation = list_create();
    triangle* super_triangle = create_super_triangle(nodes);
    
    //printf("%f %f %f %f %f %f\n",super_triangle->s1->latitude,super_triangle->s1->longitude,super_triangle->s2->latitude,super_triangle->s2->longitude,super_triangle->s3->latitude,super_triangle->s3->longitude);
    list_append(triangulation, super_triangle);
    // CHANGE 4 AFTER THE BUG HAVE BEEN FOUND !!!!!!!
    for(int i = 0 ; i < 4;i++){
        printf("%d nb triangle : %ld \n",i, list_size(triangulation));
        list_t* badTriangles = list_create();
         node_t* a = list_get(nodes, i);
         printf("%f %f\n",a->latitude,a->longitude);
         
        for(int j = 0 ; j < list_size(triangulation) ; j++){
            triangle* t_tempo = list_get(triangulation, j);
            // Check if the point is inside the circumcircle of the triangle
            //printf("le derterminant : %f\n",in_circle(t_tempo->s1,t_tempo->s2,t_tempo->s3,a));
            if(in_circle(t_tempo->s1,t_tempo->s2,t_tempo->s3,a)>0){
                //printf("passe %d in circle\n",i);
                list_append(badTriangles, t_tempo);
            }
        }
        list_t* polygon = list_create();
        
        // find the boundary of the polygonal hole
        //printf("longueur bad : %ld", list_size(badTriangles));
        for(int j = 0 ; j < list_size(badTriangles) ; j++){
            //printf("longueur bad : %ld\n", list_size(badTriangles));
            triangle* t_tempo = list_get(badTriangles, j);
            edge_t a1 ={.org = t_tempo->s1, .dest=t_tempo->s2};
            edge_t a2 ={.org = t_tempo->s1, .dest=t_tempo->s3};
            edge_t a3 ={.org = t_tempo->s2, .dest=t_tempo->s3};
            for(int k=0 ;  k < list_size(badTriangles) ; k++){
                triangle* t_tempo2 = list_get(badTriangles, k);
                edge_t* b1 = malloc(sizeof(edge_t));
                edge_t* b2 = malloc(sizeof(edge_t));
                edge_t* b3 = malloc(sizeof(edge_t));
                b1->org= t_tempo2->s1; b1->dest=t_tempo2->s2;
                b2->org= t_tempo2->s1; b2->dest=t_tempo2->s3;
                b3->org= t_tempo2->s2; b3->dest=t_tempo2->s3;
                list_append(polygon,b1);
                list_append(polygon,b2);
                list_append(polygon,b3);
                if(j!=k){
                    if(!edge_shared(a1,a2,a3,*b1)){
                        //list_remove(polygon,b1);
                        edge_t* e = list_take(polygon, list_size(polygon)-3);
                        free(e);
                    }
                    else if(!edge_shared(a1,a2,a3,*b2)){
                        edge_t* e = list_take(polygon, list_size(polygon)-2);
                        free(e);
                    }
                    else if(!edge_shared(a1,a2,a3,*b3)){
                        edge_t* e = list_take(polygon, list_size(polygon)-1);
                        free(e);
                    }
                }
            }
        }
        
        // remove bad triangles from the triangulation
        for(int n = 0; n < list_size(badTriangles); n++){
            list_remove(triangulation, list_get(badTriangles, n));
        }
        // re-triangulate the polygonal hole
        for(int o = 0; o < list_size(polygon); o++){
            edge_t* edge = list_get(polygon, o);
            triangle* newTri = malloc(sizeof(triangle));
            newTri->s1 = edge->org; // regarde si copie
            newTri->s2 = edge->dest;
            newTri->s3 = a;

            //printf("trie : %f %f %f %f %f %f\n",newTri->s1->latitude,newTri->s1->longitude,newTri->s2->latitude,newTri->s2->longitude,newTri->s3->latitude,newTri->s3->longitude);
    
            list_append(triangulation, newTri);
        }
        for(int o = 0 ; o<list_size(polygon);o++){
            free(list_get(polygon,o));
        }
        list_free(polygon);
    }
    for(int p = 0; p < list_size(triangulation); p++){
          triangle* t = list_get(triangulation, p);
          if(compare_triangle_node(*t,*super_triangle)){
            list_remove(triangulation,t);
          }
    }
    return triangulation;
}

