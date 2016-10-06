#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

int main(int argc, char **argv) {
  if (argc != 2 && argc != 3) {
    printf("Usage: %s loader text.bin [data.bin]\n", argv[0]);
    return 1;
  }

  printf("doing it\n");

  prussdrv_init();
  if (prussdrv_open(PRU_EVTOUT_0) == -1) {
    printf("prussdrv_open() failed\n");
    return 1;
  }

  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
  prussdrv_pruintc_init(&pruss_intc_initdata);

  printf("Executing program and waiting for termination\n");
  if (argc == 3) {
    printf("attempting to load data file\n");
    if (prussdrv_load_datafile(0 /* PRU0 */, argv[2]) < 0) {
      fprintf(stderr, "Error loading %s\n", argv[2]);
      exit(-1);
    } else {
      printf("Data file loaded\n");
    }
  } else {
    printf("no data file\n");
  }

  if (prussdrv_exec_program(0 /* PRU0 */, argv[1]) < 0) {
    fprintf(stderr, "Error loading %s\n", argv[1]);
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

  return 0;
}
