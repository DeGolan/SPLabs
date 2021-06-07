all: exec

libs: asm-lib

asm-lib: task0b.s
	nasm -f elf32 task0b.s -o task0b.o

exec:libs
	ld -m elf_i386 task0b.o -o task0b

.PHONY: clean
clean:
	rm -rf ./*.o task0b