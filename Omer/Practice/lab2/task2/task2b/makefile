all: exec

exec: task2b.c
	gcc -g -m32 -c -o task2b.o task2b.c
	gcc -g -m32  task2b.o -o main
	rm task2b.o

.PHONY: clean
clean:
	rm -rf ./*.o main