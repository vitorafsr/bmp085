all: bmp085

bmp085: main.c bmp085_bst.c
	gcc -c main.c -I ../include/
	gcc -c bmp085_bst.c -I ../include/
	gcc -o bmp085 *.o
