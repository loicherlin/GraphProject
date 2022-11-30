#include "../include/delauney.h"
#include <math.h>
#include <stdbool.h>

/* create_triangle */

void permuter(node_t *a, node_t *b) {
    //printf("passe permuter\n");
    node_t tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    //printf("passe permuter2\n");
}
void triRapid(list_t* tab, int first, int last) {
    int pivot, i, j;
    if(first < last) {
        pivot = first;
        i = first;
        j = last;
        while (i < j) {
            //printf("passe get\n");
            node_t* n1 = list_get(tab,i);
            node_t* n2 = list_get(tab,pivot);
            node_t* n3 = list_get(tab,j);
            //printf("%f %f %f %f %f %f\n",n1->latitude,n1->longitude,n2->latitude,n2->longitude,n3->latitude,n2->longitude);
            while(n1->latitude <= n2->latitude && i < last){
                i++;
                n1 = list_get(tab,i);}
            while(n3->latitude > n2->latitude){
                j--;
                n3 = list_get(tab,j);}
            if(i < j) {
                permuter(n1, n3);
            }
        }
        node_t* n2 = list_get(tab,pivot);
        node_t* n3 = list_get(tab,j);
        permuter(n2, n3);
        triRapid(tab, first, j - 1);
        triRapid(tab, j + 1, last);
    }
}


triangle create_triangle(node_t* a, node_t* b, node_t* c){
    triangle t;
    t.s1=a;
    t.s2=b;
    t.s3=c;
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
    node_t* p1 = malloc(sizeof(node_t));
    p1->latitude = xmid - 20 * dmax;
    p1->longitude = ymid - dmax;
    node_t* p2 =  malloc(sizeof(node_t));
    p2->latitude = xmid;
    p2->longitude = ymid + 20 * dmax;
    node_t* p3 =  malloc(sizeof(node_t));;
    p3->latitude = xmid + 20 * dmax;
    p3->longitude = ymid - dmax;
    return create_triangle(p1,p3,p2);
}



char in_circle(node_t p0, node_t p1, node_t p2, node_t p3, double epsilon)
{
    //printf("In Circle : %f %f %f %f %f %f %f %f\n",p0.latitude,p0.longitude,p1.latitude,p1.longitude,p2.latitude,p2.longitude,p3.latitude,p3.longitude);
    double rsqr = 0;
    double fabs_y1y2 = fabs(p1.longitude-p2.longitude);
    double fabs_y2y3 = fabs(p2.longitude-p3.longitude);
    node_t pc ={.latitude=0,.longitude=0};

    /* Check for coincident points */
    if (fabs_y1y2 < epsilon && fabs_y2y3 < epsilon)
        return(0);

    if (fabs_y1y2 < epsilon) {
        double m2 = - (p3.latitude - p2.latitude) / (p3.longitude-p2.longitude);
        double mx2 = (p2.latitude + p3.latitude) / 2.0;
        double my2 = (p2.longitude + p3.longitude) / 2.0;
        pc.latitude = (p2.latitude + p1.latitude) / 2.0;
        pc.longitude = m2 * (pc.latitude - mx2) + my2;
    } else if (fabs_y2y3 < epsilon) {
        double m1 = - (p2.latitude-p1.latitude) / (p2.longitude-p1.longitude);
        double mx1 = (p1.latitude + p2.latitude) / 2.0;
        double my1 = (p1.longitude + p2.longitude) / 2.0;
        pc.latitude = (p3.latitude + p2.latitude) / 2.0;
        pc.longitude = m1 * (pc.latitude - mx1) + my1;
    } else {
        double m1 = - (p2.latitude-p1.latitude) / (p2.longitude-p1.longitude);
        double m2 = - (p3.latitude-p2.latitude) / (p3.longitude-p2.longitude);
        double mx1 = (p1.latitude + p2.latitude) / 2.0;
        double mx2 = (p2.latitude + p3.latitude) / 2.0;
        double my1 = (p1.longitude + p2.longitude) / 2.0;
        double my2 = (p2.longitude + p3.longitude) / 2.0;
        pc.latitude = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
        if (fabs_y1y2 > fabs_y2y3) {
            pc.longitude = m1 * (pc.latitude - mx1) + my1;
        } else {
            pc.longitude = m2 * (pc.latitude - mx2) + my2;
        }
    }

    double dx = p2.latitude - pc.latitude;
    double dy = p2.longitude - pc.longitude;
    rsqr = dx*dx + dy*dy;

    dx = p0.latitude - pc.latitude;
    dy = p0.longitude - pc.longitude;
    double drsqr = dx*dx + dy*dy;
    //printf("InCicle : %f %f\n",drsqr,rsqr);

    return ((drsqr - rsqr) <= epsilon);
}



