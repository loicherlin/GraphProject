#include <stdio.h>
#include <stdlib.h>
#include "../include/args_parser.h"
#include "../include/array_list.h"
#include "../include/csv_parser.h"
#include "../include/bin_builder.h"
#include "../include/visualise.h"
#include "../include/delauney.h"


void free_list_n(list_t* data_list){
    size_t longueur = list_size(data_list);
    for(size_t i = 0; i < longueur; i++){
        node_t* a = list_take(data_list,list_size(data_list)-1);
        free(a);
    }
    list_free(data_list);
}


void free_list_t(list_t* data_list){
    size_t longueur = list_size(data_list);
    for(size_t i = 0; i < longueur; i++){
        triangle* a = list_take(data_list,list_size(data_list)-1);
        free(a);
    }
    list_free(data_list);
}

int main(int argc, char* argv[]){
    /*
    // Initiate arguments for args parsing
    struct arguments arguments;
    arguments.output_file = "-";
    arguments.input_file = "-";
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    // Open CSV file
    FILE* fp = open_file(arguments.input_file);
    // Build bin file based on fp
    int result = build_bin(fp, arguments.output_file);
    if(result == EXIT_FAILURE){ fclose(fp); exit(1); }
    // Open bin file to read it
    FILE* fp_bin = open_file(arguments.output_file);
    list_t* data_list = get_data_bin(fp_bin);
    // Show a visualization of data from the binary file
    show_data(1300, 900, data_list);
    // Free content of the list and the list itself
    free_list(data_list);
    fclose(fp_bin);
    return 0;*/
    FILE* fp = fopen("les-arbres.csv", "r");
    if(fp == NULL){ printf("file couldn't be read.\n"); exit(1); }
    
    if(build_bin(fp, "coor.bin") == EXIT_FAILURE){ exit(1); }

    FILE* fp_bin = fopen("coor.bin", "r");
    if(fp_bin == NULL){ printf("file couldn't be read.\n"); exit(1); }

    list_t* data_list = get_data_bin(fp_bin);
    triangle** d = delaunay_bowyer_watson(data_list);
    /*
    for(size_t i = 0; i < list_size(d); i++){
        triangle* t = (triangle*)list_get(d, i);
        printf("triange %ld:\n\t[%f.%f,\n\t%f.%f,\n\t%f.%f]\n", i,t->s1->latitude, t->s1->longitude, t->s2->latitude, t->s2->longitude, t->s3->latitude, t->s3->longitude);
    }*/
    printf("ok in Main\n");
    printf("size : %f\n",d[0][0].s1->latitude);
    printf("ok in Main2\n");
    show_data(1300, 900, d);
    for(size_t i = 1; i < d[0][0].s1->latitude; i++){
        if(d[i] != NULL){
            triangle* t = d[i];
            printf("triange %ld:\n\t[%f.%f,\n\t%f.%f,\n\t%f.%f]\n", i,t->s1->latitude, t->s1->longitude, t->s2->latitude, t->s2->longitude, t->s3->latitude, t->s3->longitude);
        }
    }
    free(d);
    free_list_n(data_list);
    fclose(fp_bin);
    return 0;
}