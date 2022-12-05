#include "../include/delaunay.h"
#include <math.h>
#include "../include/serializer.h"
#include <stdbool.h>
#define EPSILON  0.00000000000000000000001


triangle_t create_triangle(data_t* a, data_t* b, data_t* c){
    triangle_t t;
    t.s1=a;
    t.s2=b;
    t.s3=c;
    return t;
}


triangle_t create_super_triangle(list_t* nodes){
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
    data_t* p1 = malloc(sizeof(data_t));
    p1->latitude = xmid - 1.2 * dmax;
    p1->longitude = ymid - dmax;
    p1->id = -1;
    data_t* p2 =  malloc(sizeof(data_t));
    p2->latitude = xmid;
    p2->longitude = ymid + 10 * dmax;
    p2->id = -1;
    data_t* p3 =  malloc(sizeof(data_t));;
    p3->latitude = xmid + 1.2 * dmax;
    p3->longitude = ymid - dmax;
    p3->id = -1;
    return create_triangle(p1,p2,p3);
}



char in_circle(data_t p0, data_t p1, data_t p2, data_t p3, double epsilon)
{
    double rsqr = 0;
    double fabs_y1y2 = fabs(p1.longitude-p2.longitude);
    double fabs_y2y3 = fabs(p2.longitude-p3.longitude);
    data_t pc ={.latitude=0,.longitude=0, .id=-1};

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
    return ((drsqr - rsqr) <= epsilon);
}



// check if current is equals to bad_t_a, ... , bad_t_c.
int edge_shared(edge_t bad_t_a, edge_t bad_t_b, edge_t bad_t_c, edge_t current){
    return (compare_data_t(bad_t_a.org, current.org, EPSILON) && compare_data_t(bad_t_a.dest, current.dest, EPSILON)) ||
           (compare_data_t(bad_t_a.org, current.dest, EPSILON) && compare_data_t(bad_t_a.dest, current.org, EPSILON)) ||
           (compare_data_t(bad_t_b.org, current.org, EPSILON) && compare_data_t(bad_t_b.dest, current.dest, EPSILON)) ||
           (compare_data_t(bad_t_b.org, current.dest, EPSILON) && compare_data_t(bad_t_b.dest, current.org, EPSILON)) ||
           (compare_data_t(bad_t_c.org, current.org, EPSILON) && compare_data_t(bad_t_c.dest, current.dest, EPSILON)) ||
           (compare_data_t(bad_t_c.org, current.dest, EPSILON) && compare_data_t(bad_t_c.dest, current.org, EPSILON));
}


int compare_triangle_node(triangle_t a, triangle_t b){
    return (compare_data_t(a.s1, b.s1, EPSILON) || compare_data_t(a.s1, b.s2, EPSILON) || compare_data_t(a.s1, b.s3, EPSILON) || 
    compare_data_t(a.s2, b.s1, EPSILON) || compare_data_t(a.s2, b.s2, EPSILON) || compare_data_t(a.s2, b.s3, EPSILON) ||
    compare_data_t(a.s3, b.s1, EPSILON) || compare_data_t(a.s3, b.s2, EPSILON) || compare_data_t(a.s3, b.s3, EPSILON));

}

int compare_triangle(triangle_t a, triangle_t b){
    return (compare_data_t(a.s1, b.s1, EPSILON) || compare_data_t(a.s1, b.s2, EPSILON) || compare_data_t(a.s1, b.s3, EPSILON)) && 
    (compare_data_t(a.s2, b.s1, EPSILON) || compare_data_t(a.s2, b.s2, EPSILON) || compare_data_t(a.s2, b.s3, EPSILON)) &&
    (compare_data_t(a.s3, b.s1, EPSILON) || compare_data_t(a.s3, b.s2, EPSILON) || compare_data_t(a.s3, b.s3, EPSILON));
}


