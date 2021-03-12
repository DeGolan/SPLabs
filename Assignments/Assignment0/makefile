all: exec

libs: asm-lib c-lib

asm-lib: asmAssignment0.s
	# flags meanings:
	# -g = debug symbolx
	# -f elf = output format (ELF 32 bit)
	# -o add.o = write assembled file to add.o
	nasm -g -f elf -o asmAssignment0.o asmAssignment0.s

c-lib: mainAssignment0.c
	# flags meanings:
	# -g = debug symbols
	# -m32 = compile to x86_32 architecture (32bit)
	# -c = Don't link (i.e., create an object file, not an executable)
	# -o numbers.o = output compiled file to numbers.o
	gcc -g -m32 -c -o mainAssignment0.o mainAssignment0.c

exec: libs
	gcc -g -m32  mainAssignment0.o asmAssignment0.o -o main
	rm mainAssignment0.o asmAssignment0.o

.PHONY: clean
clean:
	rm -rf ./*.o main
