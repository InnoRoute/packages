CC = gcc
FLAG = -Wall
LIBS=
CFLAGS=$(shell pkg-config --cflags $(LIBS))
LDFLAGS=$(shell pkg-config --libs $(LIBS))
.PHONY: all install clean

all:clean pllctl

i2ctest:
	$(CC) $(FLAG) $(CFLAGS) $(LDFLAGS) pllctl.c -o pllctl

clean:
	if test -e "pllctl"; then rm pllctl; fi