int compare_node_t_equal(node_t* a, node_t* b){
    float epsilon = 0.0000001;
    return ((fabs(a->latitude-b->latitude) < epsilon) && (fabs(a->longitude-b->longitude) < epsilon)) ? 1 : 0;
}

// check if current is equals to bad_t_a, ... , bad_t_c.
int edge_shared(edge_t bad_t_a, edge_t bad_t_b, edge_t bad_t_c, edge_t current){
    if((compare_node_t_equal(current.org, bad_t_a.org) && compare_node_t_equal(current.dest, bad_t_a.dest)) || 
       (compare_node_t_equal(current.org, bad_t_b.org) && compare_node_t_equal(current.dest, bad_t_b.dest)) || 
       (compare_node_t_equal(current.org, bad_t_c.org) && compare_node_t_equal(current.dest, bad_t_c.dest)) || 
       
       (compare_node_t_equal(current.org, bad_t_a.dest) && compare_node_t_equal(current.dest, bad_t_a.org)) || 
       (compare_node_t_equal(current.org, bad_t_b.dest) && compare_node_t_equal(current.dest, bad_t_b.org)) || 
       (compare_node_t_equal(current.org, bad_t_c.dest) && compare_node_t_equal(current.dest, bad_t_c.org))){
        return 1;
       }
       else{
        return 0;
       }
}


int compare_triangle_node(triangle a, triangle b){
    if (
    compare_node_t_equal(a.s1, b.s1) || compare_node_t_equal(a.s1, b.s2) || compare_node_t_equal(a.s1, b.s3) || 
    compare_node_t_equal(a.s2, b.s1) || compare_node_t_equal(a.s2, b.s2) || compare_node_t_equal(a.s2, b.s3) ||
    compare_node_t_equal(a.s3, b.s1) || compare_node_t_equal(a.s3, b.s2) || compare_node_t_equal(a.s3, b.s3) ){
        return 1;
    }
    else{
        return 0;
    }
}

int compare_triangle(triangle a, triangle b){
    if (
    (compare_node_t_equal(a.s1, b.s1) || compare_node_t_equal(a.s1, b.s2) || compare_node_t_equal(a.s1, b.s3)) && 
    (compare_node_t_equal(a.s2, b.s1) || compare_node_t_equal(a.s2, b.s2) || compare_node_t_equal(a.s2, b.s3)) &&
    (compare_node_t_equal(a.s3, b.s1) || compare_node_t_equal(a.s3, b.s2) || compare_node_t_equal(a.s3, b.s3)) ){
        return 1;
    }
    else{
        return 0;
    }
}

