all: exec

libs:  c-lib



c-lib: convertor.c
	gcc -g -m32 -c -o convertor.o convertor.c

exec: libs
	gcc -g -m32  convertor.o  -o main
	rm convertor.o 

.PHONY: clean
clean:
	rm -rf ./*.o main
