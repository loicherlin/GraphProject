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

// ----------------------- TESTS CSV_PARSER AND BIN_BUILDER ----------------------- //

FILE* get_fp_bin(char* path_csv, char* path_bin, char delimiter){
    FILE* fp = fopen(path_csv, "r");
    if(fp == NULL){
        perror("Error while opening csv file");
        exit(1);
    }
    // Build bin file based on fp
    int result = build_csv_bin(fp, path_bin, delimiter);
    if(result == EXIT_FAILURE){ 
        fclose(fp);
        return NULL;
    }
    // Open bin file to read it
    FILE* fp_bin = fopen(path_bin, "rb");
    if(fp_bin == NULL){
        perror("Error while opening bin file");
        fclose(fp);
        exit(1);
    }
    fclose(fp);
    return fp_bin;
}

/**
 * @brief Test if the csv file is correctly parsed
 */
void test_csv_to_bin(){
    FILE* fp_bin = get_fp_bin("./files/csv/0", "./files/bin/.tmp/0.bin", ';');
    // Check if fp_bin is not NULL
    tps_assert(fp_bin != NULL);
    // Get data from bin file
    list_t* data_list = get_data_csv_bin(fp_bin);
    // Close files
    fclose(fp_bin);
    // Check if data_list is not empty
    tps_assert(list_size(data_list) != 0);
    data_t* d1 = list_get(data_list, 0);
    // Check if data is not NULL
    tps_assert(d1 != NULL);
    // Check if data is correct
    tps_assert(d1->latitude == 0.0);
    tps_assert(d1->longitude == 0.0);
    // Free data_t parsed and data_list
    free(d1);
    list_free(data_list);
}

/**
 * @brief Test case where the csv file is not correct
 * (file is empty)
 */
void test_wrong_csv_1(void){
    FILE* fp_bin = get_fp_bin("./files/csv/1", "./files/bin/.tmp/1.bin", ';');
    tps_assert(fp_bin == NULL);
}

/**
 * @brief Test case where the csv file is not correct
 * (file have only one line)
 */
void test_wrong_csv_2(void){
    FILE* fp_bin = get_fp_bin("./files/csv/2", "./files/bin/.tmp/2.bin", ';');
    tps_assert(fp_bin == NULL);
}

/**
 * @brief Test case where the csv file is not correct
 * (header is empty but file is not)
 */
void test_wrong_csv_3(void){
    FILE* fp_bin = get_fp_bin("./files/csv/3", "./files/bin/.tmp/3.bin", ';');
    tps_assert(fp_bin == NULL);
}

/**
 * @brief Test case where the csv file is not correct
 * (split line crash because csv file is incorrect)
 */
void test_wrong_csv_4(void){
    // Needs to use fork to test because it will use exit(EXIT_FAILURE)
    pid_t pid = fork();
    assert(pid >= 0);
    if( pid == 0 ) {
        get_fp_bin("./files/csv/4", "./files/bin/.tmp/4.bin", ';');
        exit(0);
    }
    else {
        int status;
        wait(&status);
        // Check if child process exit with EXIT_FAILURE
        if(WEXITSTATUS(status) == EXIT_FAILURE)
            tps_assert(1);
        else
            tps_assert(0);
    }
}

/**
 * @brief Test case where the csv file is not correct
 * (split line crash because coordinates are not in a good format correct)
 */
void test_wrong_csv_5(void){
    // Needs to use fork to test because it will use exit(EXIT_FAILURE)
    pid_t pid = fork();
    assert(pid >= 0);
    if( pid == 0 ) {
        get_fp_bin("./files/csv/5", "./files/bin/.tmp/5.bin", ';');
        exit(0);
    }
    else {
        int status;
        wait(&status);
        // Check if child process exit with EXIT_FAILURE
        if(WEXITSTATUS(status) == EXIT_FAILURE)
            tps_assert(1);
        else
            tps_assert(0);
    }
}

void unit_test_csv_bin(void){
    TEST(test_csv_to_bin);
    TEST(test_wrong_csv_1);
    TEST(test_wrong_csv_2);
    TEST(test_wrong_csv_3);
    TEST(test_wrong_csv_4);
    TEST(test_wrong_csv_5);
}

// ----------------------- TESTS DATA_T ----------------------- //

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

/**
 * @brief Simple test case for Delaunay triangulation
 * (4 points: 0,0;0,1;1,0;1,1)
 */
void test_delaunay_1(void){
    // Get data from bin file
    FILE* fp_bin = get_fp_bin("./files/delaunay/1", "./files/bin/.tmp/delaunay_1", ';');
    list_t* data_list = get_data_csv_bin(fp_bin);
    // Close files
    fclose(fp_bin);
    // Check if data_list is not empty
    tps_assert(list_size(data_list) != 0);
    delaunay_t* delaunay = delaunay_bowyer_watson(data_list);
    // Check if delaunay is not NULL
    tps_assert(delaunay != NULL);
    // Check if delaunay is correct
    tps_assert(delaunay->size_triangles == 2);
    tps_assert(delaunay->size_vertices == 4);
    // Free
    free_delaunay(delaunay);
    for(size_t i = 0; i < list_size(data_list); i++)
        free(list_get(data_list, i));
    list_free(data_list);
}

void unit_test_delaunay(void){
    TEST(test_delaunay_1);
}

int main(void){
    unit_test_csv_bin();
    unit_test_data_t();
    unit_test_delaunay();
    /*
    unit_test_triangle();
    unit_test_min_heap();
    unit_test_graph();
    unit_test_visualise();
    unit_test_args_parser();
    unit_test_handler();
*/
    return 0;
}