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
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// Contains tests according to the test file name
#include "./test_csv.h"
#include "./test_data_t.h"
#include "./test_delaunay.h"
#include "./test_graph.h"
#include "./test_min_heap.h"

#include "tps_unit_test.h"

int main(void) {
    // To hide stderr messages from the tests (mainly error messages)
    // fclose(stderr);
    unit_test_csv_bin();
    unit_test_data_t();
    unit_test_delaunay();
    unit_test_min_heap();
    unit_test_graph();
    /*
    unit_test_triangle();
    unit_test_visualise();
    unit_test_args_parser();
    unit_test_handler();
*/
    return 0;
}