#include "../include/serializer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



char* serialize_data_t(double latitude, double longitude, int id){
    //data_t da = {.latitude = latitude, .longitude = longitude, .id = id };
    char* data = calloc(sizeof(data_t),1);
    ((data_t*)data)->id = id;
    ((data_t*)data)->latitude = latitude;
    ((data_t*)data)->longitude = longitude;
    //memcpy(data, &da, sizeof(data_t));
    return data;
}


data_t* deserialize_data_t(char* data_b){
    data_t* da = malloc(sizeof(data_t));
    memcpy(da, data_b, sizeof(data_t));
    return da;
}

int compare_data_t(data_t* a, data_t* b, double epsilon){
    return ((fabs(a->latitude-b->latitude) < epsilon) && (fabs(a->longitude-b->longitude) < epsilon));
}

void print_data_t(data_t* da){
    printf("%d [%f, %f]\n", da->id, da->latitude, da->longitude);
}