triangle_t** delaunay_bowyer_watson(list_t* nodes){
    triangle_t* triangulation = malloc(sizeof(triangle_t)*100000000); // that malloc is huge !!
    triangle_t super_triangle = create_super_triangle(nodes);
    int size_triangle = 1;
    triangulation[0] = super_triangle;
    
    for(int i = 0 ; i < list_size(nodes);i++){
        triangle_t* badTriangles = malloc(sizeof(triangle_t));
        int size_badTriangle = 0;
        data_t* a = list_get(nodes, i);

        for(int j = 0 ; j < size_triangle ; j++){
            triangle_t t_tempo = triangulation[j];
            // Check if the point is inside the circumcircle of the triangle_t
            if(t_tempo.s1!=NULL && in_circle(*a,*(t_tempo.s1),*(t_tempo.s2),*(t_tempo.s3), EPSILON)){
                badTriangles = realloc(badTriangles, sizeof(triangle_t)*(size_badTriangle+1));
                badTriangles[size_badTriangle] = t_tempo;
                size_badTriangle++;
            }
        }
        edge_t* polygon = malloc(sizeof(triangle_t)*size_badTriangle*3);
        int size_polygone= 0;
        // find the boundary of the polygonal hole
        for(int j = 0 ; j < size_badTriangle; j++){
            triangle_t t_tempo = badTriangles[j];
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
                triangle_t t_tempo2 = badTriangles[k];
                edge_t a1 ={.org = t_tempo2.s1, .dest=t_tempo2.s2};
                edge_t a2 ={.org = t_tempo2.s1, .dest=t_tempo2.s3};
                edge_t a3 ={.org = t_tempo2.s2, .dest=t_tempo2.s3};
                if(k!=j){
                    if(supp_e1 && edge_shared(a1,a2,a3,b1)){
                        edge_t temp ={.org = 0, .dest= 0};
                        polygon[size_polygone]=temp;
                        supp_e1=0;
                    }
                    else if(supp_e2 && edge_shared(a1,a2,a3,b2)){
                        edge_t temp ={.org = 0, .dest= 0};
                        polygon[size_polygone+1]=temp;
                        supp_e2=0;
                    }
                    else if(supp_e3 && edge_shared(a1,a2,a3,b3)){
                        edge_t temp ={.org = 0, .dest= 0};
                        polygon[size_polygone+2]=temp;
                        supp_e3=0;
                    }
                }
            }
            size_polygone+=3;
        }
        printf("\e[1;1H\e[2J"); // clear screen
        printf("passage : %d polygone size : %d, badTriangle size : %d\n",i ,size_polygone,size_badTriangle);

        long int decalage = 0;
        // remove bad triangles from the triangulation
        for(int n = 0; n < size_badTriangle; n++){
            decalage=0;
            for(int m=0; m < size_triangle; m++){
                if(triangulation[m].s1!=NULL && compare_triangle(badTriangles[n],triangulation[m])){
                    decalage++;
                }
                triangulation[m] = triangulation[m+decalage];
            }
        }
        // re-triangulate the polygonal hole
        for(int o = 0 ; o < size_polygone ; o++){
            if(polygon[o].org!=NULL && polygon[o].dest!=NULL){
                triangle_t t_tempo = {.s1 = a, .s2 = polygon[o].org, .s3 = polygon[o].dest};
                triangulation[size_triangle] = t_tempo;
                size_triangle++;
            }
        }
        free(badTriangles);
        free(polygon);
    }

    size_t taille_real = 1;
    for(int i = 1 ; i < size_triangle ; i++){
        if(triangulation[i].s1!=NULL && !compare_triangle_node(triangulation[i], super_triangle)){
            taille_real++;
        }
    }
    triangle_t** triangulationFinal = malloc(sizeof(triangle_t)*(taille_real+1));
    triangle_t* t = malloc(sizeof(triangle_t));
    data_t *  n= malloc(sizeof(data_t));
    n->latitude=taille_real;
    t->s1=n;

    int indice_relatif=1;
    // I think it should be avoided to do a triangulationFinal
    // and better to clean triangulation
    for (int i = 0 ; i < size_triangle ; i++){
        if(triangulation[i].s1!=NULL && !compare_triangle_node(triangulation[i], super_triangle)){
            triangulationFinal[indice_relatif] = malloc(sizeof(triangle_t));
            triangulationFinal[indice_relatif][0] = triangulation[i];
            indice_relatif++;
        }
    }
    triangulationFinal[0]=t;
    free(triangulation);
    free(super_triangle.s1);
    free(super_triangle.s2);
    free(super_triangle.s3);
    return triangulationFinal;
}

//note : regarder l'ensemble des polygone ajouté