#include "process_file.h"

int skip_blanks(FILE *file)
{
    int c;
    while (((c = getc(file)) != EOF) && isspace(c))
        if (c == '\n')
            line_number++;
    return(c);
}

int read_until(FILE* file, char** buffer_ptr, int size, char end_delim) {
    int i = 0;
    int c = getc(file);
    while (c != EOF && c != end_delim) {
        if (!check_parens(c))
            exit(EXIT_FAILURE);
        if (i >= size-1) { // -1 to save space for '\0'
            char* temp_buff = realloc (*buffer_ptr, sizeof(char)*size*2);
            if (temp_buff == NULL) {
                fprintf(stderr, "Error reallocing memory for buffer\n");
                exit(EXIT_FAILURE);
            }
            else {
                *buffer_ptr = temp_buff; 
                size *= 2;
            }
        }
        
        (*buffer_ptr)[i] = c;
        i++;

        c = getc(file);
    }
    if (!check_parens(c))
        exit(EXIT_FAILURE);
    (*buffer_ptr)[i] = '\0';
    return i+1;
}

Boolean check_parens(char c) {
    switch (c) {
        case '}':
            if (pop(parens) != '{') {
                fprintf(stderr, "Poorly nested parenthesis at line %d\n", line_number);
                return FALSE;
            }
            else return TRUE;
        break;

        case ')':
            if (pop(parens) != '(') {
                fprintf(stderr, "Poorly nested parenthesis at line %d\n", line_number);
                return FALSE;
            }
            else return TRUE;
        break;

        case ']':
            if (pop(parens) != '[') {
                fprintf(stderr, "Poorly nested parenthesis at line %d\n", line_number);
                return FALSE;
            }
            else return TRUE;
        break;

        case '\n':
            line_number++;

        default: 
            return TRUE;
    }
}

region* read_map_data (char* file_name)
{
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "Error reading file: %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    /* init global vars */
    parens = malloc(sizeof(Stack));
    init (parens);
    line_number = 1;
    

    int c;
    c = skip_blanks(file);
    ungetc(c, file);
    if (c == EOF) return NULL;
    

    // Initialize the array of regions
    region* regions = malloc(sizeof(region)*DEFAULT_REGION_SIZE);
    int region_size = DEFAULT_REGION_SIZE;
    int region_index = 0;



    while ((c = getc(file)) != EOF) {
        if (c == '{') { // Create new region
            push(parens, '{'); // add to stack

            c = skip_blanks(file);
            ungetc(c, file);
            char* buffer = malloc(sizeof(char)*DEFAULT_BUFFER_SIZE);
            if (buffer == NULL) {
                fprintf(stderr, "Error mallocing buffer\n");
                exit(EXIT_FAILURE);
            }

            int name_size = read_until(file, &buffer, DEFAULT_BUFFER_SIZE, ',');

            /* Make sure buffer can hold info */
            if (region_index >= region_size) { // Realloc if necessary
                region* temp_regions = realloc(regions, sizeof(region)*region_size*2);
                if (temp_regions == NULL) {
                    fprintf(stderr, "Problem reallocing regions buffer\n");
                    exit(EXIT_FAILURE);
                }
                regions = temp_regions;
                region_size *= 2;
            }

            // Store the name in memory, free the buffer
            regions[region_index].name = malloc(sizeof(char)*(name_size));
            if (regions[region_index].name == NULL) {
                fprintf(stderr, "Error mallocing name in region\n");
                exit(EXIT_FAILURE);
            }

            strcpy(regions[region_index].name, buffer);
            free (buffer);

            c = skip_blanks(file);
            if (!check_parens(c))
                exit(EXIT_FAILURE);

            regions[region_index].polygons = malloc(sizeof(polygon)*DEFAULT_POLYGON_SIZE);
            int polygon_size = DEFAULT_POLYGON_SIZE; // Keeps track of buffer size for polgyons
            int polygon_index = 0;

            // Now read all the polygons
            while (c != '}') {
                if (c == '[') {
                    push(parens, '[');
                    // Create new polygon to go inside of region
                    
                    int num_verts = 0;

                    c = skip_blanks(file);

                    if (polygon_index >= polygon_size) {
                        // Buffer is out of space, reallocate
                        polygon* temp_polygons = realloc(regions[region_index].polygons,
                                sizeof(polygon)*polygon_size*2);

                        if (temp_polygons == NULL){
                            fprintf(stderr, "Problem reallocing polygon buffer\n");
                            exit(EXIT_FAILURE);
                        }
                        else {
                            regions[region_index].polygons = temp_polygons;
                            polygon_size *= 2;
                        }
                    }

                    regions[region_index].polygons[polygon_index].vertices = 
                        malloc(sizeof(point)*DEFAULT_NUM_POINTS);

                    int vertices_size = DEFAULT_NUM_POINTS;
                    int vertices_index = 0;

                    while (c != ']') {
                        if (c == '(') {
                            push(parens, '(');
                            // Create new point to go inside polygon
                            // read until ')'
                            buffer = malloc(sizeof(char)*DEFAULT_BUFFER_SIZE);
                            if (buffer == NULL) {
                                fprintf(stderr, "ERROR MALLOCING\n");
                                exit(EXIT_FAILURE);
                            }
                            read_until(file, &buffer, DEFAULT_BUFFER_SIZE, ')');
                            num_verts++;

                            // reached end of point, save it to the polygon
                            if (vertices_index >= vertices_size) { //Buffer is out of space, reallocate
                                point* temp_vertices = realloc(regions[region_index].
                                        polygons[polygon_index].vertices, sizeof(point)*vertices_size*2);

                                if (temp_vertices == NULL) {
                                    fprintf(stderr, "Problem reallocing vertices buffer\n");
                                    exit(EXIT_FAILURE);
                                }
                                else {
                                    regions[region_index].polygons[polygon_index].vertices = temp_vertices;
                                    vertices_size *= 2;
                                }
                            }

                            // Parse point of format "1.0,1.0" into x and y floats
                            char* pCh = strtok(buffer, ",");
                            char* pEnd;
                            double x = strtod(pCh, &pEnd);
                            pCh = strtok(NULL, ",");
                            double y = strtod(pCh, &pEnd);

                            if (debug) fprintf(debug_file, 
                                    "Storing point (%.2f,%.2f) in r_%d, p_%d v_%d\n",
                                    x,y,region_index,polygon_index,vertices_index);
                            regions[region_index].polygons[polygon_index].vertices[vertices_index].x = x;
                            regions[region_index].polygons[polygon_index].vertices[vertices_index].y = y;
                            free (buffer);
                            vertices_index++;
                        } // end if (
                        c = getc(file);
                            if (!check_parens(c))
                                exit(EXIT_FAILURE);
                    } // end while != ]
                    //reached end of polygon, save it now to region
                    regions[region_index].polygons[polygon_index].num_vertices = num_verts;
                    polygon_index++;
                }// end if [

                c = getc(file);
                if (!check_parens(c))
                    exit(EXIT_FAILURE);
            } // end while != 
            //reached end of region, save it
            regions[region_index].num_polygons = polygon_index;
            region_index++;
        } // end if
        if (c == '\n')
            line_number++;
    }// end while

    return regions;
}
