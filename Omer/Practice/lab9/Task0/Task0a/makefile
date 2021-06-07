all: task0a

task0a: task0a.o
	gcc -m32 -g -Wall -o task0a task0a.o 
	rm -rf ./*.o

task0a.o: task0a.c
	gcc -g -Wall -m32 -c -o task0a.o task0a.c


.PHONY: clean
clean:
	rm -rf ./*.o task0a