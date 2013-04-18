
/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

/* The code to read a memory trace and simulate it on the various
   caches. */

#include "global.h"
#include "utils.h"        // utility functions

static char LogTable256[256];


/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

/* n is a power of two, which one? */
/* 1 -> 0; 2 -> 1; 4->2... */
int which_power(int n)
{
    /* This optimization is from
     * http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogLookup
     * Generate a lookup table once before using 
     */
    unsigned r; // r will be lg(n)
    unsigned int t; // temp

    if ((t = n >> 24)) 
    {
      r = 24 + LogTable256[t];
    } 
    else if ((t = n >> 16)) 
    {
      r = 16 + LogTable256[t];
    } 
    else if ((t = n >> 8)) 
    {
      r = 8 + LogTable256[t];
    } 
    else 
    {
      r = LogTable256[n];
    }

    return r;
}

void init_lut(void) {
    LogTable256[0] = -1;
    LogTable256[1] = 0;

    int i;
    for (i = 2; i < 256; i++) 
    {
      LogTable256[i] = 1 + LogTable256[i / 2];
    }
}



/* generate a mask of n low order bits */
/* if we want a mask of two bits, shift a 1 two
   bits over (100), and then subtract one (011). */
unsigned int mask_of(int n)
{
    int m;
    if (n <= 0) return(0);
    m = (1 << n) - 1;
    return(m);
}


/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

Boolean ishex(int c)
{
    if (('0' <= c) && (c <= '9')) return(TRUE);
    if (('a' <= c) && (c <= 'f')) return(TRUE);
    if (('A' <= c) && (c <= 'F')) return(TRUE);
    return(FALSE);
}

int hexvalue(int c)
{
    if (('0' <= c) && (c <= '9')) return(c - '0');
    if (('a' <= c) && (c <= 'f')) return(c - 'a' + 10);
    if (('A' <= c) && (c <= 'F')) return(c - 'A' + 10);
    return(-1);
}

int decvalue(int c)
{
    if (('0' <= c) && (c <= '9')) return(c - '0');
    return(-1);
}


/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

/* MALLOC space for a string and copy it */

String remember_string(const String name)
{
    size_t n;
    String p;

    if (name == NULL) return(NULL);

    /* get memory to remember file name */
    n = strlen(name) + 1;
    p = CAST(String , malloc(n));
    strcpy(p, name);
    return(p);
}

/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

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

int binary_search(sorted_cache_set* array, int low, int high, int target_tag)
{
    while (low <= high)
    {
        int middle = low + (high - low)/2;
        if (target_tag < *array[middle].tag)
            high = middle-1;
        else if (target_tag > *array[middle].tag)
            low = middle+1;
        else return array[middle].original_index;
    }
    return -1;
}

void quicksort (sorted_cache_set* array, int low, int high) 
{
    int left = low, right = high;
    sorted_cache_set temp;

    int middle = low + (high - low)/2;
    int pivot = *(array[middle].tag); // just use the mid point for the pivot

    while (left <= right)
    {
        /* move high and low to the values that need to be switched */
        while (*array[left].tag < pivot)
            left++;
        while (*array[right].tag > pivot)
            right--;

        if (left <= right)
        {
            /* swap the two values around the pivot */
            temp = array[left];
            array[left] = array[right];
            array[right] = temp;
            left++;
            right--;
        }
    }

    if (low < right)
        quicksort(array, low, right);
    if (left < high)
        quicksort(array, left, high);
}
