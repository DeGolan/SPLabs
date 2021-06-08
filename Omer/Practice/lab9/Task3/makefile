all: exec

libs: asm-lib

asm-lib: task3.s
	nasm -f elf32 task3.s -o task3.o

exec:libs
	ld -m elf_i386 task3.o -o task3

.PHONY: clean
clean:
	rm -rf ./*.o task3