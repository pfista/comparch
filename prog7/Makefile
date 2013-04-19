#
#   Makefile for building region adjacency program
#
CC=gcc
CFLAGS =  -O2 -Wall 
#CFLAGS = -O0 -ggdb3
#CFLAGS = -O0 -fprofile-arcs -ftest-coverage -ggdb3
#CFLAGS = -O2 -pg -ggdb3

all: reg

reg: region_adj.o process_file.o main.o
	gcc $(CFLAGS) region_adj.o process_file.o main.o -o reg

region_adj.o: region_adj.c region_adj.h global.h

process_file.o: process_file.c global.h process_file.h

main.o: main.c global.h region_adj.h

clean: 
	rm -f *.o reg 

