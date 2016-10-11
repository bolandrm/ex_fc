#include <stdio.h>
#include <stdint.h>
#include "erl_nif.h"

#define MAX_BUF_LEN 1024

typedef struct {
  uint32_t rc1;
  uint32_t rc2;
  uint32_t rc3;
  uint32_t rc4;
  uint32_t rc5;
  uint32_t rc6;
} shared_mem_t;

static ERL_NIF_TERM
enable_prus(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  char text1[MAX_BUF_LEN];
  char data1[MAX_BUF_LEN];
  char text2[MAX_BUF_LEN];
  char data2[MAX_BUF_LEN];

  enif_get_string(env, argv[0], text1, MAX_BUF_LEN, ERL_NIF_LATIN1);
  enif_get_string(env, argv[1], data1, MAX_BUF_LEN, ERL_NIF_LATIN1);
  enif_get_string(env, argv[2], text2, MAX_BUF_LEN, ERL_NIF_LATIN1);
  enif_get_string(env, argv[3], data2, MAX_BUF_LEN, ERL_NIF_LATIN1);

  printf("c printing %s", text1);

  return enif_make_int(env, 0);
}

// Let's define the array of ErlNifFunc beforehand:
static ErlNifFunc nif_funcs[] = {
  // {erl_function_name, erl_function_arity, c_function}
  {"enable_prus", 4, enable_prus}
};

ERL_NIF_INIT(Elixir.ExFC.PruLoader, nif_funcs, NULL, NULL, NULL, NULL)
