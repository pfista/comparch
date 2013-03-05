CC=gcc
CCFLAGS=-DDEBUG	

a.out: topoSort
	cp topoSort a.out
topoSort: topoSort.o
topoSort.o: topoSort.c topoSort.h
	
clean:
	rm *.o topoSort a.out
