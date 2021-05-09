all: myShell

myShell: myShell.o LineParser.o
	gcc -m32 -g -Wall -o myShell myShell.o LineParser.o
	rm -rf ./*.o

myShell.o: myShell.c
	gcc -g -Wall -m32 -c -o myShell.o myShell.c

LineParser.o: LineParser.c
	gcc -g -Wall -m32 -c -o LineParser.o LineParser.c

.PHONY: clean
clean:
	rm -rf ./*.o myShell