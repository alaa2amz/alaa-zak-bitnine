all: main

main: simple_test.c pool.c pool.h
	gcc -o simple_test simple_test.c pool.c pool.h
