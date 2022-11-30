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

float in_circle(node_t* a, node_t* b, node_t* c, node_t* D){
        node_t* A = a;
    node_t* B = b;
    node_t* C = c;
    float min=a->longitude;
    if(min < b->longitude && min < c->longitude){
        A=a;
        B=b;
        C=c;
    }
    if(min > b->longitude && c->longitude > b->longitude){
        A=b;
        B=a;
        C=c;
    }
    else{
        A=c;
        B=b;
        C=a;
    }
    float coef_direct = (C->latitude-A->latitude)/(C->longitude-A->longitude);
    if(B->latitude > (coef_direct*(B->longitude))+(A->latitude-(coef_direct*A->longitude))){
        node_t* tempo = C;
        C=B;
        B=tempo;
    }
    //printf("x1:%f x2:%f x3:%f x4:%f x5:%f x6:%f\n",A->latitude,A->longitude,B->latitude,B->longitude,B->latitude,B->longitude);
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
    //printf("determinant : %f\n",det);
    return det;
}


float dist(node_t p1, node_t p2)
{
    return sqrt((p1.latitude - p2.latitude)*(p1.latitude - p2.latitude) +
                (p1.longitude - p2.longitude)*(p1.longitude - p2.longitude));
}

int compare_node_t(node_t a, node_t b){
    //printf("Triangle A : %f %f  Triangle B : %f %f \n",a.latitude,b.latitude, a.longitude,b.longitude);
    float epsilon = 0.0000001;
    return ((fabs(a.latitude-b.latitude) < epsilon) && (fabs(a.longitude-b.longitude) < epsilon)) ? 1 : 0;
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

void list_remove_triangle(list_t* l ,triangle* rm ){
    size_t i = 0 ; 
    size_t longueur=list_size(l);
    while(i<longueur){
        //printf("--------------- list_remove_triangle ---------------\n");
        //printf("%d %ld \n",i,longueur);
        triangle* courant = list_get(l,i);
        //printf("%f %f %f\n",courant->s1->latitude, courant->s2->latitude, courant->s3->latitude);
        if((compare_node_t( *(courant->s1) , *(rm->s1)) || compare_node_t(*(courant->s1), *(rm->s2)) || compare_node_t(*(courant->s1), *(rm->s3)) )&& 
    (compare_node_t(*(courant->s2), *(rm->s1)) || compare_node_t(*(courant->s2), *(rm->s2)) || compare_node_t(*(courant->s2), *(rm->s3)) )&& 
    (compare_node_t(*(courant->s3), *(rm->s1)) || compare_node_t(*(courant->s3), *(rm->s2)) || compare_node_t(*(courant->s3), *(rm->s3)) )){
            //printf("test\n");
            triangle* e = list_take(l,i);
            //printf("test : %f %f %f\n",e->s1->latitude, e->s2->latitude,e->s3->latitude);
            if(e!=NULL){
                //free(e);
            }
            longueur--;
        }
        else{
            i++;
        }
    }
    //printf("end\n");
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
    list_append(triangulation, create_super_triangle(nodes));
    // CHANGE 4 AFTER THE BUG HAVE BEEN FOUND !!!!!!!
    for(int i = 0 ; i < 4;i++){
        list_t* badTriangles = list_create();
        node_t* a = list_get(nodes, i);

        for(int j = 0 ; j < list_size(triangulation) ; j++){
            triangle* t_tempo = list_get(triangulation, j);
            // Check if the point is inside the circumcircle of the triangle
            if(in_circle(t_tempo->s1,t_tempo->s2,t_tempo->s3,a)>0){
                list_append(badTriangles, t_tempo);
                //list_remove(triangulation,t_tempo);
            }
        }
        list_t* polygon = list_create();
        
        // find the boundary of the polygonal hole
        for(int j = 0 ; j < list_size(badTriangles) ; j++){
            triangle* t_tempo = list_get(badTriangles, j);
            edge_t* b1 = malloc(sizeof(edge_t));
            edge_t* b2 = malloc(sizeof(edge_t));
            edge_t* b3 = malloc(sizeof(edge_t));
            b1->org= t_tempo->s1; b1->dest=t_tempo->s2;
            b2->org= t_tempo->s1; b2->dest=t_tempo->s3;
            b3->org= t_tempo->s2; b3->dest=t_tempo->s3;
            list_append(polygon,b1);
            list_append(polygon,b2);
            list_append(polygon,b3);
            for(int k=0 ;  k < list_size(badTriangles) ; k++){
                triangle* t_tempo2 = list_get(badTriangles, k);
                edge_t a1 ={.org = t_tempo2->s1, .dest=t_tempo2->s2};
                edge_t a2 ={.org = t_tempo2->s1, .dest=t_tempo2->s3};
                edge_t a3 ={.org = t_tempo2->s2, .dest=t_tempo2->s3};
                char flag=0;
                if(j!=k){
                    if(edge_shared(a1,a2,a3,*b1)){
                        edge_t* e = list_take(polygon, list_size(polygon)-3);
                        free(e);
                        flag=1;
                    }
                    else if(edge_shared(a1,a2,a3,*b2)){
                        edge_t* e = list_take(polygon, list_size(polygon)-2);
                        free(e);
                        flag=1;
                    }
                    else if(edge_shared(a1,a2,a3,*b3)){
                        edge_t* e = list_take(polygon, list_size(polygon)-1);
                        free(e);
                        flag=1;
                    }
                    if(flag){
                        break;
                    }
                }
            }
        }
        
        printf("passage : %d polygone size : %ld, badTriangle size : %ld\n",i ,list_size(polygon),list_size(badTriangles));
        // remove bad triangles from the triangulation
        for(int n = 0; n < list_size(badTriangles); n++){
            list_remove_triangle(triangulation, (triangle*)list_get(badTriangles, n));
        }
        // re-triangulate the polygonal hole
        for(int o = 0; o < list_size(polygon); o++){
            edge_t* edge = list_get(polygon, o);
            triangle* t = malloc(sizeof(triangle)); // memory leak
            t->s1 = edge->org;
            t->s2 = edge->dest;
            t->s3 = a;
            list_append(triangulation, t);
        }
        while(list_size(polygon) > 0){
            edge_t* t = list_take(polygon, list_size(polygon)-1);
            free(t);
        }
        list_free(polygon); 
        list_free(badTriangles);
    }
    for(int p = 0; p < list_size(triangulation); p++){
          triangle* t = list_get(triangulation, p);
          /*if(compare_triangle_node(*t,*super_triangle)){
            triangle* r = list_take(triangulation, p);
            free(r);
          }*/
    }
    //free(super_triangle);
    return triangulation;
}

//note : regarder l'ensemble des polygone ajouté