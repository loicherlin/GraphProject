#include "../include/visualise.h"
#include <stdio.h>
#include <stdlib.h>
#include <tps.h>


char _is_prim_active = 0;
char _is_delaunay_active = 1;

// texts
char number_nodes[100];
char press_key[100];
char number_edges[100];
char sum_weight[100] = " ";


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

void update_texts(list_t* node_list, triangle_t** delaunay, graph_t* g, int* mst,int flag){
    switch (flag){
    case TXT_DEFAULT:
        snprintf(press_key,100, "Show Prim: [key O]");
        snprintf(number_nodes,100, "Number of nodes: %ld", list_size(node_list));
        snprintf(number_edges,100, "Number of edges: %f", (delaunay[0][0].s1->latitude - 1) * 3);
        break;
    case TXT_DELAUNAY:
        snprintf(press_key,100, "Show Prim: [key O]");
        snprintf(number_edges,100, "Number of edges: %d", g->size_edges);
        snprintf(sum_weight,100, " ");
        break;
    case TXT_PRIM:
        snprintf(press_key,100, "Show Delaunay: [key O]");
        snprintf(number_edges,100, "Number of edges: %ld", (list_size(node_list) - 1));
        snprintf(sum_weight,100, "Sum of weights: %f", sum_weight_graph(mst, node_list));
        break;
    default:
        break;
    }
}

void draw_texts(){
    tps_drawText(20, 20, press_key, 30);
    tps_drawText(20, 60, number_nodes, 15);
    tps_drawText(20, 80, number_edges, 15);
    tps_drawText(20, 100, sum_weight, 15);
}

void visualize(int width, int height, list_t* node_list, int* mst, triangle_t** delaunay, graph_t* g){
    tps_createWindow("Trees of Paris", width, height);
    double x_max;
    double x_min = 50;
    double y_max;
    double y_min = 10;
    get_xy_min_max(node_list, &x_max, &x_min, &y_max, &y_min);
    screen_t s = {.width = width, .height = height, .x_max = x_max, .x_min = x_min, .y_max = y_max, .y_min = y_min};
    update_texts(node_list, delaunay, g, mst, TXT_DEFAULT);
    while(tps_isRunning()){
        tps_background(255,255,255);
        tps_setColor(0,0,0);
        draw_texts();
        if(_is_delaunay_active == 1){
            update_texts(node_list, delaunay, g, mst, TXT_DELAUNAY);
            show_delaunay(s, node_list, delaunay);
        }
        else if(_is_prim_active == 1) {
            update_texts(node_list, delaunay, g, mst, TXT_PRIM);
            show_mst(s, node_list, mst);    
        }
        tps_render();
    }
    tps_closeWindow();
}

void show_mst(screen_t s, list_t* node_list, int* mst){
    double x_max = s.x_max;
    double x_min = s.x_min;
    double y_max = s.y_max;
    double y_min = s.y_min;
    int width = s.width;
    int height = s.height;
    for(size_t i = 0; i < list_size(node_list); i++){
        data_t d = *((data_t*)list_get(node_list, i));
        double xNorm = normalize_to_screen(d.latitude, x_max, x_min);
        double yNorm = normalize_to_screen(d.longitude, y_max, y_min);
        if(i != 0){
            data_t d2 = *((data_t*)list_get(node_list, mst[i]));
            double xNorm2 = normalize_to_screen(d2.latitude, x_max, x_min);
            double yNorm2 = normalize_to_screen(d2.longitude, y_max, y_min);
            tps_drawLine(xNorm * width, yNorm * height, xNorm2 * width, yNorm2 * height);
        }
    }
}

void show_delaunay(screen_t s, list_t* node_list, triangle_t** triangles){
    double x_max = s.x_max;
    double x_min = s.x_min;
    double y_max = s.y_max;
    double y_min = s.y_min;
    int width = s.width;
    int height = s.height;
    for(size_t i = 1; i < triangles[0][0].s1->latitude; i++){
        triangle_t* t = triangles[i];
        // draw edges
        if(t->s1->latitude != 0 && t->s1->latitude < 249){
            // draw edge between s1 and s2
            tps_drawLine(normalize_to_screen(t->s1->latitude, x_max, x_min) * width, 
                        normalize_to_screen(t->s1->longitude, y_max, y_min) * height, 
                        normalize_to_screen(t->s2->latitude, x_max, x_min) * width, 
                        normalize_to_screen(t->s2->longitude, y_max, y_min) * height);
            // draw edge between s2 and s3
            tps_drawLine(normalize_to_screen(t->s2->latitude, x_max, x_min) * width,
                        normalize_to_screen(t->s2->longitude, y_max, y_min) * height,
                        normalize_to_screen(t->s3->latitude, x_max, x_min) * width,
                        normalize_to_screen(t->s3->longitude, y_max, y_min) * height);
            // draw edge between s3 and s1
            tps_drawLine(normalize_to_screen(t->s3->latitude, x_max, x_min) * width,
                        normalize_to_screen(t->s3->longitude, y_max, y_min) * height,
                        normalize_to_screen(t->s1->latitude, x_max, x_min) * width,
                        normalize_to_screen(t->s1->longitude, y_max, y_min) * height);
        }
    }
}

void onKeyDown(int key){
    if(key == SDLK_o){
        _is_delaunay_active = !_is_delaunay_active;
        _is_prim_active = !_is_prim_active;
    }
}
