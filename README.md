# LMDB application: "test_lmdb"

LMDB test application in **C**.
This code is not production ready but helps me to understand how LMDB works.
A simple main function with 99 lines of code is not a good things but it's easy to read for a beginner.

# Requirements

You need `GCC` and `make`.

# Try it!

## Compile the source code

    make

## Create the databases directory

    mkdir $HOME/dbs/lmdb_databases

## Run the test application

    ./test_lmdb $HOME/dbs/lmdb_databases

### Output

    $ ./test_lmdb $HOME/dbs/lmdb_databases
    Our data store directory is '/home/fenicks/dbs/lmdb_databases'
    [MDB database content]
    ======================
    key: user:1:created_at	- val: 1977-04-22T06:00:00Z
    key: user:1:email		- val: fenicks@lmdb.db
    key: user:1:nickname	- val: fenicks
    ======================
    [MDB database statistics]
    =========================
    > Size of database page:        4096
    > Depth of the B-tree:          1
    > Number of items in databases: 3
    =========================
    Easy, very easy isn't it?

That's it.