#pragma once
#include <stdint.h>

typedef struct data
{
    double latitude;
    double longitude;
    int id;
}data_t;

char* serialize_data_t(double latitude, double longitude, int id);

data_t* deserialize_data_t(char* data_b);

void print_data_t(data_t* da);