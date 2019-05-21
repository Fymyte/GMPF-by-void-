# Makefile for GMPF

CC=clang
CFLAGS=-Wall -Wextra -std=c99  `pkg-config --cflags gtk+-3.0` -O3
LDLIBS= `pkg-config --libs gtk+-3.0` `pkg-config --libs gmodule-2.0`
LDFLAGS=
EXEC=GMPF.out
SRC= $(wildcard src/*.c)
OBJ= $(SRC:.c=.o)

.PHONY: clean clean_save all

all:
	@$(MAKE) GMPF.out -j 8 --no-print-directory

GMPF.out: $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)


%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)


clean:
	@rm -rf $(OBJ) $(EXEC)

clean_save:
	@rm -rf *.gmpf
