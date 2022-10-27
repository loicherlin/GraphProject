#include <stdio.h>
#include <stdlib.h>
#include "../include/CSV_parser.h"
#include "../include/BIN_builder.h"

int main(int argc, char* argv[]){
    FILE* fp = fopen("petitbou", "r");
    if(fp == NULL){ printf("file couldn't be read.\n"); exit(1); }
    build_bin(fp);
    return 0;
}