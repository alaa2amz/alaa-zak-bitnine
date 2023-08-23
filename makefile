all: main

main: ptest.c pool.c pool.h
	gcc -o ptest ptest.c  pool.c 
