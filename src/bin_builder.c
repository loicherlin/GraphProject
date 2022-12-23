#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tps.h>
#include "../include/bin_builder.h"
#include "../include/csv_parser.h"
#include "../include/data_t.h"
#include "../include/array_list.h"

void sanatize_coordinates(double* lattitude, double* longitude, char* coordinates){
    char* endPtr;
    *lattitude = strtod(coordinates, &endPtr);
    // Remove ","" because the data is in form "x,y"
    // Needs to be done in a better way I think, maybe later ..
    memmove(&endPtr[0], &endPtr[0 + 1], strlen(endPtr) - 0);
    *longitude = strtod(endPtr, NULL);
}

int write_to_bin(char** contents, FILE* fp_bin, int n){
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

int build_bin(FILE* fp, char* path_bin, char delimiter){
    FILE* fp_bin = fopen(path_bin, "w+b");
    if(fp_bin == NULL){ perror("failed to fopen to path_bin\n"); return EXIT_FAILURE; }
    // Get the number of column by counting how much ; there are in the header
    int n = size_column(fp, delimiter);
    if(n == 0){ fprintf(stderr, "column of file is empty\n"); return EXIT_FAILURE; }
    // Skip header (first line)
    skip_header(fp);
    char** contents;  
    while((contents = get_line(fp, n, delimiter)) != NULL){ 
        if(write_to_bin(contents, fp_bin, n) == EXIT_FAILURE) { 
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

list_t* get_data_bin(FILE* fp){
    char data[sizeof(data_t)];
    list_t* data_list = list_create();
    while(fread(data, sizeof(data_t), 1, fp)){
        data_t* d = deserialize_data_t(data);
        list_append(data_list, d);
    }
    return data_list;
}