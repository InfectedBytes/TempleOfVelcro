/* ----- Bank setup ----- */
#pragma bank=4
#include "StateCredits.h"
UINT8 bank_STATE_CREDITS = 4;


/* ----- Includes ----- */
#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "../res/src/menuMap.h"
#include "../res/src/tiles.h"
#include "../res/src/font.h"


/* ----- Defines ----- */
#define MENU_CREDITS_LINE   1
#define MENU_CREDITS_COL    4
#define MENU_HELP_COL       3
#define MENU_HELP_LINE      7


/* ----- Types / Enums ----- */


/* ----- Prototypes ----- */


/* ----- Variables ----- */


/* ----- Functions ----- */
void Start_STATE_CREDITS(void) {
	/* setup background logo */
	InitScrollTiles(0, 32, tiles, 3);
	InitScroll(menuMapWidth, menuMapHeight, menuMap, 0, 0, 4);
	RefreshScroll();

	/* clear screen */
	Clear();

	PRINT_POS(MENU_CREDITS_COL - 2, MENU_CREDITS_LINE);
	Printf("TWITTER:");

	PRINT_POS(MENU_CREDITS_COL, MENU_CREDITS_LINE + 1);
	Printf("@DIMACHOTIN");

	PRINT_POS(MENU_CREDITS_COL, MENU_CREDITS_LINE + 2);
	Printf("@INFECTED_BYTES");

	PRINT_POS(MENU_CREDITS_COL, MENU_CREDITS_LINE + 3);
	Printf("@STONYYBIRD");

	PRINT_POS(MENU_CREDITS_COL, MENU_CREDITS_LINE + 4);
	Printf("@IAMVAULTKID");

	PRINT_POS(MENU_HELP_COL, MENU_HELP_LINE);
	Printf("PRESS ANY KEY");

	BOTTOM_LINES(8);
	BGP_REG = PAL_DEF(0, 1, 2, 3);
	SHOW_BKG;
	SHOW_WIN;

	/* TODO: play credits sound or keep music from intro? */
#if 0
	PlayMusic(test_mod_Data, 5, 1);
	NR52_REG = 0x80; // Enables sound, you should always setup this first
	NR51_REG = 0xFF;// Enables all channels (left and right)
	NR50_REG = 0x77;// Max volume
#endif
}

void Update_STATE_CREDITS(void) {
	BOTTOM_LINES(8);

	/* leave the credits on any input */
	if (KEY_TICKED(0xFFU))
	{
		/* switch to next state, abort this function */
		Clear();
		HIDE_WIN;
		SetState(STATE_MENU);
		return;
	}
}
