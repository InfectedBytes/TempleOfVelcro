/* ----- Bank setup ----- */
#pragma bank=4
#include "StateIntroOutro.h"
UINT8 bank_STATE_INTRO = 4;
UINT8 bank_STATE_OUTRO = 4;


/* ----- Includes ----- */
#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Fade.h"
#include "../res/src/introMap1.h"
#include "../res/src/introMap2.h"
#include "../res/src/introMap3.h"
#include "../res/src/introTiles1.h"
#include "../res/src/introTiles2.h"
#include "../res/src/introTiles3.h"
#include "../res/src/outroMap1.h"
#include "../res/src/outroMap2.h"
#include "../res/src/outroMap3.h"
#include "../res/src/outroTiles1.h"
#include "../res/src/outroTiles2.h"
#include "../res/src/outroTiles3.h"


/* ----- Defines ----- */
#define MAX_SCREENS  3

/* Tileset parameters
 * 1. always zero
 * 2. number of tiles in that tileset
 * 3. the tileset lable XYZ (extern unsigned char XYZ[]), see tileset .h file
 * 4. tileset bank N, see "bN"-part in filename xyzTiles.bN.c
 */
#define INTRO_TILES_PARAMS_1   0, 128, introTiles1, 8
#define INTRO_TILES_PARAMS_2   0, 128, introTiles2, 8
#define INTRO_TILES_PARAMS_3   0, 128, introTiles3, 8

#define OUTRO_TILES_PARAMS_1   0, 128, outroTiles1, 9
#define OUTRO_TILES_PARAMS_2   0, 128, outroTiles2, 9
#define OUTRO_TILES_PARAMS_3   0, 128, outroTiles3, 9

/* Map parameters
 * 1. map width, use #define from map .h file
 * 2. map height, use #define from map .h file
 * 3. map label XYZ (extern unsigned char XYZ[]), see map .h file
 * 4. always zero
 * 5. always zero
 * 6. map bank N, see "bN"-part in filename xyzMap.bN.c
 */
#define INTRO_MAP_PARAMS_1     introMap1Width, introMap1Height, introMap1, 0, 0, 8
#define INTRO_MAP_PARAMS_2     introMap2Width, introMap2Height, introMap2, 0, 0, 8
#define INTRO_MAP_PARAMS_3     introMap3Width, introMap3Height, introMap3, 0, 0, 8

#define OUTRO_MAP_PARAMS_1     outroMap1Width, outroMap1Height, outroMap1, 0, 0, 9
#define OUTRO_MAP_PARAMS_2     outroMap2Width, outroMap2Height, outroMap2, 0, 0, 9
#define OUTRO_MAP_PARAMS_3     outroMap3Width, outroMap3Height, outroMap3, 0, 0, 9


/* ----- Types / Enums ----- */
typedef enum {
	IS_INTRO,
	IS_OUTRO
} IntroOrOutro;

/* ----- Prototypes ----- */
static void StateIntroOutro_HandleInput(void);
static void StateIntroOutro_LoadScreen(void);
static void StateIntroOutro_NextState(void);


/* ----- Variables ----- */
static IntroOrOutro selection;
static UINT8 screen = 0;
extern UINT8* menu_mod_Data[];


/* ----- Functions ----- */

void Start_STATE_INTRO(void) {
	/* load intro settings */
	selection = IS_INTRO;
	screen = 0;
	StateIntroOutro_LoadScreen();

	BGP_REG = PAL_DEF(0, 1, 2, 3);
	SHOW_BKG;
	HIDE_WIN;

	PlayMusic(menu_mod_Data, BANK_AUDIO, 1);
}

void Update_STATE_INTRO(void) {
	StateIntroOutro_HandleInput();
}

void Start_STATE_OUTRO(void) {
	extern UINT8* victory_mod_Data[];

	/* load outro settings */
	selection = IS_OUTRO;
	screen = 0;
	StateIntroOutro_LoadScreen();

	BGP_REG = PAL_DEF(0, 1, 2, 3);
	SHOW_BKG;
	HIDE_WIN;

	/* load victory music */
	PlayMusic(victory_mod_Data, BANK_AUDIO, 1);
}

void Update_STATE_OUTRO(void) {
	StateIntroOutro_HandleInput();
}

/**
 * \brief Handles the input for both intro and outro
 */
static void StateIntroOutro_HandleInput(void) {
	if (selection == IS_INTRO) {
		if (KEY_TICKED(J_START)) {
			/* skip intro, start the game */
			StateIntroOutro_NextState();
			return;
		}
	}

	/* next screen or start game */
	if (KEY_TICKED(J_A)) {
		/* increase number of current screen */
		screen++;

		if (screen != MAX_SCREENS) {
			/* load next screen */
			StateIntroOutro_LoadScreen();
		} else {
			/* the screen sequence is done, goto next state */
			StateIntroOutro_NextState();
		}
	}
}

/**
 * \brief Loads the given screens content
 */
static void StateIntroOutro_LoadScreen(void) {
	if (screen != 0) {
		FadeIn();
		DISPLAY_OFF;
	}

	if (selection == IS_INTRO) {
		switch (screen) {
		case 0: {
			ZInitScrollTilesColor(INTRO_TILES_PARAMS_1, 0);
			InitScrollColor(INTRO_MAP_PARAMS_1, 0);
			break;
		}
		case 1: {
			ZInitScrollTilesColor(INTRO_TILES_PARAMS_2, 0);
			InitScrollColor(INTRO_MAP_PARAMS_2, 0);
			break;
		}
		case 2: {
			ZInitScrollTilesColor(INTRO_TILES_PARAMS_3, 0);
			InitScrollColor(INTRO_MAP_PARAMS_3, 0);
			break;
		}
		}
	} else {
		switch (screen) {
		case 0: {
			ZInitScrollTilesColor(OUTRO_TILES_PARAMS_1, 0);
			InitScrollColor(OUTRO_MAP_PARAMS_1, 0);
			break;
		}
		case 1: {
			ZInitScrollTilesColor(OUTRO_TILES_PARAMS_2, 0);
			InitScrollColor(OUTRO_MAP_PARAMS_2, 0);
			break;
		}
		case 2: {
			ZInitScrollTilesColor(OUTRO_TILES_PARAMS_3, 0);
			InitScrollColor(OUTRO_MAP_PARAMS_3, 0);
			break;
		}
		}
	}

	if (screen != 0) {
		BGP_REG = PAL_DEF(0, 1, 2, 3);
		DISPLAY_ON;
		FadeOut();
	}
}

/**
 * \brief Loads the next state, depending on intro or outro setting
 */
static void StateIntroOutro_NextState(void) {
	if (selection == IS_INTRO) {
		// gamestart, load main menu, keep music running
		SetState(STATE_INTRO, 0);
	} else {
		// from outro, load main menu, stop the music
		SetState(STATE_MENU, 1);
	}
}
