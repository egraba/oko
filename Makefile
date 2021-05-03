#
# Makefile for MacOs 
#

CC	= clang -g
CFLAGS	= -Wall -Wextra

EXEC	= oko
SRC	= main.c oko.c
OBJS	= ${SRC:.c=.o}

all: ${OBJS}
	${CC} ${CFLAGS} -o ${EXEC} ${OBJS}

clean:
	rm -f ${EXEC}
	rm -f *.o
