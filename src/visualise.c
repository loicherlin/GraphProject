#include <stdio.h>
#include <stdlib.h>
#include <tps.h>
#include "../include/visualise.h"
#include "../include/array_list.h"
#include "../include/serializer.h"


double normalize_to_screen(double coord, double coord_max, double coord_min){
    return (coord - coord_min) / (coord_max - coord_min);
}


int get_xy_min_max(list_t* node_list, double* x_max, double* x_min, double* y_max, double* y_min){
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
    return EXIT_SUCCESS;
}

void show_data(int width, int height, list_t* node_list){
    tps_createWindow("Tree of Paris", width, height);
    double x_max;
    double x_min = 50;
    double y_max;
    double y_min = 10;
    get_xy_min_max(node_list, &x_max, &x_min, &y_max, &y_min);
    while(tps_isRunning()) {
        tps_background(255,255,255);
        tps_setColor(0,0,0);
        for(size_t i = 0; i < list_size(node_list); i++){
            data_t d = *((data_t*)list_get(node_list, i));
            double xNorm = normalize_to_screen(d.latitude, x_max, x_min);
            double yNorm = normalize_to_screen(d.longitude, y_max, y_min);
            tps_drawEllipse(xNorm * width, yNorm * height, 2, 2);
        }
        tps_render();
    }
    tps_closeWindow();
    for(size_t i = 0; i < list_size(node_list); i++)
        free(list_get(node_list, i));

    list_free(node_list);
}