#include "loader.h"
#include <stdio.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include <string.h>

void load_pru0(char *text_file, char *data_file) {
  // printf("doing it\n");

  prussdrv_init();
  if (prussdrv_open(PRU_EVTOUT_0) == -1) {
    printf("prussdrv_open() failed\n");
  }

  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
  prussdrv_pruintc_init(&pruss_intc_initdata);

  // printf("Executing program and waiting for termination\n");

  // printf("attempting to load data file\n");
  if (prussdrv_load_datafile(PRU1, data_file) < 0) {
    fprintf(stderr, "Error loading %s\n", data_file);
  } else {
    printf("Data file loaded\n");
  }

  if (prussdrv_exec_program(PRU1, text_file) < 0) {
    fprintf(stderr, "Error loading %s\n", text_file);
  }

  // Wait for the PRU to let us know it's done
  // prussdrv_pru_wait_event(PRU_EVTOUT_0);
  // printf("All done\n");

  // uint32_t *mem_pointer;
  // prussdrv_map_prumem(PRUSS0_SHARED_DATARAM, (void **) &mem_pointer);

  // fprintf(stdout, "read the following %u, %u, %u \n", mem_pointer[0], mem_pointer[1], mem_pointer[2]);

  // prussdrv_pru_disable(PRU1);
  // prussdrv_exit();
}

void read_rc_values(uint32_t rc_values[]) {
  uint32_t *mem_pointer;
  prussdrv_map_prumem(PRUSS0_SHARED_DATARAM, (void **) &mem_pointer);
  memcpy(rc_values, mem_pointer, 4*4);
}
