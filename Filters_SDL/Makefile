# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

SRC= src/binarize.c src/binarize_color.c src/convolution.c src/grayscale.c src/horizontal.c src/Img_rgb_op.c src/Matrix.c src/negative.c src/pixel_operations.c src/sdl_op.c src/tinter.c src/vertical.c main.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: main
-include ${DEP}

main: ${OBJ}

clean:
	${RM} ${OBJ} ${DEP}
	${RM} main

# END
