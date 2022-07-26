#
# Makefile for macOS 
#

CC			= clang -g
CFLAGS		= -Wall -Wextra

EXEC		= oko
SRC			= $(wildcard src/*.c)
LIBS		= -framework CoreFoundation -framework IOKit -lncurses
OBJS		= $(SRC:src/.c=.o)

EXEC_TEST 	= oko_test
SRC_TEST	= $(wildcard test/*.c)
LIBS_TEST	= -I src
OBJS_TEST	= $(SRC_TEST:test/.c=.o)

.PHONY: all test clean
.DEFAULT: all

all: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(EXEC) $(OBJS)

test: $(OBJS_TEST)
	$(CC) $(CFLAGS) $(LIBS) $(LIBS_TEST) -o $(EXEC_TEST) $(filter-out src/main.c, $(OBJS)) $(OBJS_TEST)

clean:
	rm -f $(EXEC)
	rm -rdf $(EXEC).dSYM
	rm -f $(EXEC_TEST)
	rm -rdf $(EXEC_TEST).dSYM
