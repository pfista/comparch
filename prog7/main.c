#include "global.h"
#include "process_file.h"

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
                        debug_file = fopen("debug.log", "w");
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

    if (debug) fprintf(debug_file, "Reading map data from file: %s\n", argv[0]);
    region* regions = read_map_data(argv[0]);

    int i = 0;
    while (regions[i].polygons != NULL) {
        if (debug) fprintf(stdout, "%s\n", regions[i].name);
            int j = 0;
            for (j = 0; j < regions[i].num_polygons; j++) {
                fprintf(stdout, "\tpoly_%d\n", j);
                int h = 0;
                for (h = 0; h < regions[i].polygons[j].num_vertices; h++) {
                    fprintf(stdout, "\t\tpoint_%d: %.2f,%.2f\n", h, regions[i].polygons[j].vertices[h].x,
                            regions[i].polygons[j].vertices[h].y);
                }
            }
            i++;
    }


    return(0);

}
