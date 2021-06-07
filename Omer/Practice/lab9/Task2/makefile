all: exec

libs: asm-lib

asm-lib: task2.s
	nasm -f elf32 task2.s -o task2.o

exec:libs
	ld -m elf_i386 task2.o -o task2

.PHONY: clean
clean:
	rm -rf ./*.o task2