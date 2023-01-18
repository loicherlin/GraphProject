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

#include "tps_unit_test.h"

/**
 * @brief Simple test case for Delaunay triangulation
 * (4 points: 0,0;0,1;1,0;1,1)
 */
void test_delaunay_1(void) {
    // Get data from bin file
    FILE *fp_bin =
        get_fp_bin("./files/delaunay/1", "./files/bin/.tmp/delaunay_1", ';');
    list_t *data_list = get_data_csv_bin(fp_bin);
    // Close files
    fclose(fp_bin);
    // Check if data_list is not empty
    tps_assert(list_size(data_list) != 0);
    delaunay_t *delaunay = delaunay_bowyer_watson(data_list);
    // Check if delaunay is not NULL
    tps_assert(delaunay != NULL);
    // Check if delaunay is correct
    tps_assert(delaunay->size_triangles == 2);
    tps_assert(delaunay->size_vertices == 4);
    // Check if triangles are correct
    //....

    // Free
    free_delaunay(delaunay);
    for (size_t i = 0; i < list_size(data_list); i++)
        free(list_get(data_list, i));
    list_free(data_list);
}

/**
 * Test case where we are loading a delaunay triangulation correctly
 */
void test_delaunay_load(void) {
    pid_t pid = fork();
    assert(pid >= 0);
    if (pid == 0) {
        FILE *fp_bin = get_fp_bin("./files/csv/100_tree",
                                  "./files/bin/.tmp/100_tree.bin", ';');
        list_t *data_list = get_data_csv_bin(fp_bin);
        // Close files
        fclose(fp_bin);
        // Check if data_list is not empty
        tps_assert(list_size(data_list) != 0);
        delaunay_t *delaunay = initiate_delaunay(
            data_list, "", "./files/delaunay/to_load/delaunay_100.bin");
        // Check if delaunay is not NULL
        tps_assert(delaunay != NULL);
        // Check if delaunay is correct
        tps_assert(delaunay->size_vertices == 100);
        tps_assert(delaunay->size_triangles == 188);

        // Free
        free_delaunay(delaunay);
        for (size_t i = 0; i < list_size(data_list); i++)
            free(list_get(data_list, i));
        list_free(data_list);
        exit(EXIT_SUCCESS);
    } else {
        int status;
        wait(&status);
        // Check if child process exit with EXIT_SUCCESS
        if (WEXITSTATUS(status) == EXIT_SUCCESS)
            tps_assert(1);
        else
            tps_assert(0);
    }
}

/**
 * Test case where we are loading a delaunay triangulation not correctly
 * (Delaunay triangulation bin file has a size of nodes superior to the number
 * of nodes in the csv file)
 */
void test_delaunay_load_fail_1(void) {
    pid_t pid = fork();
    assert(pid >= 0);
    if (pid == 0) {
        FILE *fp_bin = get_fp_bin("./files/csv/100_tree",
                                  "./files/bin/.tmp/100_tree.bin", ';');
        list_t *data_list = get_data_csv_bin(fp_bin);
        // Close files
        fclose(fp_bin);
        // Check if data_list is not empty
        tps_assert(list_size(data_list) != 0);
        initiate_delaunay(data_list, "",
                          "./files/delaunay/to_load/delaunay_256K");
        exit(EXIT_SUCCESS);
    } else {
        int status;
        wait(&status);
        // Check if child process exit with EXIT_FAILURE
        if (WEXITSTATUS(status) == EXIT_FAILURE)
            tps_assert(1);
        else
            tps_assert(0);
    }
}

/**
 * Test case where we are loading a delaunay triangulation not correctly
 * (Delaunay triangulation bin file path is not correct)
 */
void test_delaunay_load_fail_2(void) {
    pid_t pid = fork();
    assert(pid >= 0);
    if (pid == 0) {
        FILE *fp_bin = get_fp_bin("./files/csv/100_tree",
                                  "./files/bin/.tmp/100_tree.bin", ';');
        list_t *data_list = get_data_csv_bin(fp_bin);
        // Close files
        fclose(fp_bin);
        // Check if data_list is not empty
        tps_assert(list_size(data_list) != 0);
        // There is no delaunay_256K.bin file
        initiate_delaunay(data_list, "",
                          "./files/delaunay/to_load/delaunay_256K.bin");
        exit(EXIT_SUCCESS);
    } else {
        int status;
        wait(&status);
        // Check if child process exit with EXIT_FAILURE
        if (WEXITSTATUS(status) == EXIT_FAILURE)
            tps_assert(1);
        else
            tps_assert(0);
    }
}

void test_delaunay_save_and_load(void) {
    pid_t pid = fork();
    assert(pid >= 0);
    if (pid == 0) {
        FILE *fp_bin = get_fp_bin("./files/csv/100_tree",
                                  "./files/bin/.tmp/100_tree.bin", ';');
        list_t *data_list = get_data_csv_bin(fp_bin);
        // Close files
        fclose(fp_bin);
        // Check if data_list is not empty
        tps_assert(list_size(data_list) != 0);

        delaunay_t *delaunay_saved = initiate_delaunay(
            data_list, "./files/bin/.tmp/delaunay_100.bin", "");
        // Check if delaunay is not NULL
        tps_assert(delaunay_saved != NULL);
        // Check if delaunay is correct
        tps_assert(delaunay_saved->size_vertices == 100);
        tps_assert(delaunay_saved->size_triangles == 188);

        delaunay_t *delaunay_loaded = initiate_delaunay(
            data_list, "", "./files/bin/.tmp/delaunay_100.bin");
        // Check if delaunay is not NULL
        tps_assert(delaunay_loaded != NULL);
        // Check if delaunay is correct
        tps_assert(delaunay_loaded->size_vertices == 100);
        tps_assert(delaunay_loaded->size_triangles == 188);

        // Free
        free_delaunay(delaunay_loaded);
        free_delaunay(delaunay_saved);
        for (size_t i = 0; i < list_size(data_list); i++)
            free(list_get(data_list, i));
        list_free(data_list);
        exit(EXIT_SUCCESS);
    } else {
        int status;
        wait(&status);
        // Check if child process exit with EXIT_SUCCESS
        if (WEXITSTATUS(status) == EXIT_SUCCESS)
            tps_assert(1);
        else
            tps_assert(0);
    }
}

void unit_test_delaunay(void) {
    TEST(test_delaunay_1);
    TEST(test_delaunay_load);
    TEST(test_delaunay_load_fail_1);
    TEST(test_delaunay_load_fail_2);
    TEST(test_delaunay_save_and_load);
}