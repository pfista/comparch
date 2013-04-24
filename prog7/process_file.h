#include "global.h"
#include "region_adj.h"
#include "stack.h"


int skip_blanks(FILE* file);
int skip_line(FILE* file);

/* Reads characters into the buffer until end_delim is found */
int read_until(FILE* file, char** buffer_ptr, int size, char end_delimn);

/* Read all map data into memory / structures */
region* read_map_data(char* file_name);
