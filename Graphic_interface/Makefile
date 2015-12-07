# Basic Makefile GTK+

CC=gcc
CPPFLAGS= `pkg-config --cflags sdl` `pkg-config --cflags gtk+-2.0`
CFLAGS= -g -Wall -Wextra -std=c99 -pedantic -O3 -DNDEBUG
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` `pkg-config --libs gtk+-2.0` -lm

SRC= gtk.c
OBJ= ${SRC:.c=.o}


all: gtk

main : ${OBJ}

clean:
	rm -f *~ *.o 
	rm -f gtk

#gcc `pkg-config --cflags gtk+-2.0` -o console inter_graf.c `pkg-config --libs gtk+-2.0`
# END
