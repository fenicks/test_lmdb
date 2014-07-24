#include <stdio.h>
#include <string.h>

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

  const char * data_k[3] = {"user:1:nickname", "user:1:email"   , "user:1:created_at"};
  const char * data_v[3] = {"fenicks"        , "fenicks@lmdb.db", "1977-04-22T06:00:00Z"};

  for (int iter = 0; iter < 3; ++iter) {
    key.mv_size   = sizeof(char*) * strlen(data_k[iter]);
    key.mv_data   = (void*)data_k[iter];

    value.mv_size = sizeof(char*) * strlen(data_v[iter]);
    value.mv_data = (void*)data_v[iter];

    err = mdb_put(txn, dbi, &key, &value, 0);
    if (err != MDB_SUCCESS) {
      fprintf(stderr, "Can't add a key/value to the database: %s\n", mdb_strerror(err));
    }
  }

  /* Navigation into the database */
  MDB_cursor *cursor;
  err = mdb_cursor_open(txn, dbi, &cursor);
  if (err != MDB_SUCCESS) {
    fprintf(stderr, "Can't open a cursor to the database: %s\n", mdb_strerror(err));
  }
  printf("[MDB data store content]\n");
  printf("========================\n");
  while ((err = mdb_cursor_get(cursor, &key, &value, MDB_NEXT)) == MDB_SUCCESS) {
    printf("key: %s\t- val: %s\n", (char*)key.mv_data, (char*)value.mv_data);
  }
  printf("========================\n");

  /* Get a database statistics */
  /* MDB_stat stats; */

  /* Clean the allocated resources for an opened environment */
  mdb_cursor_close(cursor);
  mdb_dbi_close(env, dbi);
  mdb_txn_abort(txn);
  mdb_env_close(env);

  printf("Easy, very easy isn't it?\n");
  return 0;
}
