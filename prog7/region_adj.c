#include "region_adj.h"


Boolean is_adjacent_region (region* r1, region* r2) 
{
    //TODO make this faster with bounding boxes
    //Check all polygons in r1 against r2
    if (debug) fprintf(debug_file, "Comparing %s:%s\n", r1->name, r2->name);
    int i,j;
    for (i = 0; i < r1->num_polygons; i++) {
        for (j = 0; j < r2->num_polygons; j++) {
            if (is_adjacent_polygon(&r1->polygons[i], &r2->polygons[j]))
                return(TRUE);
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
    double a_top = a1->y - a2->y;
    double a_bottom = a1->x - a2->x;
    double b_top = b1->y - b2->y;
    double b_bottom = b1->x - b2->x;

    double b_slope =  b_top/b_bottom;
    double a_slope = a_top/a_bottom;

    if (a_bottom == 0 && b_bottom == 0) {
        // Need to check Y overlap now
        double max_y_a = a1->y > a2->y ? a1->y : a2->y;
        double min_y_a = a1->y < a2->y ? a1->y : a2->y;
        double min_y_b = b1->y < b2->y ? b1->y : b2->y;
        double max_y_b = b1->y > b2->y ? b1->y : b2->y;
        if (max_y_a > min_y_b && min_y_a < max_y_b)
            return(TRUE);
        else if (max_y_b > min_y_a && min_y_b < max_y_a)
            return(TRUE);
        return(FALSE);
    }
    else if (a_bottom == 0 || b_bottom == 0) {
        return (FALSE);
    }
    else if (fabs(fabs(a_slope)-fabs(b_slope)) < EPSILON) {
        double a_y_intercept = -a_slope*a1->x + a1->y;
        double b_y_intercept = -b_slope*b1->x + b1->y;
        if (fabs(fabs(a_y_intercept)-fabs(b_y_intercept)) < EPSILON) {
            double max_x_a = a1->x > a2->x ? a1->x : a2->x;
            double min_x_a = a1->x < a2->x ? a1->x : a2->x;
            double min_x_b = b1->x < b2->x ? b1->x : b2->x;
            double max_x_b = b1->x > b2->x ? b1->x : b2->x;
            if (debug) fprintf(debug_file, "\tintercepts equal \t\t%.2f | %.2f\n", a_y_intercept, b_y_intercept);
            if (debug) fprintf(debug_file, "\t\t maxA%.2f minA%.2f maxB%.2f minB%.2f\n", max_x_a, min_x_a, max_x_b,
                min_x_b);
            if (max_x_a > min_x_b && min_x_a < max_x_b)
                return(TRUE);
            else if (max_x_b > min_x_a && min_x_b < max_x_a)
                return(TRUE);
            return(FALSE);
        }
        return (FALSE);
    }
    else return (FALSE);
}

void process_map_data (region* regions) {
    int i = 0, j = 0;
    Boolean region = FALSE;
    Boolean first = TRUE;

    //TODO make this faster with bounding boxes

    /* Check all regions against each other that are in the same bounding box */
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
        if (region)
            fprintf(stdout, "\n");
        region = FALSE;
        first = TRUE;
        j= 0;
        i++;
    }
}

