#ifndef  _ENEMY_H
#define  _ENEMY_H

/************************************************************************
 title:		Enemy
 file:     	enemy.h
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

#define MAX_ANIMATION_PHASES 10 	/**< max animation phases per enemy */

/**
 * @brief Struct that defines sprite information. This is not gdi_sprite.
 */
typedef struct
{
	/** @brief width of the sprite */
	uint8_t width;
	/** @brief height of the sprite */
	uint8_t height;
	/** @brief color of the sprite */
	uint16_t color;
	/** @brief array that stores the actual sprite data. There must be width*height data values */
	uint8_t* sprite_data;

}Sprite_t;


/**
 * @brief Struct that defines a animation phase (step) of an enemy.
 */
typedef struct{
	/** @brief stores the sprite data for the animation phase */
	Sprite_t sprite;
	/** @brief relative duration that this animation phase will take */
	uint16_t duration;
	/** @brief counter for time ticks */
	uint16_t anim_ticks;

}AnimationPhase_t;

/**
 * @brief Struct that defines a set of animation phases.
 */
typedef struct
{
	/** @brief array that stores animation phases  */
	AnimationPhase_t animationPhases[MAX_ANIMATION_PHASES];

	/** @brief number of animation phases in this set  */
	uint8_t numberOfAnimationPhases;
	/** @brief stores the zero bases index of the current animation step */
	uint8_t currentAnimationPhase;

}AnimationSet_t;

/**
 * @brief Struct that defines an enemy.
 */
typedef struct{
	/** @brief pointer to animation set for the enemy */
	AnimationSet_t* animationSet;
	/** @brief current x coordinate of the enemy */
	uint16_t x_coord;
	/** @brief current y coordinate of the enemy*/
	uint16_t y_coord;
	/** @brief flag that stores, if enemy was killed (1==killed, 0=not killed) */
	uint8_t killed;
	/** @brief flag that stores, if enemy was deleted from game */
	uint8_t deleted;
	/** @brief helper variable for timing purposes*/
	uint32_t ticks;
}Enemy_t;


#define ENEMY1 0
#define ENEMY2 1
#define ENEMY3 2

#define ENEMY1_WIDTH 28
#define ENEMY1_HEIGHT 20

#define ENEMY2_WIDTH 28
#define ENEMY2_HEIGHT 20

#define ENEMY3_WIDTH 28
#define ENEMY3_HEIGHT 20

#define ENEMY4_WIDTH 78//34
#define ENEMY4_HEIGHT 45 //20

#define MOTHERSHIP_STEP 10
#define MOTHERSHIP_TICKS_TILL_MOVE 2000


extern const uint8_t enemy1_ap1 []; /**< first animation phase of enemy 1 */
extern const uint8_t enemy1_ap2 []; /**< second animation phase of enemy 1 */

extern const uint8_t enemy2_ap1 []; /**< first animation phase of enemy 2 */
extern const uint8_t enemy2_ap2 []; /**< second animation phase of enemy 2 */

extern const uint8_t enemy3_ap1 []; /**< first animation phase of enemy 3 */
extern const uint8_t enemy3_ap2 []; /**< second animation phase of enemy 3 */

extern const uint8_t enemy4_ap1 []; /**< first animation phase of enemy 4 */

extern const uint8_t explosion_ap1 []; /**< first animation phase of explosion */

/**@}*/

#endif
