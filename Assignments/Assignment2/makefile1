all: calc

calc: calc.o 
	gcc -m64 -g -Wall -o calc calc.o 
	rm -rf ./*.o

calc.o: calc.c
	gcc -g -Wall -m64 -c -o calc.o calc.c


.PHONY: clean
clean:
	rm -rf ./*.o calc