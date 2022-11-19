#include "../include/convert_coordinate.h"
#include <math.h>

const int EARTH_RADIUS = 6371;

/* convert latitude longitude to x y coordinate */
void ll_to_cartesian(float latitude, float longitude, float* x, float* y){
    *x = 100*cos(latitude * M_PI / 180) * cos(longitude * M_PI / 180);
    *y = 100*cos(latitude * M_PI / 180) * sin(longitude * M_PI / 180);
}
