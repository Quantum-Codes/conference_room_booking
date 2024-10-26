# ref: https://makefiletutorial.com/
main: roomfuncs.o helper.o mainfile.o
	gcc roomfuncs.o helper.o mainfile.o -o main -lm

roomfuncs.o: roomfuncs.c globalVars.h
	gcc -c roomfuncs.c -o roomfuncs.o

helper.o: helperfuncs.c globalVars.h
	gcc -c helperfuncs.c -o helper.o

mainfile.o: main.c globalVars.h
	gcc -c main.c -o mainfile.o