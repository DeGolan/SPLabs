all: exec

libs: asm-lib 

asm-lib: calculator.s

	nasm -f elf calculator.s -o calculator.o

exec: libs
	gcc -m32 -Wall -g calculator.o -o calc
	rm calculator.o

.PHONY: clean
clean:
	rm -rf ./*.o calc