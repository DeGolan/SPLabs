all: exec

exec: lab3_bubblesort.c
	gcc -g -m32 -c -o lab3_bubblesort.o lab3_bubblesort.c
	gcc -g -m32  lab3_bubblesort.o -o main
	rm lab3_bubblesort.o

.PHONY: clean
clean:
	rm -rf ./*.o main