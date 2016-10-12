#include <stdio.h>
#include <stdint.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include "erl_nif.h"

#define MAX_BUF_LEN 1024

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

  printf("c printing %s \n", text1);

  return enif_make_int(env, 0);
}

static ErlNifFunc nif_funcs[] = {
  // {erl_function_name, erl_function_arity, c_function}
  {"enable_prus", 4, enable_prus}
};

ERL_NIF_INIT(Elixir.ExFC.PruLoader, nif_funcs, NULL, NULL, NULL, NULL)

void load_pru1(char *text_file, char *data_file) {
  printf("doing it\n");

  prussdrv_init();
  if (prussdrv_open(PRU_EVTOUT_0) == -1) {
    printf("prussdrv_open() failed\n");
  }

  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
  prussdrv_pruintc_init(&pruss_intc_initdata);

  printf("Executing program and waiting for termination\n");

  printf("attempting to load data file\n");
  if (prussdrv_load_datafile(0 /* PRU0 */, data_file) < 0) {
    fprintf(stderr, "Error loading %s\n", data_file);
    exit(-1);
  } else {
    printf("Data file loaded\n");
  }

  if (prussdrv_exec_program(0 /* PRU0 */, text_file) < 0) {
    fprintf(stderr, "Error loading %s\n", text_file);
    exit(-1);
  }

  // Wait for the PRU to let us know it's done
  prussdrv_pru_wait_event(PRU_EVTOUT_0);
  printf("All done\n");

  uint32_t *mem_pointer;
  prussdrv_map_prumem(PRUSS0_SHARED_DATARAM, (void **) &mem_pointer);

  fprintf(stdout, "read the following %u, %u, %u \n", mem_pointer[0], mem_pointer[1], mem_pointer[2]);

  prussdrv_pru_disable(0 /* PRU0 */);
  prussdrv_exit();
}
