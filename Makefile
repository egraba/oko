#
# Makefile for macOS 
#
OSX_VERSION = $(shell sysctl kern.osrelease | awk '{split($$2, v, "."); print v[1]}')

CC			= clang -g
ifneq ($(OSX_VERSION), 21)
	CFLAGS = -Wall -Wextra -D OSX_BIG_SUR -D HOST_RUNNER
else
	CFLAGS	= -Wall -Wextra
endif

EXEC		= oko
SRC			= $(wildcard src/*.c)
LIBS		= -framework CoreFoundation -framework IOKit -lncurses
OBJS		= $(SRC:src/.c=.o)

EXEC_TEST 	= oko_test
SRC_TEST	= $(wildcard test/*.c)
LIBS_TEST	= -I src -lcheck
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
	rm -f *.log
	rm -f *_test.txt
