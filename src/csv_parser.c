#include "../include/csv_parser.h"
#include "../include/cprintf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int size_column(FILE *fp, char delimiter)
{
    fseek(fp, 0, SEEK_SET);
    int c = EOF;
    int counter = 0;
    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '\n')
            break;
        else if (c == delimiter)
            counter++;
    }
    // Important
    if ((counter == 0 && feof(fp)))
        return 0;
    return counter + 1;
}

char **get_line(FILE *fp, int n, char delimiter)
{
    char *buf;
    CHK_ALLOC((buf = malloc(sizeof(char) * 1000)), "malloc failed");
    if (fgets(buf, 1000, fp) == NULL)
    {
        free(buf);
        return NULL;
    }
    char delim[2];
    delim[0] = delimiter;
    delim[1] = '\0';
    char **line_splitted;
    CHK_ALLOC((line_splitted = split_line(buf, delim, n)), "split_line failed");
    free(buf);
    return line_splitted;
}

void print_line(char **line_splitted, int n)
{
    for (int i = 0; i < n; i++)
        printf("%s ", line_splitted[i]);
    printf("\n");
}

char **split_line(char *line, char *delimiter, int n)
{
    char **spl;
    CHK_ALLOC((spl = malloc(sizeof(char *) * (n))), "malloc failed");
    char *str;
    for (int i = 0; i < n; i++)
    {
        str = strsep(&line, delimiter);
        if (str == NULL && i < n)
        {
            eprintf("strsep failed in split_line.\n", __FILE__, __LINE__);
            exit(EXIT_FAILURE);
        }
        // if it finds a string with no data
        // put NODATA instead
        if (!strcmp(str, ""))
        {
            CHK_ALLOC((spl[i] = strdup("NODATA")), "strdup failed");
        }
        else
        {
            CHK_ALLOC((spl[i] = strdup(str)), "strdup failed");
        }
    }
    return spl;
}

void skip_header(FILE *fp)
{
    fseek(fp, 0, SEEK_SET);
    int c = EOF;
    while ((c = fgetc(fp)) != EOF)
        if (c == '\n')
            break;
}

void exterminate_malloc(char **rip, int n)
{
    for (int i = 0; i < n; i++)
        free(rip[i]);
    free(rip);
}