#include <stdio.h>
#include <stdlib.h>
#include "../include/csv_parser.h"
#include "../include/bin_builder.h"

int main(int argc, char* argv[]){
    FILE* fp = fopen("petitbou", "r");
    if(fp == NULL){ printf("file couldn't be read.\n"); exit(1); }
    
    build_bin(fp, "coor.bin");

    FILE* fp_bin = fopen("coor.bin", "r");
    if(fp_bin == NULL){ printf("file couldn't be read.\n"); exit(1); }
    read_bin(fp_bin);
    fclose(fp_bin);
    return 0;
}