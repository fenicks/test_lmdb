#include <stdio.h>

#include "lmdb.h"

static const char *store_dir = NULL;
static int  err              = 0;

int main(int ac, char **ag) {
  /* Parse parameters */
  if (ac != 2) {
    fprintf(stderr, "Need a parameter to specify a directory database!\n");
    return -42;
  } else {
    store_dir = ag[1];
  }
  printf("Our data store directory is '%s'\n", store_dir);

  /* MDB environnment */
  MDB_env *env;
  err = mdb_env_create(&env);
  if (err != MDB_SUCCESS) {
    fprintf(stderr, "%s\n", mdb_strerror(err));
    return -42;
  }

  err = mdb_env_open(env, store_dir, 0, 0640);
  if (err != MDB_SUCCESS) {
    fprintf(stderr, "Can't open the data store '%s': %s\n", store_dir, mdb_strerror(err));
    mdb_env_close(env); /* As mentioned in the documentation */
    return -42;
  }

  /* Handling transaction */
  MDB_txn *txn;
  err = mdb_txn_begin(env, NULL, 0, &txn);
  if (err != MDB_SUCCESS) {
    fprintf(stderr, "Can't begin a transaction: %s\n", mdb_strerror(err));
    mdb_env_close(env);
    return -42;
  }

  /* Handle databases in environment */
  MDB_dbi dbi;
  err = mdb_dbi_open(txn, NULL, 0, &dbi);
  if (err != MDB_SUCCESS) {
    fprintf(stderr, "Can't open a database: %s\n", mdb_strerror(err));
  }

  /* Handling keys and data */
  MDB_val key, value;
  char sval[64];

  key.mv_size = sizeof(int);
  key.mv_data = sval;

  sprintf(sval, "%03x %d First value for the first key", 1000, 1000);
  value.mv_size = sizeof(sval);
  value.mv_data = sval;

  err = mdb_put(txn, dbi, &key, &value, 0);
  if (err != MDB_SUCCESS) {
    fprintf(stderr, "Can't add a key/value to the database: %s\n", mdb_strerror(err));
  }

  /* Navigation into the database */
  MDB_cursor *cursor;
  err = mdb_cursor_open(txn, dbi, &cursor);
  if (err != MDB_SUCCESS) {
    fprintf(stderr, "Can't open a cursor to the database: %s\n", mdb_strerror(err));
  }
  printf("[MDB data store content]\n");
  printf("========================\n");
  MDB_val ret_k, ret_v;
  while ((err = mdb_cursor_get(cursor, &ret_k, &ret_v, MDB_NEXT)) == MDB_SUCCESS) {
    printf("key: %s - val: %s\n", ret_k.mv_data, ret_v.mv_data);
  }
  printf("========================\n");

  /* Get a database statistics */
  //MDB_stat stats;

  /* Clean the allocated resources for an opened environment */
  mdb_cursor_close(cursor);
  mdb_dbi_close(env, dbi);
  mdb_txn_abort(txn);
  mdb_env_close(env);

  /* Easy! */
  printf("Easy, very easy isn't it?\n");
  return 0;
}
