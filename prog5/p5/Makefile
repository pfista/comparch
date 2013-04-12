all:   prog5

prog5:  decode.o   execute.o  main.o     mem.o
	gcc decode.o   execute.o  main.o mem.o -o prog5

y86.h: y86_decode.h y86_exe.h y86_opcodes.h

decode.o: decode.c y86.h y86_decode.h

execute.o: execute.c y86.h

main.o: main.c y86.h

mem.o: mem.c y86.h


clean:
	rm *.o prog5