triangle** delaunay_bowyer_watson(list_t* nodes){
    triangle* triangulation = malloc(sizeof(triangle)*1000000);
    triangle super_triangle = create_super_triangle(nodes);
    //triRapid(nodes,0,list_size(nodes)-1);
    int size_triangle = 1;
    triangulation[0]=super_triangle;
    // CHANGE 4 AFTER THE BUG HAVE BEEN FOUND !!!!!!!
    for(int i = 0 ; i < 4;i++){

        triangle* badTriangles = malloc(sizeof(triangle)*10000);
        int size_badTriangle = 0;
        node_t* a = list_get(nodes, i);

        for(int j = 0 ; j < size_triangle ; j++){
            triangle t_tempo = triangulation[j];
            // Check if the point is inside the circumcircle of the triangle
            if(t_tempo.s1!=NULL && in_circle(*a,*(t_tempo.s1),*(t_tempo.s2),*(t_tempo.s3),0.0001)){
                badTriangles[size_badTriangle]=t_tempo;
                size_badTriangle++;
                //list_remove(triangulation,t_tempo);
            }
        }
        edge_t* polygon = malloc(sizeof(triangle)*size_badTriangle*3);
        int size_polygone= 0;

        // find the boundary of the polygonal hole
        for(int j = 0 ; j < size_badTriangle; j++){
            triangle t_tempo = badTriangles[j];
            edge_t b1;
            edge_t b2;
            edge_t b3;
            b1.org= t_tempo.s1; b1.dest=t_tempo.s2;
            b2.org= t_tempo.s1; b2.dest=t_tempo.s3;
            b3.org= t_tempo.s2; b3.dest=t_tempo.s3;
            polygon[size_polygone]=b1;
            polygon[size_polygone+1]=b2;
            polygon[size_polygone+2]=b3;
            char supp_e1=1;
            char supp_e2=1;
            char supp_e3=1;
            for(int k=0 ;  k < size_badTriangle ; k++){
                triangle t_tempo2 = badTriangles[k];
                edge_t a1 ={.org = t_tempo2.s1, .dest=t_tempo2.s2};
                edge_t a2 ={.org = t_tempo2.s1, .dest=t_tempo2.s3};
                edge_t a3 ={.org = t_tempo2.s2, .dest=t_tempo2.s3};
                    if(supp_e1 && edge_shared(a1,a2,a3,b1)){
                        edge_t temp ={.org = 0, .dest= 0};
                        polygon[size_polygone]=temp;
                        printf("entreedge1\n");
                        supp_e1=0;
                    }
                    else if(supp_e2 && edge_shared(a1,a2,a3,b2)){
                        edge_t temp ={.org = 0, .dest= 0};
                        polygon[size_polygone+1]=temp;
                        printf("entreedge2\n");
                        supp_e2=0;
                    }
                    else if(supp_e3 && edge_shared(a1,a2,a3,b3)){
                        edge_t temp ={.org = 0, .dest= 0};//marche pas je pense
                        polygon[size_polygone+2]=temp;
                        printf("entreedge3\n");
                        supp_e3=0;
                    }
            }
            size_polygone+=3;
        }
        
        printf("passage : %d polygone size : %d, badTriangle size : %d\n",i ,size_polygone,size_badTriangle);
        // remove bad triangles from the triangulation
        triangle triangle_vide={.s1=NULL,.s2=NULL,.s3=NULL};
        for(int n = 0; n < size_badTriangle; n++){
            for(int m=0; m < size_triangle; m++){
                if(triangulation[m].s1!=NULL && compare_triangle(badTriangles[n],triangulation[m])){
                    triangulation[m]= triangle_vide;
                    //printf("entretriangleRemove\n");
                }
            }
        }
        // re-triangulate the polygonal hole
        for(int o = 0; o < size_polygone; o++){
            edge_t edge = polygon[o];
            if(edge.dest!=0){
                triangle t;
                t.s1 = edge.org;
                t.s2 = edge.dest;
                t.s3 = a;
                triangulation[size_triangle]=t;
                size_triangle++;
            }
        }
        free(badTriangles);
    }

    size_t taille_real = 1;
    for(int i = 1 ; i < size_triangle ; i++){
        if(triangulation[i].s1!=NULL && !compare_triangle_node(triangulation[i], super_triangle)){
            taille_real++;
        }
    }
    triangle** triangulationFinal = malloc(sizeof(triangle)*(taille_real+1));
    triangle* t = malloc(sizeof(triangle));
    node_t *  n= malloc(sizeof(node_t));
    n->latitude=taille_real;
    t->s1=n;

    int indice_relatif=1;
    for (int i = 0 ; i < size_triangle ; i++){
        if(triangulation[i].s1!=NULL && !compare_triangle_node(triangulation[i], super_triangle)){
            triangulationFinal[indice_relatif] = malloc(sizeof(triangle));
            triangulationFinal[indice_relatif][0] = triangulation[i];
            indice_relatif++;
        }
    }
    triangulationFinal[0]=t;
    return triangulationFinal;
}

//note : regarder l'ensemble des polygone ajouté