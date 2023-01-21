#include "../include/args_parser.h"
#include "../include/array_list.h"
#include "../include/bin_builder.h"
#include "../include/cprintf.h"
#include "../include/csv_parser.h"
#include "../include/data_t.h"
#include "../include/delaunay.h"
#include "../include/graph.h"
#include "../include/handler.h"
#include "../include/visualise.h"
#include <stdio.h>
#include <stdlib.h>
#include <tps.h>

// Initiate arguments for args parsing
struct arguments arguments;

void free_list_n(list_t *data_list)
{
    size_t longueur = list_size(data_list);
    for (size_t i = 0; i < longueur; i++)
    {
        data_t *a = list_take(data_list, list_size(data_list) - 1);
        free(a);
    }
    list_free(data_list);
}

list_t *initiate_data_list()
{
    // Open CSV file
    FILE *fp;
    CHK_ALLOC(fp = fopen(arguments.input_file, "r"), "fopen failed");
    // Build bin file based on fp
    int result = build_csv_bin(fp, arguments.output_file, arguments.delimiter[0]);
    if (result == EXIT_FAILURE)
    {
        fclose(fp);
        exit(1);
    }
    // Open bin file to read it
    FILE *fp_bin;
    CHK_ALLOC(fp_bin = fopen(arguments.output_file, "rb"), "fopen failed");
    // Get data from bin file
    list_t *data_list = get_data_csv_bin(fp_bin);
    // Close files
    fclose(fp_bin);
    fclose(fp);
    return data_list;
}

void initiate_args(int argc, char *argv[])
{
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
    if (!strcmp(arguments.output_file, "") || !strcmp(arguments.input_file, ""))
    {
        printf("Please give input and output file. (use --help for help)\n");
        exit(1);
    }
    _debug = arguments.debug;
}

int main(int argc, char *argv[])
{
    // Initiate handler
    initiate_handler();
    // Initiate arguments
    initiate_args(argc, argv);
    // Initiate data_list
    list_t *data_list = initiate_data_list();
    // Initiate delaunay triangles
    delaunay_t *delaunay = initiate_delaunay(data_list, arguments.save_delaunay, arguments.load_delaunay);
    // Create graph from delaunay triangles
    set_distn(HAVESINE);
    graph_t *g = create_graph(delaunay->size_vertices);
    // Convert delaunay triangles to graph
    delaunay_to_graph(delaunay, g);
    // Get minimum spanning tree using Prim algorithm
    int *mst = prim_mst(g, arguments.save_mst);
    // If visualise is true, visualize Prim and Delaunay result
    arguments.visualise ? visualize(arguments.width, arguments.height, data_list, mst, delaunay, g) : 0;
    // Free memory
    free_graph(g);
    free(mst);
    free_delaunay(delaunay);
    free_list_n(data_list);
    return 0;
}