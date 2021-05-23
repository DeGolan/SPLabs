all: task4

task4: task4.o
	gcc -m32 -fno-pie -g -Wall -o task4 task4.o 
	rm -rf ./*.o

task4.o: task4.c
	gcc -g -Wall -m32 -fno-pie -c -o task4.o task4.c


.PHONY: clean
clean:
	rm -rf ./*.o task4