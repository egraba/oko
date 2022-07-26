#
# Makefile for macOS 
#

CC		= clang -g
CFLAGS	= -Wall -Wextra

EXEC	= oko
SRC		= $(wildcard *.c)
LIBS	= -framework CoreFoundation -framework IOKit -lncurses
OBJS	= $(SRC:.c=.o)

.PHONY: all clean
.DEFAULT: all
	
all: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(EXEC) $(OBJS)

clean:
	rm -f $(EXEC)
	rm -f *.o
