#include "process_file.h"

int skip_blanks(FILE *file)
{
    int c;
    while (((c = getc(file)) != EOF) && isspace(c)) /* keep reading */;
    return(c);
}


int skip_line(FILE *file)
{
    int c;
    while (((c = getc(file)) != EOF) && (c != '\n')) /* keep reading */;
    c = skip_blanks(file);
    return(c);
}

void read_map_data (FILE* file)
{
    int c;

    c = skip_blanks(file);
    while (c == '#')
        {
            c = skip_line(file);
        }
    if (c == EOF) return;

    //TODO modify this
    /* Syntax for Cache Descriptions:  { key=value, key=value, ... } */
    /* So, we read a key and a value and define the field of the
       cds defined by the key to have the given value. */

    if (c != '{')
        {
            fprintf(stderr, "Cache description should start with {, not %c\n", c);
            return;
        }

}
