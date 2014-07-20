# Test application with LMDB database

###############################################################################
# Just a test app for playing with LMDB.
#

CC      = gcc
CFLAGS  = -W -Wall -Wabi -std=c1x -pthread -pedantic -O3
LDFLAGS = 
SRC     = test_lmdb.c mdb.c midl.c

progname = test_lmdb


$(progname): mdb.o midl.o
	$(CC) $(CFLAGS) $(progname).c -o $@ $?

%.o: %.c lmdb.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $<

.PHONY: clean
clean:
	rm -rf $(progname) *.[ao] *.so *~
