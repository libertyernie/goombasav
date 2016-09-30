all: goombasav gbaromextract

goombasav: main.c goombasav.c minilzo/minilzo.c
	gcc -Wall -o goombasav main.c goombasav.c minilzo/minilzo.c

gbaromextract: gbaromextract.c goombarom.c pocketnesrom.c
	gcc -Wall -o gbaromextract gbaromextract.c goombarom.c pocketnesrom.c
