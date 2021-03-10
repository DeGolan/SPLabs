exec:
	gcc -g -m32 -c -o encoder2.o encoder2.c
	gcc -g -m32  encoder2.o -o encoder
	rm encoder2.o

.PHONY: clean
clean:
	rm -rf ./*.o encoder