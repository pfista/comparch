#include "global.h"
#include "region_adj.h"
#include "stack.h"


int skip_blanks(FILE* file);

Stack* parens;
int line_number;

/* Reads characters into the buffer until end_delim is found */
int read_until(FILE* file, char** buffer_ptr, int size, char end_delimn);
Boolean check_parens(char c);

/* Read all map data into memory / structures */
region* read_map_data(char* file_name);
