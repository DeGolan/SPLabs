exec:
	gcc -g -m32 -c -o encoderA.o encoderA.c
	gcc -g -m32  encoderA.o -o encoder
	rm encoderA.o

.PHONY: clean
clean:
	rm -rf ./*.o encoder