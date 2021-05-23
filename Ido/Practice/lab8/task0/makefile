all: myELF

myELF: myELF.o
	gcc -m32 -g -Wall -o myELF myELF.o 
	rm -rf ./*.o

myELF.o: myELF.c
	gcc -g -Wall -m32 -c -o myELF.o myELF.c


.PHONY: clean
clean:
	rm -rf ./*.o myELF