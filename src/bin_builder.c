#include <stdio.h>
#include <stdlib.h>
#include "../include/bin_builder.h"
#include "../include/csv_parser.h"
#include "../include/serializer.h"
#include <string.h>

int write_to_bin(char** contents, FILE* fp_bin, int n){
    char* endPtr;
    double x = strtod(contents[n-1], &endPtr);
    //Remove ","" because the data is in form "x,y"
    memmove(&endPtr[0], &endPtr[0 + 1], strlen(endPtr) - 0);
    double y = strtod(endPtr, NULL);

    char* data = serialize_data_t(x,y);
    fwrite(data, sizeof(data_t), 1, fp_bin);
    free(data);
    return EXIT_SUCCESS;
}

int build_bin(FILE* fp, char* path_bin){
    FILE* fp_bin = fopen(path_bin, "w+b");
    if(fp_bin == NULL){perror("failed to fopen path_bin"); return EXIT_FAILURE; }

    // get the number of column by counting 
    // how much ; there are in the header
    int n = size_column(fp, ';');
    // skip header (first line)
    skip_header(fp);
    char** contents;  
    while((contents = get_line(fp, n)) != NULL){ 
        print_line(contents, n);
        if(write_to_bin(contents, fp_bin, n) == EXIT_FAILURE) { 
            exterminate_malloc(contents, n); 
            return EXIT_FAILURE;
        }
        exterminate_malloc(contents, n);
    }
    fclose(fp_bin);
    return EXIT_SUCCESS;
}

int read_bin(FILE* fp){
    char data[sizeof(data_t)];
    while(fread(data, sizeof(data_t), 1, fp)){
        data_t d = deserialize_data_t(data);
        printf("%f\n%f\n\n", d.x, d.y);
    }
    return EXIT_SUCCESS;
}