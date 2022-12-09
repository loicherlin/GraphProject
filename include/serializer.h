#pragma once
#include <stdint.h>

typedef struct data
{
    double latitude;
    double longitude;
    int id;
}data_t;

/**
 * Serialize data_t
 * @param latitude
 * @param longitude
 * @param id
 * @return serialized data_t
 */
char* serialize_data_t(double latitude, double longitude, int id);

/**
 * Deserialize data_t
 * @param data_b
 * @return data_t
 */
data_t* deserialize_data_t(char* data_b);

/**
 * Compare two data_t
 * @param a
 * @param b
 * @param epsilon
 * @return 1 if a == b according to epsilon, 0 otherwise
 */
int compare_data_t(data_t* a, data_t* b, double epsilon);

/**
 * Print data_t
 * @param da
 */
void print_data_t(data_t* da);
