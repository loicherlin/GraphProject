#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/csv_parser.h"

FILE* open_file(char* path){
    FILE* fp = fopen(path, "r");
    if(fp == NULL){ printf("file couldn't be read.\n"); exit(1); }
    return fp;
}

int size_column(FILE* fp, char delimiter){
    fseek(fp, 0, SEEK_SET);
    int c = EOF;
    int counter = 0;
    while ((c=fgetc(fp)) != EOF) {
        if (c=='\n'){
            break;
        }
        else if (c == delimiter){
            counter++;
        }
    }
    // Important
    if(counter == 0){ return 0; }
    return counter + 1;
}


char** get_line(FILE* fp, int n){
    char* buf = malloc(sizeof(char) * 1000);
    if(fgets(buf, 1000, fp) == NULL){ 
        free(buf); return NULL; 
    }
    char** line_splitted = split_line(buf, ";", n);
    if(line_splitted == NULL){ free(buf); printf("malloc failed in access_content.\n"); exit(1); }

    free(buf);
    return line_splitted;
}

void print_line(char** line_splitted, int n){

    for(int i = 0; i < n; i++){
        printf("%s ", line_splitted[i]);
    }
    printf("\n");
}

char** split_line(char* line, char* delimiter, int n){
    char** spl = (char**)malloc(sizeof(char*) * (n));
    if(spl  == NULL){ printf("malloc failed in split_line.\n"); exit(1); }
    char* str;
    for(int i = 0; i < n; i++){
        str = strsep(&line, delimiter);
        // if it finds a string with no data
        // put NODATA instead
        if(!strcmp(str, "")){
            spl[i] = strdup("NODATA");
        } else {
            spl[i] = strdup(str);
        }
    }
    return spl;
}


void skip_header(FILE* fp){
    fseek(fp, 0, SEEK_SET);
    int c = EOF;
    while ((c=fgetc(fp)) != EOF) {
        if (c=='\n')
            break;
    }
}

void exterminate_malloc(char** rip, int n){
    for(int i = 0; i < n; i++){
        free(rip[i]);
    }
    free(rip);
}