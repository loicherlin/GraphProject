#pragma once
#include "array_list.h"


/**
 * Sanatize coordinates that are in form of "lattitude,longitude"
 * @param lattitude
 * @param longitude
 * @param coordinates
 */
void sanatize_coordinates(double* lattitude, double* longitude, char* coordinates);

/**
 * Write serialized data to fp.
 * Return 1 if done it successfuly, otherwise 0.
 * fp is not closed into this function.
 * @param str
 * @param fp
 * @param n
 * @return 1 if done it successfuly, otherwise 0
 * @note fp is not closed into this function.
 */
int write_to_bin(char** str, FILE* fp, int n);

/**
 * Build binary file from fp that represent a csv file, saved to path_bin 
 * @param fp
 * @param path_bin
 * @param delimiter
 * @return 1 if done it succesfuly, otherwise 0
 * @note fp is not closed into this function.
 * @note delimiter is by default ';'
 */
int build_bin(FILE* fp, char* path_bin, char delimiter);

/**
 * Read and deserialize binary file from fp and return a list of theses data.
 * and return a list of theses data. 
 * @param fp
 * @return list of data_t
 * @note fp is not closed into this function.
 */
list_t* get_data_bin(FILE* fp);