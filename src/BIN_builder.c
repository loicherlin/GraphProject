#include <stdio.h>
#include <stdlib.h>
#include "../include/BIN_builder.h"
#include "../include/CSV_parser.h"

void build_bin(FILE* fp){
    // get the number of column by counting 
    // how much ; there are in the header
    int n = size_column(fp, ';');
    // skip header (first line)
    skip_header(fp);
    char** contents;  
    while((contents = access_content(fp, n)) != NULL){ 
        print_line(contents, n);
        exterminate_malloc(contents, n);
    }
}