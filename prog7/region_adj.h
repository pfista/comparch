#include "global.h"

typedef  struct point {

}point;

typedef struct polygon {

}polygon;

typedef struct region {

}region;

Boolean is_adjacent_region (region* r1, region* r2);

Boolean is_adjacent_polygon (polygon* p1, polygon* p2);

Boolean lines_intersect (point* a1, point* a2, point* b1, point* b2);

void process_map_data (void);
