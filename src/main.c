#include <stdio.h>
#include <stdlib.h>
#include "../include/args_parser.h"
#include "../include/array_list.h"
#include "../include/csv_parser.h"
#include "../include/bin_builder.h"
#include "../include/visualise.h"
#include "../include/prim.h"
#include "../include/delaunay.h"
#include <tps.h>



void free_list_n(list_t* data_list){
    size_t longueur = list_size(data_list);
    for(size_t i = 0; i < longueur; i++){
        data_t* a = list_take(data_list,list_size(data_list)-1);
        free(a);
    }
    list_free(data_list);
}


void free_list_t(triangle_t** triangles, size_t size){
    free(triangles[0][0].s1);
    for(int i = 0; i < size; i++){
        free(triangles[i]);
    }
    free(triangles);
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
    // Apply Delaunay algorithm
    triangle_t** delaunay = delaunay_bowyer_watson(data_list);
    // Apply Prim's algorithm
    graph_t* g = create_graph(list_size(data_list));
    convert_to_graph(delaunay, g);
    int* mst = prim_mst(g);
    // Visualize Prim and Delaunay result
    tps_onKeyDown(onKeyDown);
    visualize(1400, 900, data_list, mst, delaunay, g);
    // Free memory
    free_graph(g);
    free(mst);
    free_list_t(delaunay, delaunay[0][0].s1->latitude);
    free_list_n(data_list);
    fclose(fp_bin);
    return 0;
}