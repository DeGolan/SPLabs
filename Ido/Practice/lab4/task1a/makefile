all: exec

libs: asm-lib c-lib

asm-lib: start.s
	
	nasm -f elf32 start.s -o start.o

c-lib: util.c encoder.c
	
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector encoder.c -o encoder.o

exec: libs
	ld -m elf_i386 start.o encoder.o util.o -o task1
	rm encoder.o start.o util.o

.PHONY: clean
clean:
	rm -rf ./*.o main
