all: exec

exec: menu_map.c
	gcc -g -m32 -c -o menu_map.o menu_map.c
	gcc -g -m32  menu_map.o -o main
	rm menu_map.o

.PHONY: clean
clean:
	rm -rf ./*.o main