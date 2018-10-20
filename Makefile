CFLAGS = -g -Wall -Wextra -Werror -pedantic -std=c99 -fPIC -fvisibility=hidden
LDFLAGS = -shared
TARGET_LIB = libmalloc.so
VPATH = . src
OBJS = malloc.o free.o
TEST = tests/test.sh

.PHONY: all ${TARGET_LIB} clean

${TARGET_LIB}: ${OBJS}
	${CC} ${LDFLAGS} -o $@ $^

all: ${TARGET_LIB}

check: 
	$(TEST)

clean:
	${RM} ${TARGET_LIB} ${OBJS}
