#pragma once
#include <argp.h>

//https://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html

const char *argp_program_version =
  "MSTrees Paris 1.0";

/* Program documentation. */
static char doc[] =
  "Give the Minimum Spanning Tree based on input file (normaly Tree's of Paris)\n\nThe programs firstly parse the input file and store in binary format into output file. And then starts to work on this binary file to create the MST based of this binary file.";


/* The options we understand. */
static struct argp_option options[] = {
  {"input", 'i', "CSV_FILE", 0, "" },
  {"output", 'o', "BINARY_FILE", 0, ""},
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
  //char *args[2];                /* arg1 & arg2 */
  char *output_file;
  char* input_file;
};

/* Parse a single option. */
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;
  switch (key)
    {
    case  ARGP_KEY_END:
        // weird 
        if(state->argc != 5){
            // ARGP_HELP_BUG_ADDR
            argp_usage(state);
        }
        break;
    case 'i':
      arguments->input_file = arg;
      break;
    case 'o':
      arguments->output_file = arg;
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }

  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, NULL, doc };