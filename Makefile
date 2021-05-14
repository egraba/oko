#
# Makefile for MacOs 
#

CC	= clang -g
CFLAGS	= -Wall -Wextra

EXEC	= oko
SRC	= main.c oko.c
LIB	= -lncurses
OBJS	= ${SRC:.c=.o}

all: ${OBJS}
	${CC} ${CFLAGS} $(LIB) -o ${EXEC} ${OBJS}

clean):
	rm) -f ${EXEC}
	rm -f *.o
