CC = gcc
CFLAGS = -Wall -g3 -O0 -std=c11 -D_DEBUG
LDFLAGS =
LDLIBS = -lgsl -lgslcblas

all: main

main:

.PHONY: clean

clean:
	$(RM) -f *.exe *.o *.lib *.dll *.so
