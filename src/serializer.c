#include "../include/serializer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



char* serialize_data_t(float latitude, float longitude, int id){
    data_t da = {.latitude = latitude, .longitude = longitude, .id = id };
    char* data = malloc(sizeof(data_t));
    memcpy(data, &da, sizeof(da));
    return data;
}


node_t* deserialize_data_t(char* data_b){
    node_t* da = malloc(sizeof(node_t));
    memcpy(da, data_b, sizeof(node_t));
    return da;
}

void print_data_t(data_t* da){
    printf("%d [%f, %f] ", da->id, da->latitude, da->longitude);
}