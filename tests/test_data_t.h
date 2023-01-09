#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/args_parser.h"
#include "../include/array_list.h"
#include "../include/bin_builder.h"
#include "../include/cprintf.h"
#include "../include/csv_parser.h"
#include "../include/data_t.h"
#include "../include/delaunay.h"
#include "../include/graph.h"
#include "../include/handler.h"
#include "../include/min_heap.h"
#include "../include/triangle.h"
#include "../include/visualise.h"


#include "tps_unit_test.h"


void test_data_t(void){
    data_t d1 = {.id = 0, .latitude = 1.123, .longitude = 1.123};
    data_t d2 = {.id = 1, .latitude = 1.123, .longitude = 1.123};
    // check if d1 and d2 are equals
    tps_assert(compare_data_t(&d1, &d2, 0.000001) == 1);
    data_t d3 = {.id = 0, .latitude = 1.123, .longitude = 1.125};
    // check if d1 and d3 are not equals
    tps_assert(compare_data_t(&d1, &d3, 0.000001) == 0);
}

void unit_test_data_t(void){
    TEST(test_data_t);
}