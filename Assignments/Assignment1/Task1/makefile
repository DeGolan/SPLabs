all: exec

libs: asm-lib c-lib

asm-lib: asm_task1Assignment1.s
	
	nasm -g -f elf -o asm_task1Assignment1.o asm_task1Assignment1.s

c-lib: main_task1Assignment1.c
	
	gcc -g -m32 -c -o main_task1Assignment1.o main_task1Assignment1.c

exec: libs
	gcc -g -m32  main_task1Assignment1.o asm_task1Assignment1.o -o main
	rm main_task1Assignment1.o asm_task1Assignment1.o

.PHONY: clean
clean:
	rm -rf ./*.o main
