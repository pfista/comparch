Programming Assignment 7 -- CS429 Spring 2013

Due Date: 26 April 2013
Region Adjacency

Write a program to read a set of regions, and then report, for each region,
which other regions are adjacent to it. Two regions are adjacent if any edge of
one intersects with any edge of the other in more than one point.

Input is a file of text defining a set of regions. Each region is a set of
polygons. Each polygon is a sequence of points, defining a closed area.

A region starts with a left brace "{", followed by the name of the region, a
comma, and then a comma-separated list of polygons, followed by a right brace.
For example: { R1, polygon } or { R 2, polygon1, polygon2 }

A polygon is a left square bracket "[" followed by a list of comma-separated
points, followed by a right square bracket "]". For example, [ point, point,
point ]

A point is an x,y pair, expressed as a left parenthesis "(" followed by two
comma separated numbers, followed by a right parenthesis ")". For example, (
23.23, -2.0 )

Ignore spaces and line breaks.

A simple set of 3 regions, each region being one polygon is:

{ Region1, [ (0,1), (0,2), (2,2), (2,1), (0,1) ]}
{ Region2, [ (1,0), (3,0), (3,1), (1,1), (1,0) ]}
{ Region3, [ (3,0), (3,1), (4.1,1), (4.1,0), (3,0) ]}
We don't know the units of the points. They can be positive or negative,
integer or floating point numbers.

Each polygon should be closed -- it's first point should be the same as it's
last point.

Note that in the example, Region3 is adjacent to Region2, since they both share
the edge from (3,0) to (3,1). But Region1 is adjacent to Region2 because the
edge from (2,1) to (0,1) for Region1 intersects the edge from (3,1) to (1,1)
for Region2. They share the (non-point) edge from (1,1) to (2,1).

Output for the sample input above would then be:

Region1: Region2
Region2: Region1, Region3
Region3: Region2
If the input is badly formed, simply report: "Bad input at line %d", giving the
line number where the input does not match the expected input format.
Sample test files are in peterson/maps

Due Date: 26 April 2013a
