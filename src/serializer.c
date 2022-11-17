#include "../include/serializer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



char* serialize_data_t(double latitude, double longitude){
    data_t da = {.latitude = latitude, .longitude = longitude };
    char* data = malloc(sizeof(data_t));
    memcpy(data, &da, sizeof(da));
    return data;
}


data_t* deserialize_data_t(char* data_b){
    data_t* da = malloc(sizeof(data_t));
    memcpy(da, data_b, sizeof(data_t));
    return da;
}