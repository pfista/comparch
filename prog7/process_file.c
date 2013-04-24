#include "process_file.h"

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

int read_until(FILE* file, char** buffer_ptr, int size, char end_delim) {
    int i = 0;
    int c = getc(file);
    while (c != end_delim) {
        if (i >= size-1) { // -1 to save space for '\0'
            char* temp_buff = realloc (*buffer_ptr, size*2);
            if (temp_buff == NULL) {
                fprintf(stderr, "Error reallocing memory for buffer\n");
                exit(EXIT_FAILURE);
            }
            else {
                *buffer_ptr = temp_buff; 
                size *= 2;
            }
        }
        
        if (c == '\n') { // ignore line breaks;
            c = getc(file);
            continue;
        }

        (*buffer_ptr)[i] = c;
        i++;

        c = getc(file);
    }
    (*buffer_ptr)[i] = '\0';
    return i+1;
}

region* read_map_data (char* file_name)
{
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "Error reading file: %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    Stack* parens = malloc(sizeof(Stack));
    init (parens);

    int c;

    c = skip_blanks(file);
    ungetc(c, file);
    if (c == EOF) return;
    
    int line_number = 1;

    // Initialize the array of regions
    region* regions = malloc(sizeof(region)*DEFAULT_REGION_SIZE);
    int region_size = DEFAULT_REGION_SIZE;
    int region_index = 0;



    while ((c = getc(file)) != EOF) {
        if (c == '{') {
            fprintf(stdout, "Creating New Region\n");
            // Create new region
            c = skip_blanks(file);
            ungetc(c, file);
            char* buffer = malloc(sizeof(char)*DEFAULT_BUFFER_SIZE);

            int name_size = read_until(file, &buffer, DEFAULT_BUFFER_SIZE, ',');
            if (debug) fprintf(stdout, "region_size: %d\n", region_size);

            if (region_index >= region_size) { // Realloc if necessary
                if (debug) fprintf(stdout, "Region index >= region_size\n", region_size);
                region* temp_regions = realloc(regions, region_size*2);
                if (temp_regions == NULL) {
                    fprintf(stderr, "Problem reallocing regions buffer\n");
                    exit(EXIT_FAILURE);
                }

                if (debug) fprintf(stdout, "Increasing region_size buffer\n");
                regions = temp_regions;
                region_size *= 2;
            }

            // Store the name in memory, free the buffer
            if (debug) fprintf(stdout, "regionIndex:%d",region_index);
            if (debug) fprintf(stdout," Name:_%s_\n",buffer);
            regions[region_index].name = malloc(sizeof(char)*(name_size));
            if (regions[region_index].name == NULL) {
                fprintf(stderr, "Error mallocing name in region\n");
                exit(EXIT_FAILURE);
            }

            strcpy(regions[region_index].name, buffer);
            free (buffer);

            c = skip_blanks(file);

            regions[region_index].polygons = malloc(sizeof(polygon)*DEFAULT_POLYGON_SIZE);
            int polygon_size = DEFAULT_POLYGON_SIZE; // Keeps track of buffer size for polgyons
            int polygon_index = 0;
        

            // Now read all the polygons
            while (c != '}') {

                if (c == '[') {
                    fprintf(stdout, "\tCreating New Polygon\n");
                    // Create new polygon to go inside of region
                    
                    int num_verts = 0;

                    c = skip_blanks(file);

                    if (polygon_index >= polygon_size) {
                        // Buffer is out of space, reallocate
                        polygon* temp_polygons = realloc(regions[region_index].polygons, polygon_size*2);
                        if (temp_polygons == NULL){
                            fprintf(stderr, "Problem reallocing polygon buffer\n");
                            exit(EXIT_FAILURE);
                        }
                        else {
                            regions[region_index].polygons = temp_polygons;
                            polygon_size *= 2;
                        }
                    }

                    regions[region_index].polygons[polygon_index].vertices = malloc(sizeof(point)*DEFAULT_NUM_POINTS);
                    int vertices_size = DEFAULT_NUM_POINTS;
                    int vertices_index = 0;

                    while (c != ']') {
                    

                        if (c == '(') {
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
                                point* temp_vertices = realloc(regions[region_index].polygons[polygon_index].vertices, vertices_size*2);
                                if (temp_vertices == NULL) {
                                    fprintf(stderr, "Problem reallocing vertices buffer\n");
                                    exit(EXIT_FAILURE);
                                }
                                else {
                                    regions[region_index].polygons[polygon_index].vertices = temp_vertices;
                                    vertices_size *= 2;
                                }

                            }
                            float x = 1.1; //scanf("f", buffer);
                            float y = 2.2; //scanf("f", buffer);
                            free (buffer);
                            regions[region_index].polygons[polygon_index].vertices[vertices_index].x = x;
                            regions[region_index].polygons[polygon_index].vertices[vertices_index].y = y;

                        } // end if (

                        vertices_index++;
                        c = getc(file);
                    } // end while != ]
                    //reached end of polygon, save it now to region
                    
                    regions[region_index].polygons[polygon_index].num_vertices = num_verts;

                    polygon_index++;

                }// end if [

                c = getc(file);
            } // end while != 
            //reached end of region, save it
            regions[region_index].num_polygons = polygon_index;
            region_index++;
            
        } // end if
    }// end while

    int in = 0;
    for (in = 0; in < region_index; in++) {
        if (debug) fprintf(stdout, "%s\n", regions[in].name);
    }

    return regions;

            
}
