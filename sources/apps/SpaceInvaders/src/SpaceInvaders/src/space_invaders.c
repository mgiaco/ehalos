

#include "space_invaders.h"
#include "enemy.h"
#include "player.h"
#include "shoot.h"
#include "hgdi_api.h"
#include <stdint.h>



uint32_t game_time;

//Alienswarm
Enemy_t enemies[NUM_OF_ENEMIES];
AnimationSet_t animationSets[NUM_OF_ANIMATION_SETS] ;

uint8_t swarm_direction = MOVE_TO_RIGHT;
int16_t swarm_x_offset = 0;
int16_t swarm_y_offset = 60;
uint16_t swarm_ticks = 0;
uint32_t score = 0;
uint8_t enemies_killed = 0;

uint16_t x_step = XSTEP;
uint16_t y_step = YSTEP;

uint8_t mothership_in_action = 0;
uint16_t mothership_step = MOTHERSHIP_STEP;
uint16_t mothership_ticks = 0;


//Mothership
Enemy_t mothership;
AnimationSet_t mothershipAnim;

//Player
Player_t player;
AnimationSet_t playerAnim;

//Shoot
Shoot_t shoot;

//lock input
uint8_t lock_input = 0;

//flag if game is finished;
uint8_t game_running = 1;


void game_init(){
	//initGraph();
    //setDevice(0);


	swarm_direction = MOVE_TO_RIGHT;
	swarm_x_offset = 0;
	swarm_y_offset = 60;
	swarm_ticks = 0;
	score = 0;
	enemies_killed = 0;
	game_running = 1;

	x_step = XSTEP;
	y_step = YSTEP;

	mothership_in_action = 0;
	mothership_step = MOTHERSHIP_STEP;
	mothership_ticks = 0;

	gdi_open(0,0);

    int i=0;
    //initialize first enemy row


    /*************** define animation phases of first enemy *************/

    //animation phase 1
    animationSets[ENEMY1].animationPhases[0].sprite.width=ENEMY1_WIDTH;
    animationSets[ENEMY1].animationPhases[0].sprite.height=ENEMY1_HEIGHT;
    animationSets[ENEMY1].animationPhases[0].sprite.sprite_data = (uint8_t*)enemy1_ap1;
    animationSets[ENEMY1].animationPhases[0].duration = 200;
    animationSets[ENEMY1].animationPhases[0].anim_ticks = 0;

    //animation phase 2
    animationSets[ENEMY1].animationPhases[1].sprite.width=20;
    animationSets[ENEMY1].animationPhases[1].sprite.height=20;
    animationSets[ENEMY1].animationPhases[1].sprite.sprite_data = (uint8_t*)enemy1_ap2;
    animationSets[ENEMY1].animationPhases[1].duration = 200;
    animationSets[ENEMY1].animationPhases[1].anim_ticks = 0;

    animationSets[ENEMY1].numberOfAnimationPhases = 2;
    animationSets[ENEMY1].currentAnimationPhase = 0;



    /*************** define animation phases of second enemy *************/

    //animation phase 1
    animationSets[ENEMY2].animationPhases[0].sprite.width=ENEMY2_WIDTH;
    animationSets[ENEMY2].animationPhases[0].sprite.height=ENEMY2_HEIGHT;
    animationSets[ENEMY2].animationPhases[0].sprite.sprite_data = (uint8_t*)enemy2_ap1;
    animationSets[ENEMY2].animationPhases[0].duration = 200;
    animationSets[ENEMY2].animationPhases[0].anim_ticks = 0;

    //animation phase 2
    animationSets[ENEMY2].animationPhases[1].sprite.width=ENEMY2_WIDTH;
    animationSets[ENEMY2].animationPhases[1].sprite.height=ENEMY2_HEIGHT;
    animationSets[ENEMY2].animationPhases[1].sprite.sprite_data = (uint8_t*)enemy2_ap2;
    animationSets[ENEMY2].animationPhases[1].duration = 200;
    animationSets[ENEMY2].animationPhases[1].anim_ticks = 0;

    animationSets[ENEMY2].numberOfAnimationPhases = 2;
    animationSets[ENEMY2].currentAnimationPhase = 0;


    /*************** define animation phases of third enemy  *************/

   //animation phase 1
   animationSets[ENEMY3].animationPhases[0].sprite.width=ENEMY3_WIDTH;
   animationSets[ENEMY3].animationPhases[0].sprite.height=ENEMY3_HEIGHT;
   animationSets[ENEMY3].animationPhases[0].sprite.sprite_data = (uint8_t*)enemy3_ap1;
   animationSets[ENEMY3].animationPhases[0].duration = 200;
   animationSets[ENEMY3].animationPhases[0].anim_ticks = 0;

   //animation phase 2
   animationSets[ENEMY3].animationPhases[1].sprite.width=ENEMY3_WIDTH;
   animationSets[ENEMY3].animationPhases[1].sprite.height=ENEMY3_HEIGHT;
   animationSets[ENEMY3].animationPhases[1].sprite.sprite_data = (uint8_t*)enemy3_ap2;
   animationSets[ENEMY3].animationPhases[1].duration = 200;
   animationSets[ENEMY3].animationPhases[1].anim_ticks = 0;

   animationSets[ENEMY3].numberOfAnimationPhases = 2;
   animationSets[ENEMY3].currentAnimationPhase = 0;


   //place enemies in virtual grid and itialize them
   for(i=0; i < 11; i++){
	   enemies[i].animationSet = &animationSets[ENEMY1];
	   enemies[i].killed = 0;
	   enemies[i].deleted = 0;
	   enemies[i].ticks = 0;

	   enemies[NUM_ENEMIES_PER_ROW + i].animationSet = &animationSets[ENEMY2];
	   enemies[NUM_ENEMIES_PER_ROW + i].killed = 0;
	   enemies[NUM_ENEMIES_PER_ROW + i].deleted = 0;
	   enemies[NUM_ENEMIES_PER_ROW + i].ticks = 0;

	   enemies[NUM_ENEMIES_PER_ROW*2+i].animationSet = &animationSets[ENEMY2];
	   enemies[NUM_ENEMIES_PER_ROW*2+i].killed = 0;
	   enemies[NUM_ENEMIES_PER_ROW*2+i].deleted = 0;
	   enemies[NUM_ENEMIES_PER_ROW*2+i].ticks = 0;

	   enemies[NUM_ENEMIES_PER_ROW*3+i].animationSet = &animationSets[ENEMY3];
	   enemies[NUM_ENEMIES_PER_ROW*3+i].killed = 0;
	   enemies[NUM_ENEMIES_PER_ROW*3+i].deleted = 0;
	   enemies[NUM_ENEMIES_PER_ROW*3+i].ticks = 0;

	   enemies[NUM_ENEMIES_PER_ROW*4+i].animationSet = &animationSets[ENEMY3];
	   enemies[NUM_ENEMIES_PER_ROW*4+i].killed = 0;
	   enemies[NUM_ENEMIES_PER_ROW*4+i].deleted = 0;
	   enemies[NUM_ENEMIES_PER_ROW*4+i].ticks = 0;
   }


   //mothership

   mothershipAnim.animationPhases[0].sprite.width=ENEMY4_WIDTH;
   mothershipAnim.animationPhases[0].sprite.height=ENEMY4_HEIGHT;
   mothershipAnim.animationPhases[0].sprite.sprite_data = (uint8_t*)enemy4_ap1;
   mothershipAnim.animationPhases[0].duration = 200;
   mothershipAnim.animationPhases[0].anim_ticks = 0;

   mothership.animationSet = &mothershipAnim;
   mothership.y_coord = 20;
   mothership.x_coord = 30;
   mothership.killed = 0;
   mothership.deleted = 0;

   //player

   playerAnim.animationPhases[0].sprite.width=PLAYER_WIDTH;
   playerAnim.animationPhases[0].sprite.height=PLAYER_HEIGHT;
   playerAnim.animationPhases[0].sprite.sprite_data = (uint8_t*)player_ap1;
   playerAnim.animationPhases[0].duration = 200;
   playerAnim.animationPhases[0].anim_ticks = 0;

   player.animationSet = &playerAnim;
   player.x_coord = SCREEN_WIDTH / 2;
   player.y_coord = SCREEN_HEIGHT - PLAYER_HEIGHT - 4;

   //shoot

   shoot.charged = 1;

   lock_input = 0;
}


