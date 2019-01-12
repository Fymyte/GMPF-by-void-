# Makefile for malloc
 
CC=clang

CFLAGS= -Wall -Wextra -std=c99 -lm `pkg-config --cflags gtk+-3.0`
LDLIBS= `pkg-config --libs gtk+-3.0` `pkg-config --libs gmodule-2.0`

SOURCE_FILE = $(wildcard src/*c)
TEST_FILE = $(wildcard test/*.c)

all:
	${CC} $(LDLIBS) ${CFLAGS} -Ofast -xc -o run.out $(SOURCE_FILE)

c_debug:
	${CC} $(LDLIBS) ${CFLAGS} -g -xc -o debug.out $(SOURCE_FILE)

c_test:
	${CC} $(LDLIBS) ${CFLAGS} -lcriterion -0fast -xc -o test.out $(TEST_FILE)

clean:
	rm -f *~ *.out ; rm -rf memory/
# end

