#ifndef PLAYER_H_
#define PLAYER_H_

/************************************************************************
 title:		Player
 file:     	player.h
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


#include"enemy.h"

/**@{*/

/**
 * @brief Struct that defines the player.
 */
typedef struct{
	/** @brief pointer to animation set for the enemy */
	AnimationSet_t* animationSet;
	/** @brief current x coordinate of player */
	uint16_t x_coord;
	/** @brief current y coordinate of player */
	uint16_t y_coord;
}Player_t;

#define PLAYER_WIDTH  34
#define PLAYER_HEIGHT 20
#define KISS_WIDTH    72
#define KISS_HEIGHT   22
#define PLAYER_STEP_LEFT  10
#define PLAYER_STEP_RIGHT 10

extern const uint8_t player_ap1 []; /**< first animation phase of player */
extern const uint8_t victory_ap1 []; /**< first animation phase of victory*/

/**@}*/

#endif /*PLAYER_H_*/
