
/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

#include "global.h"

String remember_string(const String name);

/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

int which_power(int n);
void init_lut(void);
unsigned int mask_of(int n);


Boolean ishex(int c);
int hexvalue(int c);
int decvalue(int c);

int skip_line(FILE *file);
int skip_blanks(FILE *file);

int binary_search(sorted_cache_set* array, int low, int high, int target_tag);
void quicksort(sorted_cache_set* array , int low, int high);
