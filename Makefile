all: p1 p2

p1: p1.c
	gcc -o p1 p1.c -lrt -lpthread

p2: p2.c
	gcc -o p2 p2.c -lrt -lpthread