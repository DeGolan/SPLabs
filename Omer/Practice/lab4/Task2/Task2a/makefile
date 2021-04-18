all: exec

libs: asm-lib c-lib

asm-lib: start.s
	
	nasm -f elf32 start.s -o start.o

c-lib: main.c util.c
	
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main.c -o main.o

exec: libs
	ld -m elf_i386 start.o main.o util.o -o flame2
	rm util.o start.o main.o

.PHONY: clean
clean:
	rm -rf ./*.o flame2
