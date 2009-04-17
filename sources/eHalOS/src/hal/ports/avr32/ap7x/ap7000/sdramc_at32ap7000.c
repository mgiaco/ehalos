/* This source file is part of the ATMEL AVR32-SoftwareFramework-AT32AP7000-1.0.0 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief SDRAMC on EBI driver for AVR32.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with an SDRAMC module can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ******************************************************************************/

/* Copyright (C) 2006-2008, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "compiler.h"
#include "ngw100.h"
#include "sdramc_at32ap7000.h"
#include "pio.h"
#include "sys/sysregs.h"
#include "pm_at32ap7000.h"

#include "init.h"

static void sdramc_enable_muxed_pins(void)
{
  volatile avr32_hmatrix_t *hmatrix = &AVR32_HMATRIX;

  // Enable SDRAM mode for CS1 in the BAMBI mux
  hmatrix->sfr[AVR32_SDRAMC_HMATRIX_NR] |= 0x0002;
  hmatrix->sfr[AVR32_SDRAMC_HMATRIX_NR] |= 0x0100;

//  static const gpio_map_t SDRAMC_EBI_GPIO_MAP =
//  {
//    // Enable DATA 16 through 31 pins, which is muxed with LCD
//    {AVR32_EBI_DATA_16_PIN,           AVR32_EBI_DATA_16_FUNCTION          },
//    {AVR32_EBI_DATA_17_PIN,           AVR32_EBI_DATA_17_FUNCTION          },
//    {AVR32_EBI_DATA_18_PIN,           AVR32_EBI_DATA_18_FUNCTION          },
//    {AVR32_EBI_DATA_19_PIN,           AVR32_EBI_DATA_19_FUNCTION          },
//    {AVR32_EBI_DATA_20_PIN,           AVR32_EBI_DATA_20_FUNCTION          },
//    {AVR32_EBI_DATA_21_PIN,           AVR32_EBI_DATA_21_FUNCTION          },
//    {AVR32_EBI_DATA_22_PIN,           AVR32_EBI_DATA_22_FUNCTION          },
//    {AVR32_EBI_DATA_23_PIN,           AVR32_EBI_DATA_23_FUNCTION          },
//    {AVR32_EBI_DATA_24_PIN,           AVR32_EBI_DATA_24_FUNCTION          },
//    {AVR32_EBI_DATA_25_PIN,           AVR32_EBI_DATA_25_FUNCTION          },
//    {AVR32_EBI_DATA_26_PIN,           AVR32_EBI_DATA_26_FUNCTION          },
//    {AVR32_EBI_DATA_27_PIN,           AVR32_EBI_DATA_27_FUNCTION          },
//    {AVR32_EBI_DATA_28_PIN,           AVR32_EBI_DATA_28_FUNCTION          },
//    {AVR32_EBI_DATA_29_PIN,           AVR32_EBI_DATA_29_FUNCTION          },
//    {AVR32_EBI_DATA_30_PIN,           AVR32_EBI_DATA_30_FUNCTION          },
//    {AVR32_EBI_DATA_31_PIN,           AVR32_EBI_DATA_31_FUNCTION          }
//  };
//
//  gpio_enable_module(SDRAMC_EBI_GPIO_MAP, sizeof(SDRAMC_EBI_GPIO_MAP) / sizeof(SDRAMC_EBI_GPIO_MAP[0]));
};

//static void sdram_delay(int tics)
//{
//  int i, loop_limit;
//
//  loop_limit = tics * 20;
//
//  for (i=0; i<loop_limit;i++);
//
//}

void  delay_us (unsigned int usec)
{
	unsigned int start;
	unsigned int tick;
	unsigned int end;

    start = __builtin_mfsr(SYSREG_COUNT);
    tick = usec * (pm_get_mclk_freq_hz() / 1000000);
    end  = start + tick;

    if (start > end) {
        while (__builtin_mfsr(SYSREG_COUNT) > start) {
            ;
        }
    }

    while (__builtin_mfsr(SYSREG_COUNT) < end) {
        ;
    }
}


void sdramc_init(unsigned long hsb_hz)
{

  volatile avr32_sdramc_t *sdramc = &AVR32_SDRAMC;
  volatile U32 *sdram = (void *) BOARD_SDRAM_BASE;
  U32 i;

  sdramc_enable_muxed_pins();

  // Setup SDRAM controller
  sdramc->cr = ( (SDRAM_COL_BITS - 8) 	<< AVR32_SDRAMC_CR_NC ) |
			   ( (SDRAM_ROW_BITS - 11) 	<< AVR32_SDRAMC_CR_NR ) |
			   ( (SDRAM_BANK_BITS - 1) 	<< AVR32_SDRAMC_CR_NB ) |
			   ( 			SDRAM_CAS 	<< AVR32_SDRAMC_CR_CAS ) |
			   ( 			SDRAM_TWR 	<< AVR32_SDRAMC_CR_TWR ) |
			   ( 			SDRAM_TRC 	<< AVR32_SDRAMC_CR_TRC ) |
			   ( 			SDRAM_TRP 	<< AVR32_SDRAMC_CR_TRP ) |
			   ( 			SDRAM_TRCD 	<< AVR32_SDRAMC_CR_TRCD ) |
			   (		 	SDRAM_TRAS 	<< AVR32_SDRAMC_CR_TRAS ) |
			   ( 			SDRAM_TXSR 	<< AVR32_SDRAMC_CR_TXSR ) |
			   (	   				1  	<< AVR32_SDRAMC_CR_DBW_OFFSET);

  //sdram_delay(200);
  delay_us(400);

  // Precharge All command is issued to the SDRAM
  sdramc->mr = AVR32_SDRAMC_MR_MODE_BANKS_PRECHARGE;
  sdramc->mr;
  sdram[0] = 0;

  // Provide eight auto-refresh (CBR) cycles
  sdramc->mr = AVR32_SDRAMC_MR_MODE_AUTO_REFRESH;
  sdramc->mr;
  for (i = 0; i < 8; i++)
    sdram[0] = 0;

  // A mode register set (MRS) cycle is issued to program
  // SDRAM parameters, in particular CAS latency and burst
  // length.

  // CAS from info struct, burst length 1, serial burst type
  sdramc->mr = AVR32_SDRAMC_MR_MODE_LOAD_MODE;
  sdramc->mr;
  sdram[0x020] = 0;

  // A Normal Mode command is provided, 3 clocks after tMRD is met.
  sdramc->mr;
  sdramc->mr = AVR32_SDRAMC_MR_MODE_NORMAL;
  sdramc->mr;
  sdram[0] = 0;

  // Write refresh rate into SDRAMC refresh timer count register
  // sdramc->tr = ( ( 156 * (hsb_hz / 1000) ) / 10000 );

  sdramc_refresh_rate_update();

}


void  sdramc_refresh_rate_update (void)
{
    uint32_t  hsb_freq;

    //hsb_freq = Get_HSB_Clk();
    hsb_freq = pm_read_module_freq_hz(PM_PBB_HSDRAMC);

    hsb_freq        /=  1000;
    AVR32_SDRAMC.tr  = (15625 * hsb_freq) / 1000000;                    /* write SDRAM refresh time for 15.625us                    */
}


