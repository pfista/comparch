#include "region_adj.h"


Boolean is_adjacent_region (region* r1, region* r2) 
{
    //Check all polygons in r1 against r2
    if (debug) fprintf(debug_file, "Comparing %s:%s\n", r1->name, r2->name);
    int i,j;
    for (i = 0; i < r1->num_polygons; i++) {
        for (j = 0; j < r2->num_polygons; j++) {
            if (is_adjacent_polygon(&r1->polygons[i], &r2->polygons[j]))
                return(TRUE);
            //TODO make this faster
        }
    }
    return(FALSE);
}

Boolean is_adjacent_polygon (polygon* p1, polygon* p2) 
{
    int i,j;
    for (i = 0; i < p1->num_vertices-1; i++){
        for (j = 0; j < p2->num_vertices-1; j++) {
            if (lines_intersect(&p1->vertices[i], &p1->vertices[i+1], &p2->vertices[j], &p2->vertices[j+1]))
                return(TRUE);
        }
    }
    return(FALSE);
}

Boolean lines_intersect (point* a1, point* a2, point* b1, point* b2) 
{
    if (debug) fprintf(debug_file, "\t(%.2f,%.2f)(%.2f,%.2f)\n\t(%.2f,%.2f)(%.2f,%.2f)\n", a1->x, a1->y,
            a2->x, a2->y, b1->x, b1->y, b2->x, b2->y);
    double top = (a1->y - a2->y);
    double bottom = (a1->x - a2->x) == 0 ? 999999 : a1->x - a2->x;
    double a_slope = top/bottom;
    top = (b1->y - b2->y);
    bottom = (b1->x - b2->x) == 0 ? 999999 : b1->x - b2->x;
    double b_slope =  top/bottom;


    // TODO epsilon
    if (a_slope == b_slope) {
        double a_y_intercept = -a_slope*a1->x + a1->y;
        double b_y_intercept = -b_slope*b1->x + b1->y;
        if (debug) fprintf(debug_file, "\t\t%.2f | %.2f\n", a_y_intercept, b_y_intercept);
        if (a_y_intercept == b_y_intercept) {
            double max_x_a = a1->x > a2->x ? a1->x : a2->x;
            double min_x_b = b1->x < b2->x ? b1->x : b2->x;
            if (max_x_a > min_x_b)
                return(TRUE);
            return(FALSE);
        }
        return (FALSE);
    }
    return (FALSE);
}

void process_map_data (region* regions) {
    int i = 0, j = 0;
    Boolean region = FALSE;
    Boolean first = TRUE;

    while (regions[i].polygons != NULL) {
        while (regions[j].polygons != NULL) {
            if (i != j && is_adjacent_region(&regions[i], &regions[j])) {
                if (!region) {
                    fprintf(stdout, "%s: ", regions[i].name);
                    region = TRUE;
                }
                if (first) {
                    fprintf(stdout, "%s", regions[j].name);
                    first = FALSE;
                }
                else
                    fprintf(stdout, ", %s", regions[j].name);
            }
            j++;
        }
        fprintf(stdout, "\n");
        region = FALSE;
        first = TRUE;
        j= 0;
        i++;
    }
}

