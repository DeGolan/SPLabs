exec:
	gcc -g -m32 -c -o encoderC.o encoderC.c
	gcc -g -m32  encoderC.o -o encoder
	rm encoderC.o

.PHONY: clean
clean:
	rm -rf ./*.o encoder