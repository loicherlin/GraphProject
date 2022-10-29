#pragma once

/* little wrapper of fopen */
FILE* open_file(char* path);

/*Get the line splitted into an array of strings (array 2D)*/
char** get_line(FILE* fp, int n);

/* print the contnet of a line that was been sliced */
void print_line(char** line_splitted, int n);

/* get the number of column by counting how many delimiter have the header */
int size_column(FILE* fp, char delimiter);

/* split the line into an array */
char** split_line(char* line, char* delimiter, int n);

/* Skip the first line of the file. (CSV header) */
void skip_header(FILE* fp);

/* free... */
void exterminate_malloc(char** rip, int n);