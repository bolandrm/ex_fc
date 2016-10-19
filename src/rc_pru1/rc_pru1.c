#include <stdint.h>
#include <pru_cfg.h>
#include <pru_iep.h>
#include "shared_headers/types.h"

#define bv(bit) (1 << (bit))
#define bit_read(value, bit) (((value) >> (bit)) & 0x01)
#define bit_set(value, bit) ((value) |= bv(bit))
#define bit_clear(value, bit) ((value) &= ~bv(bit))

#define NUM_PINS 4
#define CH1_INPUT 6
#define CH2_INPUT 7
#define CH3_INPUT 4
#define CH4_INPUT 5

volatile shared_mem_t *shared_mem = (shared_mem_t *) 0x00010000;

volatile register uint32_t __R30;
volatile register uint32_t __R31;

void init();
void init_iep();
void clear_channel_values();

uint32_t channel_inputs[NUM_PINS] = { CH1_INPUT, CH2_INPUT, CH3_INPUT, CH4_INPUT };
uint32_t channel_start[NUM_PINS] = { 0 };
uint32_t input_previous_values[NUM_PINS] = { 0 };

void main(void) {
  init();
  clear_channel_values();

  uint32_t i;

  for (;;) {
    for (i = 0; i < NUM_PINS; i++) {
      uint32_t current_val = bit_read(__R31, channel_inputs[i]);

      if (current_val == input_previous_values[i]) continue;

      if (current_val) {
        channel_start[i] = CT_IEP.TMR_CNT_bit.COUNT;
      } else {
        shared_mem->rc_values[i] = CT_IEP.TMR_CNT_bit.COUNT - channel_start[i];
      }

      input_previous_values[i] = current_val;
    }
  }
}

void init_iep() {
	/* Reset Count register */
	CT_IEP.TMR_CNT = 0x0;

	/* Clear overflow status register */
	CT_IEP.TMR_GLB_STS_bit.CNT_OVF = 0x1;

	/* Enable counter */
	CT_IEP.TMR_GLB_CFG_bit.DEFAULT_INC = 0x1;
	CT_IEP.TMR_GLB_CFG_bit.CNT_EN = 0x1;

	/* Disable compensation */
	CT_IEP.TMR_COMPEN_bit.COMPEN_CNT = 0x0;
}

void init() {
	/* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

  init_iep();
}

void clear_channel_values() {
  uint32_t i;
  for (i = 0; i < NUM_PINS; i++) {
    shared_mem->rc_values[i] = 0;
  }
}

// volatile uint32_t gpio;

// gpio = 1 << 15;

// uint32_t i = 0;
// uint32_t count = 0;

// while (1) {
// 	__R30 ^= gpio;
// 	__delay_cycles(10000000);
// 	i++;

// 	uint32_t count = CT_IEP.TMR_CNT_bit.COUNT;
// 	//if (count > 3000000000) break;
// 	if (count > 300000000) break;
// }

// count = CT_IEP.TMR_CNT_bit.COUNT;
// while (CT_IEP.TMR_CNT_bit.COUNT - count < 300000000) {
// 	__R30 ^= gpio;
// 	__delay_cycles(100000000);
// 	i++;
// }

// uint32_t *shared_mem = (uint32_t *) 0x00010000;
// shared_mem[0] = 0xFFFF;
// shared_mem[1] = 0x0000;
// shared_mem[2] = 0xF0F0;

// __R30 &= ~(1 << 15);

// __R31 = 35;
// __halt();
