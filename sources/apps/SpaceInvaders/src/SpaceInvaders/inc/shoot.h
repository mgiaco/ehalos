#ifndef SHOOT_H_
#define SHOOT_H_

/************************************************************************
 title:		Shoot
 file:     	shoot.h
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


/**@{*/


/**
 * @brief Struct that defines the shoot of the player.
 */
typedef struct{
	/** @brief flag that stores, if weapon is charged (1==charged) */
	uint8_t  charged;
	/** @brief current x position of shoot */
	int16_t x_shoot;
	/** @brief current y position of shoot  */
	int16_t y_shoot;
}Shoot_t;

#define SHOOT_VELOCITY 1

extern uint8_t shoot_1[];  /**< first animation phase of enemy 1 */

/**@}*/

#endif /*SHOOT_H_*/
