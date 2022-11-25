#pragma once
#include <stdint.h>

typedef struct data
{
    float latitude;
    float longitude;
    int id;
}data_t;

char* serialize_data_t(float latitude, float longitude, int id);

data_t* deserialize_data_t(char* data_b);

void print_data_t(data_t* da);