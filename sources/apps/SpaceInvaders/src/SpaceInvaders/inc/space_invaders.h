#ifndef  _SPACE_INVADERS_H
#define _SPACE_INVADERS_H


/************************************************************************
 title:		Space Invaders
 file:     	space_invaders.h
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
#include "hgdi_api.h"

/**@{*/

//SCREEN
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272

//ENEMIES
#define NUM_OF_ENEMIES 55
#define NUM_ENEMIES_PER_ROW 11
#define NUM_OF_ANIMATION_SETS 3

//SWARM DEFINES
#define MOVE_TO_RIGHT 0
#define MOVE_TO_LEFT  1
#define SWARM_TICKS_TILL_MOVE 4000
#define XSTEP 5
#define YSTEP 4

//OTHERS
#define KILLED 1
#define EXPLOSION_TICKS 10
#define TRUE  1
#define FALSE 0



/**
 * @brief initializes the game
 */
extern void game_init();


/**
 * @brief draws the swarm
 * @param c color of the swarm
 */
extern void draw_swarm(int c);

/**
 * @brief draws the mothership
 * @param c color of the swarm
 */
extern void draw_mothership(color_t c);

/**
 * @brief updates the swarm (movement)
 */
extern void update_swarm();


/**
 * @brief draws a shoot
 * @param c the color of the shoot
 */
extern void draw_shoot(int c);


/**
 * @brief checks if a point lies in a rectangle
 * @param  x the x coordinate of the point
 * @param  y the y coordinate of the point
 * @param x1_r the upper left x coordinate of the rectangle
 * @param y1_r the upper left y coordinate of the rectangle
 * @param width  the width of the rectangle
 * @param height the height of the rectangle
 */
uint8_t in_rectangle(int16_t x, int16_t y, int16_t x1_r, int16_t y1_r, int16_t width, int16_t height);


/**
 * @brief checks if an enemy was hit
 */
uint8_t check_hit();


/**
 * @brief updates the shoot position
 */
void update_shoot();


/**
 * @brief draws the player
 * @param c the color of the player
 */
void draw_player(int c);


/**
 * @brief keeps the game alive
 */
extern uint8_t game_step();

extern void draw_sprites();

/**
 * @brief draws the the score and other information
 */
extern void draw_hud();

/**
 * @brief updates the player score
 * @param row the row the enemy was killed in
 */
extern void update_score(uint32_t row);

/**
 * @brief converts integer to ascii
 * @param i the integer that should be converted to ascii
 */
extern char* itoa(int32_t i);

/**
 *@brief moves the player to the left
 */
extern void player_move_left();

/**
 *@brief moves the player to the right
 */
extern void player_move_right();

/**
 *@brief triggers player shoot
 */
extern void player_shoot();


/**
 *@brief checks if game over condition is reached
 */
void check_game_over();

/**@}*/

#endif
