exec:
	gcc -g -m32 -c -o encoderD.o encoderD.c
	gcc -g -m32  encoderD.o -o encoder
	rm encoderD.o

.PHONY: clean
clean:
	rm -rf ./*.o encoder