#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include "../include/args_parser.h"
#include "../include/array_list.h"
#include "../include/csv_parser.h"
#include "../include/bin_builder.h"
#include "../include/visualise.h"


int main(int argc, char* argv[]){
    struct arguments arguments;
    arguments.output_file = "-";
    arguments.input_file = "-";
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    FILE* fp = fopen(arguments.input_file , "r");
    if(fp == NULL){ printf("file couldn't be read.\n"); exit(1); }
    
    if(build_bin(fp, arguments.output_file) == EXIT_FAILURE){ exit(1); }

    FILE* fp_bin = fopen(arguments.output_file, "r");
    if(fp_bin == NULL){ printf("file couldn't be read.\n"); exit(1); }

    list_t* data_list = get_data_bin(fp_bin);
    show_data(1300, 900, data_list);
    for(size_t i = 0; i < list_size(data_list); i++)
        free(list_get(data_list, i));
    list_free(data_list);
    fclose(fp_bin);
    return 0;
}