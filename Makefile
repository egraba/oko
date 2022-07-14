#
# Makefile for macOS 
#

CC		= clang -g
CFLAGS	= -Wall -Wextra

EXEC	= oko
SRC		= display.c log.c main.c oko.c utils.c
LIBS	= -framework CoreFoundation -framework IOKit -lncurses
OBJS	= ${SRC:.c=.o}

all: ${OBJS}
	${CC} ${CFLAGS} $(LIBS) -o ${EXEC} ${OBJS}

clean:
	rm -f ${EXEC}
	rm -f *.o
