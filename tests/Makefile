CC		= clang -g
CFLAGS	= -Wall -Wextra

EXEC	= oko_tests
SRC		= tests.c
OBJS	= ${SRC:.c=.o}

all: ${OBJS}
	${CC} ${CFLAGS} -o ${EXEC} ${OBJS}

clean:
	rm -f ${EXEC}
	rm -f *.o
