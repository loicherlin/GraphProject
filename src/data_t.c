#include "../include/data_t.h"
#include "../include/cprintf.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *serialize_data_t(double latitude, double longitude, int id)
{
    // data_t da = {.latitude = latitude, .longitude = longitude, .id = id };
    char *data;
    CHK_ALLOC((data = calloc(sizeof(data_t), 1)), "calloc failed");
    ((data_t *)data)->id = id;
    ((data_t *)data)->latitude = latitude;
    ((data_t *)data)->longitude = longitude;
    // memcpy(data, &da, sizeof(data_t));
    return data;
}

data_t *deserialize_data_t(char *data_b)
{
    data_t *da;
    CHK_ALLOC(da = malloc(sizeof(data_t)), "malloc failed");
    memcpy(da, data_b, sizeof(data_t));
    return da;
}

int compare_data_t(data_t *a, data_t *b, double epsilon)
{
    return ((fabs(a->latitude - b->latitude) < epsilon) && (fabs(a->longitude - b->longitude) < epsilon));
}

void print_data_t(data_t *da) { printf("%d [%f, %f]\n", da->id, da->latitude, da->longitude); }

// compare two data_t for qsort functions
int qsort_compare_data_t(const void *d1, const void *d2)
{
    // d1 and d2 comes from a list of data_t, more precisely from a void** array
    // so we need to cast them to void* and then to data_t*
    data_t *aa = *((data_t **)d1);
    data_t *bb = *((data_t **)d2);
    double a = aa->latitude;
    double b = bb->latitude;
    if (a > b)
        return 1;
    else if (a < b)
        return -1;
    else
        return 0;
}