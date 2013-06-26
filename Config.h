#ifndef _CONFIG_H
#define _CONFIG_H

#define FPS 60

#if PSP
#define RATIO 0.5f
#define RES_WIDTH 480
#define RES_HEIGHT 272
#else
#define RES_WIDTH 912
#define RES_HEIGHT 672
#define RATIO 1.0f
#endif

#define DEBUG_COLLIDER false

enum GameState{
	GAME_INIT,
	GAME_LOAD_MENU,
	GAME_MENU,
	GAME_LOAD_GAME,
	GAME_INGAME,
	GAME_LOAD_SURVIVAL,
	GAME_SURVIVAL,
	GAME_OPTIONS,
	GAME_CREDITS
};


#endif
