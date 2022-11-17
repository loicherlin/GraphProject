#include <stdio.h>
#include <stdlib.h>
#include "../include/array_list.h"
#include "../include/csv_parser.h"
#include "../include/bin_builder.h"
#include "../include/visualise.h"

int main(int argc, char* argv[]){
    FILE* fp = fopen("../les-arbres.csv", "r");
    if(fp == NULL){ printf("file couldn't be read.\n"); exit(1); }
    
    build_bin(fp, "coor.bin");

    FILE* fp_bin = fopen("coor.bin", "r");
    if(fp_bin == NULL){ printf("file couldn't be read.\n"); exit(1); }

    list_t* data_list = get_data_bin(fp_bin);
    show_data(1300, 900, data_list);
    fclose(fp_bin);
    return 0;
}