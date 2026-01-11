# a simple asteroids game

APPNAME   = asteroids
SRCDIR    = ./src
BINDIR    = ./bin
ASTDIR    = ./assets
CACHEDIR  = ./.cache
ASTBINDIR = ${BINDIR}/${ASTDIR}
BIN = ${BINDIR}/${APPNAME}
SRC = ${wildcard ${SRCDIR}/*.c ${SRCDIR}/**/*.c}
AST = ${wildcard ${ASTDIR}/*.*}
OBJ = ${SRC:.c=.o}
INC = raylib5.5
LIB = ${SRCDIR}/libraylib.a
LNK = -lm

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    FLAGS := -framework IOKit -framework Cocoa
endif

CC = clang
# TODO: verify if any of this is redundant
CFLAGS = -I./${INC} -Wall -Wextra -Wconversion -Wdouble-promotion        \
         -Wno-unused-parameter -Wno-unused-function -Wno-sign-conversion \
         -fsanitize=undefined -fsanitize-trap -std=c23                   \
         -D_POSIX_C_SOURCE=200809L

${BIN}: ${OBJ} ${LIB} ${ASTBINDIR}
	${CC} -g -o ${BIN} ${OBJ} ${FLAGS} ${LIB} ${LNK}

${ASTBINDIR}: ${AST}
	mkdir -p ${ASTBINDIR}
	rm -f ${ASTBINDIR}/*
	cp -f ${AST} ${ASTBINDIR}/

run: CFLAGS += -g
run: ${BIN}
	${BIN}

rerun: clean run

runopt: CFLAGS += -O3
runopt: run

${LIB}: ${INC}
	cd ${INC} && make

clean:
	rm -f ${BIN} ${OBJ} ${LIB} ${ASTBINDIR}/*

cleandeps:
	cd ${INC} && make clean

rmbindir:
	rm -rf ${BINDIR}

rmcachedir:
	rm -rf ${CACHEDIR}

cleanall: clean cleandeps rmbindir rmcachedir

fresh: cleanall ${BIN}

.PHONY: clean cleandeps rmbindir rmcachedir cleanall fresh run rerun runopt
