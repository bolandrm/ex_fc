#include <stdio.h>
#include <stdint.h>
#include <erl_nif.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include "shared_headers/types.h"

#define MAX_BUF_LEN 1024

void load_pru1(char *text_file, char *data_file);

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

  load_pru1(text1, data1);

  return enif_make_int(env, 0);
}

static ERL_NIF_TERM
nif_read_rc_values(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  volatile shared_mem_t *shared_mem;
  prussdrv_map_prumem(PRUSS0_SHARED_DATARAM, (void **) &shared_mem);

  return enif_make_tuple4(
    env,
    enif_make_int(env, shared_mem->rc_values[0]),
    enif_make_int(env, shared_mem->rc_values[1]),
    enif_make_int(env, shared_mem->rc_values[2]),
    enif_make_int(env, shared_mem->rc_values[3])
  );
}

static ErlNifFunc nif_funcs[] = {
  // {erl_function_name, erl_function_arity, c_function}
  {"enable_prus", 4, nif_enable_prus},
  {"read_rc_values", 0, nif_read_rc_values}
};

ERL_NIF_INIT(Elixir.ExFC.PruLoader, nif_funcs, NULL, NULL, NULL, NULL)

void load_pru1(char *text_file, char *data_file) {
  prussdrv_init();
  if (prussdrv_open(PRU_EVTOUT_0) == -1) {
    printf("prussdrv_open() failed\n");
  }

  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
  prussdrv_pruintc_init(&pruss_intc_initdata);

  if (prussdrv_load_datafile(PRU1, data_file) < 0) {
    fprintf(stderr, "Error loading %s\n", data_file);
  } else {
    printf("Data file loaded\n");
  }

  if (prussdrv_exec_program(PRU1, text_file) < 0) {
    fprintf(stderr, "Error loading %s\n", text_file);
  }
}
