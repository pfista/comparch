CC=gcc

all: a.out 

debug: CC += -DDEBUG -g
debug: a.out 

all : a.out

a.out : topoSort
	cp topoSort a.out
topoSort : topoSort.o
topoSort.o : topoSort.c topoSort.h

clean :
	rm *.o topoSort a.out
