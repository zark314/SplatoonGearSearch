CC=gcc
TARGET=splatoon_gear_search

SOURCES=splatoon_gear_search.c constants.c

all: ${SOURCES}
	${CC} ${SOURCES} -O3 -o ${TARGET}

run:
	./${TARGET}

clean:
	rm ./${TARGET}
	rm -f test