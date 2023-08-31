main: main.o
	gcc main.o -o main

main.o: main.c
	gcc -c main.c -Wall

clear:
	rm -f *.o

purge:
	rm -f *.o
	rm -f main