CC=gcc

all: a.out

debug: CC += -DDEBUG -g
debug: a.out

a.out : y86sim mem
	cp y86sim a.out

y86sim : y86sim.o mem.o
y86sim.o : y86sim.c y86sim.h
mem.o : mem.c mem.h

clean :
	rm *.o y86sim a.out
