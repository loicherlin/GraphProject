#pragma once
#include <stdint.h>

typedef struct data
{
    double x;
    double y;
}data_t;

char* serialize_data_t(double x, double y);

data_t deserialize_data_t(char* data_b);