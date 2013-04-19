#include "global.h"

Boolean debug = FALSE;

void usage(void)
{
    fprintf (stderr,"usage: region-adj [-d] map_file\n");
}

void scanargs(char* s)
{
    /* check each character of the option list for
       its meaning. */
    while (*++s != '\0')
        switch (*s)
            {

            case 'd': /* debug option */
                debug = TRUE;
                if (debug)
                    {
                        debug_file = fopen("DEBUG_LOG", "w");
                        if (debug_file == NULL)
                            {
                                fprintf(stderr, "Cannot open DEBUG_LOG\n");
                                debug = FALSE;
                            }
                    }
                break;

            default:
                fprintf (stderr,"caches: Bad option %c\n", *s);
                usage();
                exit(1);
            }
}

int main (int argc, char * argv[] ) {

    /* Process all arguments. */
    /* skip program name */
    argc--, argv++;
    while ((argc > 1) && (**argv == '-'))
        {
            scanargs(*argv);
            argc--, argv++;
        }

    if (argc != 1)
        {
            usage();
            exit(-1);
        }


return(0);

}
