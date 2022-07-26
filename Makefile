#
# Makefile for macOS 
#

CC		= clang -g
CFLAGS	= -Wall -Wextra

EXEC	= oko
SRC		= $(wildcard src/*.c)
LIBS	= -framework CoreFoundation -framework IOKit -lncurses
OBJS	= $(SRC:src/.c=build/.o)

.PHONY: all clean
.DEFAULT: all

all: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(EXEC) $(OBJS)

clean:
	rm -f $(EXEC)
	rm -rdf $(EXEC).dSYM
