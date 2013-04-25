#include "global.h"

/* x, y pair, could be ints or floats */
typedef struct point {
    double x, y;
}point;

/* set of points defining a closed area
 start and end with the same point */
typedef struct polygon {
    point* vertices;
    int num_vertices;
}polygon;

/* Set of polygons */
typedef struct region {
    polygon* polygons;
    int num_polygons;
    char* name;
}region;

Boolean is_adjacent_region (region* r1, region* r2);

Boolean is_adjacent_polygon (polygon* p1, polygon* p2);

Boolean lines_intersect (point* a1, point* a2, point* b1, point* b2);

void process_map_data (region* regions);
