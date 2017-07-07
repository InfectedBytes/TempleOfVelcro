/* ----- Bank setup ----- */
#pragma bank=4
#include "StateCredits.h"
UINT8 bank_STATE_CREDITS = 4;


/* ----- Includes ----- */
#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "../res/src/creditsMap.h"
#include "../res/src/creditsTiles.h"
#include "../res/src/tutorialTiles.h"
#include "../res/src/tutorialMap.h"
#include "../res/src/font.h"


/* ----- Defines ----- */
#define MENU_CREDITS_LINE   1
#define MENU_CREDITS_COL    4
#define MENU_HELP_COL       3
#define MENU_HELP_LINE      7

#define CREDIT_SPAWN_TIME 80
#define CREDIT_SPAWN_Y 40


/* ----- Types / Enums ----- */


/* ----- Prototypes ----- */


/* ----- Variables ----- */
UINT8 creditsOrTutorial;
static UINT8 sheepSpawnCountdown;
UINT16 creditSheepCounter;
static UINT8 collision_tiles[] = { 2, 5, 0 };

/* ----- Functions ----- */
void Start_STATE_CREDITS(void) {
	/* init sprites */
	creditSheepCounter = 0;
	sheepSpawnCountdown = CREDIT_SPAWN_TIME >> 2;
	SET_PAL0(2, 0, 1, 3);
	SPRITES_8x16;
	SpriteManagerLoad(SPRITE_CREDIT_SHEEP);
	SHOW_SPRITES;

	/* setup background logo */
	if (creditsOrTutorial == CREDITS_MODE) {
		InitScrollTiles(0, 105, creditsTiles, BANK_MISC_TILES);
		InitScroll(creditMapWidth, creditMapHeight, creditMap, collision_tiles, 0, BANK_MISC_MAPS);
	} else {
		InitScrollTiles(0, 73, tutorialTiles, BANK_MISC_TILES);
		InitScroll(tutorialMapWidth, tutorialMapHeight, tutorialMap, collision_tiles, 0, BANK_MISC_MAPS);
	}
	INIT_FONT(font, BANK_FONT, PRINT_BKG);

	BGP_REG = PAL_DEF(0, 1, 2, 3);
	SHOW_BKG;
	HIDE_WIN;
}

void Update_STATE_CREDITS(void) {
	MoveScroll(20, 0);
	if (sheepSpawnCountdown-- == 0) {
		SpriteManagerAdd(SPRITE_CREDIT_SHEEP, 0, CREDIT_SPAWN_Y);
		sheepSpawnCountdown = CREDIT_SPAWN_TIME;
	}
	PRINT_POS(12, 10);
	Printf("%u", creditSheepCounter);

	/* leave the credits on any input */
	if (KEY_TICKED(0xFFU))
	{
		/* switch to next state, abort this function */
		Clear();
		HIDE_WIN;
		SetState(STATE_MENU, 0);
		return;
	}
}
