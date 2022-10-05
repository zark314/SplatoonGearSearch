TARGET=splatoon_gear_search
CC=clang

all:
	${CC} ${TARGET}.c -O3 -o ${TARGET}

run:
	./${TARGET}

clean:
	rm ./${TARGET}