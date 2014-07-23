# Test application with LMDB database
# LMDB: Lightning Memory-mapped DataBase
###############################################################################
# Just a test app for playing with LMDB.
#

CC      = gcc
CFLAGS  = -W -Wall -Wabi -std=c1x -pthread -pedantic -O3
LDFLAGS = 

progname = test_lmdb


$(progname): mdb.o midl.o $(progname).o
	$(CC) $(CFLAGS) -o $@ $?

%.o: %.c lmdb.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $<

.PHONY: clean
clean:
	rm -rf $(progname) *.[ao] *.so *~
