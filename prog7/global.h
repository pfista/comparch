
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

/* The global declarations that every C file should have */
#define _GNU_SOURCE  /* for strcasestr */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef short Boolean;
#define TRUE 1
#define FALSE 0

FILE *debug_file;

#endif
