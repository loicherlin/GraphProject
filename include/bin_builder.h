#pragma once
#include "./array_list.h"


/*
    Sanatize coordinates that are in form of "lattitude,longitude"
*/
void sanatize_coordinates(float* lattitude, float* longitude, char* coordinates);

/* 
    Write serialized data to fp.
    Return 1 if done it successfuly, otherwise 0.
    fp is not closed into this function.
*/
int write_to_bin(char** str, FILE* fp, int n);

/* 
    Read the content of fp, serialized and write it
    into path_bin. 
    Return 1 if done it succesfuly, otherwise 0.
    fp is not closed into this function. 
*/
int build_bin(FILE* fp, char* path_bin);

/* 
    Read and deserialize data from fp 
    and return a list of theses data. 
*/
list_t* get_data_bin(FILE* fp);