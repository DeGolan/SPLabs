all: task3

task3: task3.o LineParser.o
	gcc -m32 -g -Wall -o task3 task3.o LineParser.o
	rm -rf ./*.o

task3.o: task3.c
	gcc -g -Wall -m32 -c -o task3.o task3.c

LineParser.o: LineParser.c
	gcc -g -Wall -m32 -c -o LineParser.o LineParser.c

.PHONY: clean
clean:
	rm -rf ./*.o task3