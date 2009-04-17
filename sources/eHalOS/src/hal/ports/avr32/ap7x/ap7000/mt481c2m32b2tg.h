/* This header file is part of the ATMEL AVR32-SoftwareFramework-AT32AP7000-1.0.0 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief MT481C2M32B2TG SDRAM driver for AVR32 SDRAMC on EBI.
 *
 * \note The values defined in this file are device-specific. See the device
 *       datasheet for further information.
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


#ifndef _MT481C2M32B2TG_H_
#define _MT481C2M32B2TG_H_


/*
These values are device specific. Consult the datasheet for
the given device for more information
*/

/*!
 * The number of columns for this sdram.
 */
// number of cols = 256  ->  8 col bits
#define SDRAM_COL_BITS  9

/*!
 * The number of rows for this sdram.
 */
// number of rows = 2048 -> 11 row bits
#define SDRAM_ROW_BITS  13

/*!
 * The number of banks for this sdram.
 */
// number of banks = 4   ->  2 bank bits
#define SDRAM_BANK_BITS 2


/*!
 * The refresh rate. NOTE: This is dependant on the clock frequency
 */
// Refresh rate
// Use 15.6 uS = 15600 ns.
// With cpu hz 20 MHz = 50 ns clock cycle :
// Refresh rate = 15600 ns / 50 ns = 312 clock cycles
// Generic formula for chip (156 * (CPU_HZ / 1000) ) / 10000
#define SDRAM_TR 312


/*!
 * The CAS latency for this sdram.
 */
// CAS Latency: 3
// Should be 3 for Speed grade -5 part (up to 200Mhz)
#define SDRAM_CAS 3

/*!
 * The write recovery time. NOTE: This setting is dependant on the clock frequency
 */
// Min: 2ck
#define SDRAM_TWR 2

/*!
 * Minimal ACTIVE to ACTIVE command period. NOTE: This setting is dependant on the clock frequency
 */

// Min 55 ns -> 55 / 50 ~=  2
#define SDRAM_TRC 7

/*!
 * Minimal PRECHARGE command period. NOTE: This setting is dependant on the clock frequency
 */
// 15 ns -> 15 / 50 ~= 1
#define SDRAM_TRP 2
/*!
 * Minimal ACTIVE to READ or WRITE delay. NOTE: This setting is dependant on the clock frequency
 */
//15 ns -> 15 / 50 ~= 1
#define SDRAM_TRCD 2

/*!
 * Minimal ACTIVE to PRECHARGE command. NOTE: This setting is dependant on the clock frequency.
 */
// 42 ns -> 42 / 50 ~= 1
#define SDRAM_TRAS 5

/*!
 *Minimal exit SELF REFREASH to ACTIVE command. NOTE: This setting is dependant on the clock frequency
 */
// 55 ns -> 55 / 50 ~= 2
#define SDRAM_TXSR 5

//! The minimal number of AUTO REFRESH commands required during initialization for this SDRAM.
#define SDRAM_INIT_AUTO_REFRESH_COUNT 8

//! The minimal refresh cycle time for this SDRAM.
//! AUTO REFRESH command period.
//! Unit: ns.
#define SDRAM_TRFC                      66

//! The minimal mode register delay time for this SDRAM.
//! LOAD MODE REGISTER command to ACTIVE or REFRESH command delay.
//! Unit: tCK (SDRAM cycle period).
#define SDRAM_TMRD                      2

//! The minimal stable-clock initialization delay for this SDRAM.
//! Unit: us.
#define SDRAM_STABLE_CLOCK_INIT_DELAY   100

#endif
