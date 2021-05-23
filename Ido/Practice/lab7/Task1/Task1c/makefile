all: hexeditplus

hexeditplus: hexeditplus.o
	gcc -m32 -g -Wall -o hexeditplus hexeditplus.o 
	rm -rf ./*.o

hexeditplus.o: hexeditplus.c
	gcc -g -Wall -m32 -c -o hexeditplus.o hexeditplus.c


.PHONY: clean
clean:
	rm -rf ./*.o hexeditplus