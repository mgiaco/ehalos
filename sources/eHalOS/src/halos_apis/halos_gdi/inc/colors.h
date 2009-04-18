/**
 * @file colors.h
 * @brief colortable
 * @author Team eHalOS
 * @version 0.0.1
 * @date    2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 * @license LGPLv2.1
 */

#ifndef COLORS_H_
#define COLORS_H_

#if (COLORS == BPP_24)				// 24-bit colors
	#include "colors24.h"
#elif (COLORS == BPP_16)			// 16-bit colors
	#include "colors16.h"
#elif (COLORS == BPP_8)				//  8-bit colors
	#include "colors8.h"
#endif

#endif /*COLORS_H_*/
