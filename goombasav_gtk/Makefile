all: goombasav_gtk

goombasav_gtk: gtkmain.c ../goombasav.c ../minilzo-2.06/minilzo.c
	gcc -std=c99 -o goombasav_gtk gtkmain.c ../goombasav.c ../minilzo-2.06/minilzo.c `pkg-config --cflags --libs gtk+-2.0`

