# Makefile for LMDB test application

CC  = gcc
CFLAGS  = -W -Wall -Wabi -std=c1x -pedantic -O2
LDFLAGS = -lc -pthread

progname = test_lmdb


all: $(progname)

$(progname): $(progname).o mdb.o midl.o lmdb.h midl.h
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(progname) *.[ao] *.so *~
