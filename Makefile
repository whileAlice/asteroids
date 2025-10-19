# a simple asteroids game

APPNAME = asteroids
SRCDIR = ./src
BINDIR = ./bin
BIN = ${BINDIR}/${APPNAME}
SRC = ${wildcard ${SRCDIR}/*.c}
OBJ = ${SRC:.c=.o}
INC = ./raylib5.5
LNK = ${SRCDIR}/libraylib.a

CC = clang
# TODO: verify if any of this is redundant
CFLAGS = -I./${INC} -Wall -Wextra -Wconversion -Wdouble-promotion\
         -Wno-unused-parameter -Wno-unused-function -Wno-sign-conversion\
         -fsanitize=undefined -fsanitize-trap -std=c23

${BIN}: ${OBJ} ${LNK}
	${CC} -o ${BIN} ${OBJ} ${LNK}

run: ${BIN}
	${BIN}

${LNK}: ${INC}
	cd ${INC} && make

clean:
	rm -f ${BIN} ${OBJ} ${LNK}

cleandeps:
	cd ${INC} && make clean

cleanall: clean cleandeps

fresh: cleanall ${BIN}

.PHONY: clean cleandeps cleanall fresh run
