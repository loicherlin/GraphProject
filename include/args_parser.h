#pragma once
#include "../include/cprintf.h"
#include <argp.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @defgroup ParsingModule
 * @{
 * @brief Parse argments from command line using argp
 * @note Reference:
 * https://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html
 */

const char *argp_program_version = "1.0";

/* Program documentation. */
static char doc[] = "Give the Minimum Spanning Tree based on input file (normaly Tree's of "
                    "Paris)\nusing Delaunay Triangulation and Prim algorithm.";

/* The options we understand. */
static struct argp_option options[] = {
    {"debug", 'g', 0, 0, "Produce debugging output", 0},
    {"input", 'i', "CSV_FILE", 0, "", 0},
    {"output", 'o', "BINARY_FILE", 0, "", 0},
    {"delimiter", 'd', "DELIMITER", 0, "Delimiter for parsing input file. Default is ';'.", 0},
    {"load delaunay", 'l', "BINARY_FILE", 0, "Load delaunay triangles from binary file.", 0},
    {"save delaunay", 's', "PATH", 0, "Save delaunay triangles to binary file.", 0},
    {"save mst", 'm', "PATH", 0, "Save mst to plain txt file with \"org - dest\" convention.", 0},
    {"visualise", 'v', 0, 0, "Visualise the result.", 0},
    {"height", 'h', "HEIGHT", 0, "Height of the visualisation window. (default 900)", 0},
    {"width", 'w', "WIDTH", 0, "Width of the visualisation window. (default 1400)", 0},
    {0}};

/* Used by main to communicate with parse_opt. */
typedef struct arguments
{
    // char *args[2];                /* arg1 & arg2 */
    char *output_file;
    char *input_file;
    char *delimiter;
    char *load_delaunay;
    char *save_delaunay;
    char *save_mst;
    bool debug;
    bool visualise;
    int height;
    int width;
} arguments_t;

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    /* Get the input argument from argp_parse, which we
       know is a pointer to our arguments structure. */
    arguments_t *arguments = state->input;
    switch (key)
    {
    case ARGP_KEY_END:
        // weird
        if (state->argc != 5)
        {
            // ARGP_HELP_BUG_ADDR
            // argp_usage(state);
        }
        break;
    case 'i':
        arguments->input_file = arg;
        break;
    case 'o':
        arguments->output_file = arg;
        break;
    case 'd':
        arguments->delimiter = arg;
        break;
    case 'l':
        arguments->load_delaunay = arg;
        break;
    case 's':
        arguments->save_delaunay = arg;
        break;
    case 'g':
        arguments->debug = true;
        break;
    case 'v':
        arguments->visualise = true;
        break;
    case 'h':
        arguments->height = atoi(arg);
        break;
    case 'w':
        arguments->width = atoi(arg);
        break;
    case 'm':
        arguments->save_mst = arg;
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

/* Our argp parser. */
static struct argp argp = {options, parse_opt, NULL, doc, NULL, NULL, NULL};
/** @} */
