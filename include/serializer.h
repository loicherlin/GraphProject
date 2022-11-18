#pragma once
#include <stdint.h>

typedef struct data
{
    double latitude;
    double longitude;
}data_t;

char* serialize_data_t(double latitude, double longitude);

data_t* deserialize_data_t(char* data_b);