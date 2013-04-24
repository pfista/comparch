#include "global.h"
#include "region_adj.h"
#include "stack.h"


int skip_blanks(FILE* file);
int skip_line(FILE* file);

/* Read all map data into memory / structures */
region* read_map_data(char* file_name);
