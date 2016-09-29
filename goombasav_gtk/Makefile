all: goombasav_gtk

goombasav_gtk: gtkmain.c ../goombasav.c ../minilzo/minilzo.c
	gcc -std=c99 -o goombasav_gtk gtkmain.c ../goombasav.c ../goombarom.c ../pocketnesrom.c ../minilzo/minilzo.c `pkg-config --cflags --libs gtk+-2.0`

clean:
	rm goombasav_gtk
