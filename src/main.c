#include <stdio.h>
#include <stdlib.h>
#include "../include/args_parser.h"
#include "../include/array_list.h"
#include "../include/csv_parser.h"
#include "../include/bin_builder.h"
#include "../include/visualise.h"
#include "../include/prim.h"

void free_list(list_t* data_list){
    for(size_t i = 0; i < list_size(data_list); i++)
        free(list_get(data_list, i));
    list_free(data_list);
}

int main(int argc, char* argv[]){
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
    //show_data(1300, 900, data_list);
    //graph_t* g = create_graph(2);
    data_t* d0 = (data_t*)list_get(data_list, 0);
    data_t* d1 = (data_t*)list_get(data_list, 1);
    data_t* d2 = (data_t*)list_get(data_list, 2);
    data_t* d3 = (data_t*)list_get(data_list, 3);
    data_t* d4 = (data_t*)list_get(data_list, 4);
    data_t* d5 = (data_t*)list_get(data_list, 5);

    graph_t* g = create_graph(4);
    add_edge(g, d0, d1);
    add_edge(g, d0, d2);
    add_edge(g, d0, d3);
    add_edge(g, d1, d3);
    add_edge(g, d2, d3);
    
    show_graph_ajd(g);
    free_graph(g);
    // Free content of the list and the list itself
    free_list(data_list);
    fclose(fp_bin);
    return 0;
}