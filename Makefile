all: main.c goombasav.c minilzo-2.06/minilzo.c
	gcc -Wall -o goombasav main.c goombasav.c minilzo-2.06/minilzo.c
