
#ifndef TITLE_H_
#define TITLE_H_

/************************************************************************
 title:		Title
 file:     	title.h
 sw: 		AVR32-GCC
 hw: 		NGW100
 license:  	copyright(c) 2009 team halos
 usage:    	see doxygen manual

 LICENSE:
 This program is a free software;
 ************************************************************************/

/**
 *  @defgroup space_invaders Space Invaders
 *  @brief Space Invaders Game
 *
 *  @note ...
 *  @author team halos
 *  @version 0.0.0.1
 */


#include <stdint.h>


/**@{*/

/**
 * @brief draws the splash screen
 */
extern void draw_splash();

#define TITLE_WIDTH   198
#define TITLE_HEIGHT  86
#define LEFT_SHIFT 30

/**@}*/

#endif /* TITLE_H_ */
