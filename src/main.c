#include <stdio.h>
#include <stdlib.h>
#include <tps.h>
#include "../include/args_parser.h"
#include "../include/array_list.h"
#include "../include/csv_parser.h"
#include "../include/bin_builder.h"
#include "../include/visualise.h"
#include "../include/graph.h"
#include "../include/delaunay.h"
#include "../include/cprintf.h"
#include "../include/handler.h"

// Initiate arguments for args parsing
struct arguments arguments;

void free_list_n(list_t* data_list){
    size_t longueur = list_size(data_list);
    for(size_t i = 0; i < longueur; i++){
        data_t* a = list_take(data_list,list_size(data_list)-1);
        free(a);
    }
    list_free(data_list);
}


list_t* initiate_data_list(){
    // Open CSV file
    FILE* fp = open_file(arguments.input_file);
    // Build bin file based on fp
    int result = build_bin(fp, arguments.output_file, arguments.delimiter[0]);
    if(result == EXIT_FAILURE){ fclose(fp); exit(1); }
    // Open bin file to read it
    FILE* fp_bin = open_file(arguments.output_file);
    list_t* data_list = get_data_bin(fp_bin);
    fclose(fp_bin);
    fclose(fp);
    return data_list;
}

void initiate_args(int argc, char* argv[]){
    arguments.output_file = "";
    arguments.input_file = "";
    arguments.delimiter = ";"; // default delimiter
    arguments.load_delaunay = "";
    arguments.save_delaunay = "";
    arguments.save_mst = "";
    arguments.debug = false;
    arguments.visualise = false;
    arguments.height = 900;
    arguments.width = 1400;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    if(!strcmp(arguments.output_file,"") || !strcmp(arguments.input_file,"")){
        printf("Please give input and output file. (use -h for help)\n");
        exit(1);
    }
    _debug = arguments.debug;
}



int main(int argc, char* argv[]){
    // Initiate handler
    initiate_handler();
    // Initiate arguments
    initiate_args(argc, argv);
    // Initiate data_list
    list_t* data_list = initiate_data_list();
    // Initiate delaunay triangles
    triangle_t** delaunay = initiate_delaunay(data_list, arguments.save_delaunay, arguments.load_delaunay);   
    // Create graph from delaunay triangles
    int nb_vertices = delaunay[0][0].s1->longitude;
    graph_t* g = create_graph(nb_vertices);
    // Convert delaunay triangles to graph
    delaunay_to_graph(delaunay, g);
    // Get prim mst
    int* mst = prim_mst(g, arguments.save_mst);
    // Visualize Prim and Delaunay result
    tps_onKeyDown(onKeyDown);
    // If visualise is true, visualize the result
    arguments.visualise ? visualize(arguments.width, arguments.height, data_list, mst, delaunay, g) : 0;
    // Free memory
    free_graph(g);
    free(mst);
    free_list_t(delaunay, delaunay[0][0].s1->latitude);
    free_list_n(data_list);
    return 0;
}