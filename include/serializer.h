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

int compare_data_t(data_t* a, data_t* b, double epsilon);

void print_data_t(data_t* da);
