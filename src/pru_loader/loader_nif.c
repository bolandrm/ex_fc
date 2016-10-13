#include <stdio.h>
#include <stdint.h>
#include "erl_nif.h"
#include "loader.h"

#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#define MAX_BUF_LEN 1024

static ERL_NIF_TERM
nif_enable_prus(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  char text0[MAX_BUF_LEN];
  char data0[MAX_BUF_LEN];
  char text1[MAX_BUF_LEN];
  char data1[MAX_BUF_LEN];

  enif_get_string(env, argv[0], text0, MAX_BUF_LEN, ERL_NIF_LATIN1);
  enif_get_string(env, argv[1], data0, MAX_BUF_LEN, ERL_NIF_LATIN1);
  enif_get_string(env, argv[2], text1, MAX_BUF_LEN, ERL_NIF_LATIN1);
  enif_get_string(env, argv[3], data1, MAX_BUF_LEN, ERL_NIF_LATIN1);

  printf("c printing %s \n", text0);

  load_pru0(text0, data0);

  return enif_make_int(env, 0);
}

static ERL_NIF_TERM
nif_read_rc_values(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  //uint32_t rc_values[4] = { 0 };
  //read_rc_values(rc_values);

  uint32_t *mem_pointer;
  prussdrv_map_prumem(PRUSS0_SHARED_DATARAM, (void **) &mem_pointer);

  return enif_make_tuple4(
    env,
    //enif_make_int(env, rc_values[0]),
    //enif_make_int(env, rc_values[1]),
    //enif_make_int(env, rc_values[2]),
    //enif_make_int(env, rc_values[3])
    enif_make_int(env, mem_pointer[0]),
    enif_make_int(env, mem_pointer[1]),
    enif_make_int(env, mem_pointer[2]),
    enif_make_int(env, mem_pointer[3])
  );
}

static ErlNifFunc nif_funcs[] = {
  // {erl_function_name, erl_function_arity, c_function}
  {"enable_prus", 4, nif_enable_prus},
  {"read_rc_values", 0, nif_read_rc_values}
};

ERL_NIF_INIT(Elixir.ExFC.PruLoader, nif_funcs, NULL, NULL, NULL, NULL)
