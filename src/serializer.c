#include "../include/serializer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



char* serialize_data_t(float latitude, float longitude){
    node_t da = {.latitude = latitude, .longitude = longitude };
    char* data = malloc(sizeof(node_t));
    memcpy(data, &da, sizeof(da));
    return data;
}


node_t* deserialize_data_t(char* data_b){
    node_t* da = malloc(sizeof(node_t));
    memcpy(da, data_b, sizeof(node_t));
    return da;
}