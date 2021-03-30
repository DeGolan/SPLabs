all: encoder

libs: c-libs

c-libs: encoder.c
	gcc -g -Wall -c -m32 -o encoder.o encoder.c

encoder: libs
	gcc -g -Wall -m32 encoder.o -o encoder
	rm encoder.o

.PHONY: clean
clean:
	rm -rf ./*.o lab1
