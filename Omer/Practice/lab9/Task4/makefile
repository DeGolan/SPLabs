all: exec

libs: asm-lib

asm-lib: task4.s
	nasm -f elf32 task4.s -o task4.o

exec:libs
	ld -m elf_i386 task4.o -o task4

.PHONY: clean
clean:
	rm -rf ./*.o task4