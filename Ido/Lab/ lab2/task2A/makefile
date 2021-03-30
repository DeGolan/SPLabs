all: exec

exec: task2a.c
	gcc -g -m32 -c -o task2a.o task2a.c
	gcc -g -m32  task2a.o -o main
	rm task2a.o

.PHONY: clean
clean:
	rm -rf ./*.o main