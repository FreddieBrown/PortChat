CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c99
TARGET := main

SRCDIR := src
OBJDIR := obj

SRC := ${wildcard ${SRCDIR}/*.c}
OBJ := ${SRC:${SRCDIR}/%.c=${OBJDIR}/%.o}

default: build

debug: CFLAGS += -g
debug: build

linux: CFLAGS += -lpthread
linux: build
	
build: ${OBJ}
	${CC} ${CFLAGS} ${OBJ} -o ${TARGET}

${OBJDIR}/%.o: ${SRCDIR}/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

run: 
	./${TARGET} $(port)

clean:
	rm -f ${OBJDIR}/*.o ${TARGET}
