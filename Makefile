SHELL = /bin/bash
CC = gcc
CFLAGS = -lcrypto
SRC = $(wildcard *.c)
EXE = $(patsubst %.c, %, $(SRC))


all: ${EXE}

%: %.c
	${CC} $@.c -o $@ ${CFLAGS} 

clean:
	rm ${EXE}