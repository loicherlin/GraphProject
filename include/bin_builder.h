#pragma once
#include "array_list.h"

/**
 * @defgroup BinBuilderModule
 * @{
 * @brief Build and read binary files from csv files.
 */

/**
 * Sanatize coordinates that are in form of "lattitude,longitude"
 * @param lattitude
 * @param longitude
 * @param coordinates
 */
void sanatize_coordinates(double *lattitude, double *longitude, char *coordinates);

/**
 * Write serialized data to fp.
 * Return 1 if done it successfuly, otherwise 0.
 * fp is not closed into this function.
 * @param str
 * @param fp
 * @param n
 * @return 0 if done it successfuly, otherwise 1
 * @note fp is not closed into this function.
 */
int write_to_csv_bin(char **str, FILE *fp, int n);

/**
 * Build binary file from fp that represent a csv file, saved to path_bin
 * @param fp
 * @param path_bin
 * @param delimiter
 * @return 0 if done it succesfuly, otherwise 1
 * @note fp is not closed into this function.
 * @note delimiter is by default ';'
 */
int build_csv_bin(FILE *fp, char *path_bin, char delimiter);

/**
 * Read and deserialize binary file from fp that represent a csv file,
 * and return a list of theses data.
 * @param fp
 * @return list of data_t
 * @note fp is not closed into this function.
 */
list_t *get_data_csv_bin(FILE *fp);
/** @} */