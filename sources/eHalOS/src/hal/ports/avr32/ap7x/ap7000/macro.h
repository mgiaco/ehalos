/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief MMU example application.
 *
 * This file gives an example of using the MMU to manage the memory
 * on AVR32 MCUs.
 *
 * - Compiler:           IAR EWAAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with MMU.
 * - AppNote:            AVR32113 Configuration and Use of the Memory
 *			 Management Unit
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support email: avr32@atmel.com
 *
 * $Name$
 * $Revision: 679 $
 * $RCSfile$
 * $Date: 2007-10-12 09:44:30 +0200 (Fri, 12 Oct 2007) $
 *****************************************************************************/

/* Copyright (c) 2006, Atmel Corporation All rights reserved.
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

#ifndef _MACRO_H_
#define _MACRO_H_

#ifdef __ICCAVR32__
#include <intrinsics.h>
#endif

/*! \def AVR32_SET_SR_BIT(sregister, bitname)
 *
 *  \brief define which sets a bit in system registers
 *
 *  \param sregister address of the system register to modify
 *  \param bitname offset to the bit which is to be set
 */
#ifdef __ICCAVR32__
#define AVR32_SET_SR_BIT(sregister, bitname) \
	volatile long avr32_sr_set_value; \
	avr32_sr_set_value = __get_system_register(sregister); \
	avr32_sr_set_value |= bitname; \
	__set_system_register(sregister, avr32_sr_set_value)
#elif __GNUC__
#define AVR32_SET_SR_BIT(sregister, bitname) \
	volatile long avr32_sr_set_value; \
	avr32_sr_set_value = __builtin_mfsr(sregister); \
	avr32_sr_set_value |= bitname; \
	__builtin_mtsr(sregister, avr32_sr_set_value)
#else
#error No known compiler used
#endif


/*! \def AVR32_SET_SR_REG(sregister, regval)
 *
 *  \brief define sets an entire system register
 *
 *  \param sregister address of the system register to modify
 *  \param regval the register value to set in \param sregister
 */
#ifdef __ICCAVR32__
#define AVR32_SET_SR_REG(sregister, regval) \
	__set_system_register(sregister, regval)
#elif __GNUC__
#define AVR32_SET_SR_REG(sregister, regval) \
	__builtin_mtsr(sregister, regval)
#else
#error No known compiler used
#endif


/*! \def AVR32_CLEAR_SR_BIT(sregister, bitname)
 *
 *  \brief Define which clears a bit in system registers.
 *
 *  \param sregister address of the system register to modify
 *  \param bitname offset to the bit which is to be cleared
 */
#ifdef __ICCAVR32__
#define AVR32_CLEAR_SR_BIT(sregister, bitname) \
	volatile long avr32_sr_clear_value; \
	avr32_sr_clear_value = __get_system_register(sregister); \
	avr32_sr_clear_value &= ~bitname; \
	__set_system_register(sregister, avr32_sr_clear_value)
#elif __GNUC__
#define AVR32_CLEAR_SR_BIT(sregister, bitname) \
	volatile long avr32_sr_clear_value; \
	avr32_sr_clear_value = __builtin_mfsr(sregister); \
	avr32_sr_clear_value &= ~bitname; \
	__builtin_mtsr(sregister, avr32_sr_clear_value)
#else
#error No known compiler used
#endif


/*! \def AVR32_TOGGLE_SR_BIT(sregister, bitname)
 *
 *  \brief Define which toggles a bit in system registers.
 *
 *  \param sregister address of the system register to modify
 *  \param bitname offset to the bit which is to be toggled
 */
#ifdef __ICCAVR32__
#define AVR32_TOGGLE_SR_BIT(sregister, bitname) \
	volatile long avr32_sr_toggle_value; \
	avr32_sr_toggle_value = __get_system_register(sregister); \
	avr32_sr_toggle_value ^= bitname; \
	__set_system_register(sregister, avr32_sr_toggle_value)
#elif __GNUC__
#define AVR32_TOGGLE_SR_BIT(sregister, bitname) \
	volatile long avr32_sr_toggle_value; \
	avr32_sr_toggle_value = __builtin_mfsr(sregister); \
	avr32_sr_toggle_value ^= bitname; \
	__builtin_mtsr(sregister, avr32_sr_toggle_value)
#else
#error No known compiler used
#endif


/*! \def AVR32_READ_SR_REG(sregister, return_value)
 *
 *  \brief Define to read a given system registers.
 *
 *  \param sregister address of the system register to modify
 *  \param return_value the variable to return the register value to
 */
#ifdef __ICCAVR32__
#define AVR32_READ_SR_REG(sregister, return_value) \
	return_value = __get_system_register(sregister)
#elif __GNUC__
#define AVR32_READ_SR_REG(sregister, return_value) \
	return_value = __builtin_mfsr(sregister)
#else
#error No known compiler used
#endif

#endif /* End _MACRO_H_ */

