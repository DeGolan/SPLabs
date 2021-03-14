exec:
	gcc -g -m32 -c -o encoderB.o encoderB.c
	gcc -g -m32  encoderB.o -o encoder
	rm encoderB.o

.PHONY: clean
clean:
	rm -rf ./*.o encoder