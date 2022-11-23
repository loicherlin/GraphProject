#pragma once
#include <stdint.h>

typedef struct data
{
    float latitude;
    float longitude;
}node_t;

char* serialize_data_t(float latitude, float longitude);

node_t* deserialize_data_t(char* data_b);