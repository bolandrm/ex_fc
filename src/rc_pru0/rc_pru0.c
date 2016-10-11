/*
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the
 *	  distribution.
 *
 *	* Neither the name of Texas Instruments Incorporated nor the names of
 *	  its contributors may be used to endorse or promote products derived
 *	  from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <pru_cfg.h>
#include <pru_iep.h>
#include "resource_table_empty.h"

#define CLR(x,y) (x&=(~(1<<y)))
#define SET(x,y) (x|=(1<<y))
#define TOGGLE(x,y) (x^=(1<<y))
#define _BV(bit) (1 << (bit))

volatile register uint32_t __R30;
volatile register uint32_t __R31;

void main(void)
{
	volatile uint32_t gpio;

	/* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

	/* Reset Count register */
	CT_IEP.TMR_CNT = 0x0;

	/* Clear overflow status register */
	CT_IEP.TMR_GLB_STS_bit.CNT_OVF = 0x1;

	/* Enable counter */
	CT_IEP.TMR_GLB_CFG_bit.DEFAULT_INC = 0x1;
	CT_IEP.TMR_GLB_CFG_bit.CNT_EN = 0x1;

	/* Disable compensation */
	CT_IEP.TMR_COMPEN_bit.COMPEN_CNT = 0x0;

	// /* Enable counter */
	// CT_IEP.TMR_GLB_CFG = 0x11;

	/* Toggle GPO pins TODO: Figure out which to use */
	gpio = 1 << 15;

	uint32_t i = 0;
	uint32_t count = 0;

	/* TODO: Create stop condition, else it will toggle indefinitely */
	while (1) {
		__R30 ^= gpio;
		__delay_cycles(10000000);
		i++;

		uint32_t count = CT_IEP.TMR_CNT_bit.COUNT;
		//if (count > 3000000000) break;
		if (count > 300000000) break;
	}

	count = CT_IEP.TMR_CNT_bit.COUNT;
	while (CT_IEP.TMR_CNT_bit.COUNT - count < 300000000) {
		__R30 ^= gpio;
		__delay_cycles(100000000);
		i++;
	}

	uint32_t *shared_mem = (uint32_t *) 0x00010000;
	shared_mem[0] = 0xFFFF;
	shared_mem[1] = 0x0000;
	shared_mem[2] = 0xF0F0;

	__R30 &= ~(1 << 15);

	__R31 = 35;
	__halt();
}

