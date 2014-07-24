# Test application with LMDB database
# LMDB: Lightning Memory-mapped DataBase
###############################################################################
# Just a test app for playing with LMDB.
#

CC      = gcc
CFLAGS  = -W -Wall -Wabi -std=c1x -pedantic -O3
LDFLAGS = -lc -pthread

progname = test_lmdb


all: $(progname)

$(progname): $(progname).o mdb.o midl.o lmdb.h
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(progname) *.[ao] *.so *~
