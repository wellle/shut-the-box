run: all
	./stb
all:
	gcc -std=c99 main.c bits.c -o stb
