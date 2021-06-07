all: exec

libs: asm-lib

asm-lib: task1.s
	nasm -f elf32 task1.s -o task1.o

exec:libs
	ld -m elf_i386 task1.o -o task1

.PHONY: clean
clean:
	rm -rf ./*.o task1