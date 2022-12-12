#pragma once

/**
 * Open a file
 * @param path
 * @return file pointer
 */
FILE* open_file(char* path);

/**
 * Get the line of fp, splitted into an array of strings
 * @param fp
 * @param n
 * @param delimiter
 * @return array of strings
 * @note delimiter is by default ';'
 */
char** get_line(FILE* fp, int n, char delimiter);

/**
 * Print the content of a line that was been sliced
 * @param line_splitted
 * @param n (size of line_splitted)
 */
void print_line(char** line_splitted, int n);

/**
 * Get the number of column by counting how many delimiter have the header
 * @param fp
 * @param delimiter
 * @return number of column
 */
int size_column(FILE* fp, char delimiter);

/**
 * Split the line into an array
 * @param line
 * @param delimiter
 * @param n (size)
 * @return array of strings
 */
char** split_line(char* line, char* delimiter, int n);

/**
 * Skip the header of the file
 * @param fp
 */
void skip_header(FILE* fp);

/**
 * Free the memory allocated by get_line
 * @param rip
 * @param n
 */
void exterminate_malloc(char** rip, int n);