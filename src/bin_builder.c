#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tps.h>
#include "../include/cprintf.h"
#include "../include/bin_builder.h"
#include "../include/csv_parser.h"
#include "../include/data_t.h"
#include "../include/delaunay.h"
#include "../include/array_list.h"

void sanatize_coordinates(double* lattitude, double* longitude, char* coordinates){
    char* endPtr;
    char* checkPtr;
    *lattitude = strtod(coordinates, &endPtr);
    // Remove ","" because the data is in form "x,y"
    memmove(&endPtr[0], &endPtr[0 + 1], strlen(endPtr) - 0);
    *longitude = strtod(endPtr, &checkPtr);
    // check if the conversion was successful
    if(strcmp(checkPtr, endPtr) == 0){
        fprintf(stderr, "Failed to convert coordinates to double (coordinate must be the last element of the line\n in form of \"x,y\" !)\n");
        exit(EXIT_FAILURE);
    }
}

int write_to_csv_bin(char** contents, FILE* fp_bin, int n){
    double lattitude, longitude;
    static int i = 0;
    sanatize_coordinates(&lattitude, &longitude, contents[n-1]);
    char* data = serialize_data_t(lattitude,longitude, i);
    if(fwrite(data, sizeof(data_t), 1, fp_bin) != 1){
        perror("Failed to write to file\n");
        return EXIT_FAILURE;
    }
    free(data);
    i++;
    return EXIT_SUCCESS;
}

bool is_empty_line(FILE* fp){
    char c = fgetc(fp);
    // end of file
    if(feof(fp)){
         return true; 
    }
    // empty line
    if(c == '\t' || c == ' ' || c == '\n'){
        return true;
    }
    // not empty line, go back to previous position
    else{
        fseek(fp, -1, SEEK_CUR);
        //deprintf("is_empty_line character (ascii): %d\n", c);
        return false;
    }
}

int build_csv_bin(FILE* fp, char* path_bin, char delimiter){
    FILE* fp_bin = fopen(path_bin, "w+b");
    if(fp_bin == NULL){ perror("failed to fopen to path_bin\n"); return EXIT_FAILURE; }
    // Get the number of column by counting how much delimter there are in the header
    int n = size_column(fp, delimiter);
    if(n == 0){
        fprintf(stderr, "column of file is empty, delimiter problem maybe ?\n"); 
        return EXIT_FAILURE; 
    }
    // Skip header (first line)
    skip_header(fp);
    // check if we are at the end of the file
    if(feof(fp)){
        fprintf(stderr, "Error: end of file, your file is maybe empty.\n");
        return EXIT_FAILURE;
    }
    // check if next line is empty
    if(is_empty_line(fp)){
        fprintf(stderr, "Error: line is empty\n");
        return EXIT_FAILURE;
    }
    char** contents;  
    while((contents = get_line(fp, n, delimiter)) != NULL){ 
        if(write_to_csv_bin(contents, fp_bin, n) == EXIT_FAILURE) { 
            fprintf(stderr,"Failed to write to file\n");
            fclose(fp_bin);
            exterminate_malloc(contents, n); 
            return EXIT_FAILURE;
        }
        exterminate_malloc(contents, n);
    }
    fclose(fp_bin);
    return EXIT_SUCCESS;
}

list_t* get_data_csv_bin(FILE* fp){
    char data[sizeof(data_t)];
    list_t* data_list = list_create();
    while(fread(data, sizeof(data_t), 1, fp)){
        data_t* d = deserialize_data_t(data);
        list_append(data_list, d);
    }
    return data_list;
}

//