void draw_swarm(int c){
	int32_t i=0;
	int32_t color = 255;
	gdi_sprite_t gdi_spr;

	char* p;

	if(c == 0){
		color = 0;
	}

	//draw all enemies in swarm
	for(i=0; i < NUM_OF_ENEMIES; i++){


		Enemy_t* enemy = &enemies[i];

		//retrieve the sprite data of the current animation phase
		uint8_t* data =  enemy->animationSet->animationPhases
				[enemy->animationSet->currentAnimationPhase].sprite.sprite_data;

		//calculate the x an y position of the enemy in the swarm
		uint16_t x = i % NUM_ENEMIES_PER_ROW;
		uint16_t y = i / NUM_ENEMIES_PER_ROW;
		x = x * 28 + x * 4;
		y = y * 20 + y * 4;


		//initialize the sprite
		gdi_spr.x = swarm_x_offset + x;
		gdi_spr.y = swarm_y_offset + y;
		gdi_spr.width  = 28;
		gdi_spr.height = 20;
		gdi_spr.data = data;


		//check if the alien eats the player
		if(gdi_spr.y + 20 >= player.y_coord &&
		  gdi_spr.y + 20 <= player.y_coord + PLAYER_HEIGHT &&
		  gdi_spr.x + 14 >= player.x_coord &&
		  gdi_spr.x + 14 <= player.x_coord + PLAYER_WIDTH &&
		  enemy->killed == 0)
		{
			//draw game over
			lock_input = 1;

			set_color(BLACK);
			clr_scr(1);

			select_font(ARIAL_BOLD_14);
			set_color(RED);

			goto_xy(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			set_text_justify(1,1);
			draw_string("You were killed by the aliens! Try again.");
			goto_xy(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 30);
			p = itoa(score);
			set_color(YELLOW);
			draw_string("Your Score: ");
			goto_xy(SCREEN_WIDTH / 2+100, SCREEN_HEIGHT / 2 + 30);
			draw_string(p);


			//insert a little break,
			for(i=0; i<1500000; i++){}

			game_running = 0;

			break;

		}


		//check if enemy was killed before but not yet deleted -> draw explosion
		if(enemy->killed == KILLED && !enemy->deleted){

			//erase the dead enemy from screen
			if(enemy->ticks == 0){
				set_color_rgb(0,0,0);
				draw_sprite2(&gdi_spr);
			}

			enemy->ticks = enemy->ticks + 1;

			//check if the explosion is finished
			if(enemy->ticks >= EXPLOSION_TICKS){
				enemy->deleted = TRUE;
				gdi_spr.data = (uint8_t*)explosion_ap1;
				set_color_rgb(0, 0 ,0);
				draw_sprite2(&gdi_spr);

			}else{
				gdi_spr.data = (uint8_t*)explosion_ap1;
				set_color_rgb(color, 0, 0);
				draw_sprite2(&gdi_spr);
			}
		}
		else if(!enemy->deleted){
			set_color_rgb(0, color ,0);
			//clear enemy
			draw_sprite2(&gdi_spr);

		}
	}

}

void draw_mothership(color_t c){


	gdi_sprite_t mothership_spr;

	mothership_spr.x = mothership.x_coord;
	mothership_spr.y = mothership.y_coord;
	mothership_spr.width = ENEMY4_WIDTH;
	mothership_spr.height = ENEMY4_HEIGHT;

	if(!mothership.killed){
		mothership_spr.data = mothership.animationSet[0].animationPhases[0].sprite.sprite_data;
	}else{
		mothership_spr.width = 28;
		mothership_spr.height = 20;
		mothership_spr.data = (uint8_t*)explosion_ap1;
	}
	set_color(c);
	if(mothership.killed){
		draw_sprite2(&mothership_spr);

	}else{
		if(c == BLACK){
			fill_rectangle(mothership_spr.x, mothership_spr.y, mothership_spr.x+ ENEMY4_WIDTH, mothership_spr.y+ENEMY4_HEIGHT);
		}else{
			draw_color_sprite(&mothership_spr);
		}
	}


}

void update_swarm(){

	int i=0;

	swarm_ticks++;


	//process swarm

	if(swarm_ticks >= SWARM_TICKS_TILL_MOVE){
		swarm_ticks =0;

		//erase the swarm from screen
		draw_swarm(0);

		//check if swarm move to right side
		if(swarm_direction == MOVE_TO_RIGHT){
			swarm_x_offset+=x_step;

			//check if swarm is to far right
			if(swarm_x_offset+ 352 >= SCREEN_WIDTH){
				swarm_direction = MOVE_TO_LEFT;

			swarm_y_offset += y_step;
			}
		}else{
			swarm_x_offset-=x_step;

			//check if swarm is to far left
			if(swarm_x_offset<= 0){
				swarm_direction = MOVE_TO_RIGHT;
				swarm_y_offset += y_step;
			}
		}

		//update the animation sets
		for(i=0; i < NUM_OF_ANIMATION_SETS; i++){

			animationSets[i].currentAnimationPhase = (animationSets[i].currentAnimationPhase + 1) %
					animationSets[i].numberOfAnimationPhases;

		}

		//if the swarm gets deeper, start mothership
		if(swarm_y_offset >= 65){
			mothership_in_action = 1;
		}


		//draw the swarm
		draw_swarm(1);

	}

	//check if mothership is flying

	if(mothership_in_action && !mothership.deleted){
		mothership_ticks++;

		if(mothership_ticks >= MOTHERSHIP_TICKS_TILL_MOVE){

			//check if mothership was killed
			if(mothership.killed){
				mothership.ticks++;

				if(mothership.ticks >= 20){
					mothership.deleted = 1;
					draw_mothership(BLACK);
				}
			}

			//check if mothership was deleted
			if(!mothership.deleted){
				draw_mothership(BLACK);
				mothership.x_coord = mothership.x_coord +  mothership_step;
				mothership_ticks=0;
				draw_mothership(RED);
			}

			if(mothership.x_coord >  SCREEN_WIDTH - ENEMY4_WIDTH-20 || mothership.x_coord < 20){
				mothership_step = -mothership_step;
			}
		}

	}
}


void draw_shoot(int c){
	uint32_t color = c;
	set_color(color);

	set_x_pos(shoot.x_shoot);
	set_y_pos(shoot.y_shoot);

	//draw the shoot sprite
	draw_sprite(shoot_1,4,5);

}

uint8_t in_rectangle(int16_t x, int16_t y, int16_t x1_r, int16_t y1_r, int16_t width, int16_t height){
	return (x >= x1_r && x <= x1_r+width &&
	   y >= y1_r && y <= y1_r+height);
}


uint8_t check_hit(){
	//int32_t i=0;
	int16_t offset_x = 0.0;
	int16_t offset_y = 0.0;
	int16_t rest_x = 0.0;
	int16_t rest_y = 0.0;
	uint8_t hitEnemyIndexX = 0;
	uint8_t hitEnemyIndexY = 0;
	uint8_t indexInArray =0;
	uint8_t hit = 0;

	//test if shoot his in enemy swarm grid
	if(shoot.x_shoot > swarm_x_offset &&
	   shoot.x_shoot < swarm_x_offset + 11 * (ENEMY1_WIDTH+4) &&
	   shoot.y_shoot > swarm_y_offset &&
	   shoot.y_shoot < swarm_y_offset + 5 * (ENEMY1_HEIGHT+4)){


		//calc the potential enemy that could be hit
		offset_x = shoot.x_shoot - swarm_x_offset;
		offset_y = shoot.y_shoot - swarm_y_offset;

		hitEnemyIndexX = offset_x  / (ENEMY1_WIDTH+4);
		rest_x = offset_x  % (ENEMY1_WIDTH+4);
		hitEnemyIndexY = offset_y  / (ENEMY1_HEIGHT+4);
		rest_y =  offset_y  % (ENEMY1_HEIGHT+4);

		indexInArray = hitEnemyIndexY * 11 + hitEnemyIndexX;


		if(indexInArray < 11){
			 if(rest_x > 6 && rest_x < 21){
				 hit = 1;
			 }

		}else if(indexInArray < 22){
			if(rest_x > 2 && rest_x < 24){
				hit = 1;
			}

		}else{
			if(rest_x > 2 && rest_x < 24){
				hit = 1;
			}

		}

		//kill the enmy if hit and not killed bevore
		if(enemies[indexInArray].killed == 0 && hit == 1){
			enemies[indexInArray].killed = 1;
			shoot.charged = 1;

			enemies_killed++;
			update_score(indexInArray);

			check_game_over();

		}
	}

	//check mothership

	if(shoot.y_shoot >= mothership.y_coord  &&
	   shoot.y_shoot <= mothership.y_coord + ENEMY4_HEIGHT &&
	   shoot.x_shoot >= mothership.x_coord &&
	   shoot.x_shoot <= mothership.x_coord + ENEMY4_WIDTH &&
	   !mothership.killed &&
	    mothership_in_action){

		draw_mothership(0);
		mothership.killed = 1;
		update_score(55);
	}

	return hit;
}


void check_game_over(){
	char *p = 0;
	int i = 0;
	gdi_sprite_t kiss_spr;

	//check if all enemies where killed
	if(enemies_killed == NUM_OF_ENEMIES){

		//draw game over
		lock_input = 1;

		set_color(BLACK);
		clr_scr(1);


		select_font(ARIAL_BOLD_14);
		set_color(RED);

		goto_xy(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		set_text_justify(1,1);
		draw_string("You have killed all aliens! Good boy");
		goto_xy(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 30);
		p = itoa(score);
		set_color(YELLOW);
		draw_string("Your Score: ");
		goto_xy(SCREEN_WIDTH / 2+60, SCREEN_HEIGHT / 2 + 30);
		draw_string(p);

		set_color(RED);
		kiss_spr.x = (SCREEN_WIDTH - KISS_WIDTH) / 2;
		kiss_spr.y =  70;
		kiss_spr.width  = KISS_WIDTH;
		kiss_spr.height = KISS_HEIGHT;
		kiss_spr.data   = (uint8_t*)victory_ap1;

		set_color(RED);
		draw_sprite2(&kiss_spr);


		for(i=0; i<1500000; i++){}

		game_running = 0;

	}

}


void update_shoot(){
	if(shoot.charged == 0){

		draw_shoot(BLACK);
		shoot.y_shoot -= SHOOT_VELOCITY;

		if(shoot.y_shoot < -5){
			shoot.charged = 1;
		}

		draw_shoot(WHITE);

		if(check_hit()){
			draw_shoot(BLACK);
		}
	}
}


void draw_player(int c){
	gdi_sprite_t gdi_spr;

	uint8_t* data =  player.animationSet->animationPhases
				[player.animationSet->currentAnimationPhase].sprite.sprite_data;


	gdi_spr.x = player.x_coord;
	gdi_spr.y = player.y_coord;
	gdi_spr.width = PLAYER_WIDTH;
	gdi_spr.height = PLAYER_HEIGHT;
	gdi_spr.data = data;

	set_color_rgb(0,0,c == 1 ? 255 : 0);
	draw_sprite2(&gdi_spr);

}


void draw_hud(uint32_t color){
	char* buff;

	select_font(ARIAL_BOLD_14);
	set_color(color);

	set_text_justify(0,0);
	goto_xy(30,10);
	draw_string("SCORE");

	buff =itoa(score);

	set_text_justify(0,0);
	goto_xy(100, 10);
	draw_string(buff);

}

char* itoa(int32_t i)
{
	static char a[255];

	char *b = a + sizeof(a) - 1;
	uint8_t   sign = (i < 0);

	if (sign) i = -i;
	*b = 0;
	do {
		*--b = '0' + (i % 10);
		i /= 10;
	} while (i);
	if (sign) *--b = '-';
	return b;
}

void update_score(uint32_t index){

	draw_hud(BLACK);

	//calc score that killed enemy is worth
	if(index < 11){
		score = score + 40;
	}else if(index < 33){
		score = score + 20;
	}else if(index < 55){
		score = score + 10;
	}else{
		//mothership
		score = score + 100;
	}

	if(enemies_killed >= 10){
		y_step = 10;
	}else if(enemies_killed >=20){
		y_step = 15;
	}else if(enemies_killed >=30){
		y_step = 18;
	}

	draw_hud(WHITE);

}

uint8_t game_step(){
	update_swarm();
	update_shoot();

	return game_running;
}


void player_move_left(){

	if(lock_input){
		return;
	}

	draw_player(0);


	if(player.x_coord >= PLAYER_STEP_LEFT){
		player.x_coord-=PLAYER_STEP_LEFT;
	}
	draw_player(1);
}


void player_move_right(){

	if(lock_input){
		return;
	}

	draw_player(0);
	if(player.x_coord <= SCREEN_WIDTH - PLAYER_WIDTH - PLAYER_STEP_RIGHT){
		player.x_coord+=PLAYER_STEP_RIGHT;
	}
	draw_player(1);
}


void player_shoot(){

	if(lock_input){
		return;
	}

	if(shoot.charged != 0){
		shoot.x_shoot = player.x_coord + PLAYER_WIDTH / 2;
		shoot.y_shoot = player.y_coord;
		shoot.charged = 0;
	}
}

/* end of space_invaders.c */

