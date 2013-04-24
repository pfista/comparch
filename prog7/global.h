
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

/* The global declarations that every C file should have */
#define _GNU_SOURCE  /* for strcasestr */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef short Boolean;
FILE *debug_file;
Boolean debug;

#define TRUE 1
#define FALSE 0

#define DEFAULT_REGION_SIZE 4
#define DEFAULT_BUFFER_SIZE 50
#define DEFAULT_POLYGON_SIZE 3
#define DEFAULT_NUM_POINTS 10


#endif
