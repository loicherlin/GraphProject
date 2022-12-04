#include <stdio.h>
#include <stdlib.h>
#include <tps.h>
#include "../include/visualise.h"
#include "../include/array_list.h"
#include "../include/serializer.h"
#include "../include/prim.h"

double normalize_to_screen(double coord, double coord_max, double coord_min){
    return (coord - coord_min) / (coord_max - coord_min);
}

void get_xy_min_max(list_t* node_list, double* x_max, double* x_min, double* y_max, double* y_min){
    for(size_t i = 0; i < list_size(node_list); i++){
        if((*(data_t*)list_get(node_list, i)).latitude >= *x_max){
            *x_max = (*(data_t*)list_get(node_list, i)).latitude;
        }
        if((*(data_t*)list_get(node_list, i)).latitude < *x_min){
            *x_min = (*(data_t*)list_get(node_list, i)).latitude;
        }
        if((*(data_t*)list_get(node_list, i)).longitude >= *y_max){
            *y_max = (*(data_t*)list_get(node_list, i)).longitude;
        }
        if((*(data_t*)list_get(node_list, i)).longitude < *y_min){
            *y_min = (*(data_t*)list_get(node_list, i)).longitude;
        }
    }   
}

void show_mst(int width, int height, list_t* node_list, int* mst){
    tps_createWindow("Tree of Paris", width, height);
    double x_max;
    double x_min = 50;
    double y_max;
    double y_min = 10;
    get_xy_min_max(node_list, &x_max, &x_min, &y_max, &y_min);
    printf("%f %f %f %f\n", x_max, x_min, y_max, y_min);
    while(tps_isRunning()) {
        tps_background(255,255,255);
        tps_setColor(0,0,0);
        for(size_t i = 0; i < list_size(node_list); i++){
            //printf("%ld %d\n", i, mst[i]);
            data_t d = *((data_t*)list_get(node_list, i));
            double xNorm = normalize_to_screen(d.latitude, x_max, x_min);
            double yNorm = normalize_to_screen(d.longitude, y_max, y_min);
            //tps_drawEllipse(xNorm * width, yNorm * height, 10, 10);
            if(i != 0 && mst[i] != -1){
                data_t d2 = *((data_t*)list_get(node_list, mst[i]));
                double xNorm2 = normalize_to_screen(d2.latitude, x_max, x_min);
                double yNorm2 = normalize_to_screen(d2.longitude, y_max, y_min);
                tps_drawLine(xNorm * width, yNorm * height, xNorm2 * width, yNorm2 * height);
            }
        }
        tps_render();
    }
    tps_closeWindow();
}

void show_delaunay(int width, int height, list_t* node_list, triangle** triangles){
    tps_createWindow("Tree of Paris", width, height);
    double x_max;
    double x_min = 50;
    double y_max;
    double y_min = 10;
    get_xy_min_max(node_list, &x_max, &x_min, &y_max, &y_min);
    printf("%f %f %f %f\n", x_max, x_min, y_max, y_min);
    while(tps_isRunning()) {
        tps_background(255,255,255);
        tps_setColor(0,0,0);
        for(size_t i = 1; i < triangles[0][0].s1->latitude; i++){
            triangle* t = triangles[i];
            // draw edges
            if(t->s1->latitude != 0 && t->s1->latitude < 249){
                tps_drawLine(normalize_to_screen(t->s1->latitude, x_max, x_min) * width, normalize_to_screen(t->s1->longitude, y_max, y_min) * height, normalize_to_screen(t->s2->latitude, x_max, x_min) * width, normalize_to_screen(t->s2->longitude, y_max, y_min) * height);
                tps_drawLine(normalize_to_screen(t->s2->latitude, x_max, x_min) * width, normalize_to_screen(t->s2->longitude, y_max, y_min) * height, normalize_to_screen(t->s3->latitude, x_max, x_min) * width, normalize_to_screen(t->s3->longitude, y_max, y_min) * height);
                tps_drawLine(normalize_to_screen(t->s3->latitude, x_max, x_min) * width, normalize_to_screen(t->s3->longitude, y_max, y_min) * height, normalize_to_screen(t->s1->latitude, x_max, x_min) * width, normalize_to_screen(t->s1->longitude, y_max, y_min) * height);
            }
        }
        tps_render();
    }
    tps_closeWindow();
}