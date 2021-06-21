utils.o: utils.c utils.h
	gcc -c $< 

%.x: %.c utils.o
	gcc $< utils.o -o $@ -luv

