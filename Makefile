#
# Makefile for MacOs 
#

# Compiler rules
CC	= clang -g
CFLAGS	= -Wall -Wextra

# Project settings
EXEC	= oko
SRC	= main.c oko.c
OBJS	= ${SRC:.c=.o}

# Directives
all: ${OBJS}
	${CC} ${CFLAGS} -o ${EXEC} ${OBJS}

clean:
	rm -f ${EXEC}
	rm -f *.o
