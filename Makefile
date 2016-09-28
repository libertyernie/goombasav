all: main.c goombasav.c minilzo/minilzo.c
	gcc -Wall -o goombasav main.c goombasav.c minilzo/minilzo.c
