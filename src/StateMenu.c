/* ----- Bank setup ----- */
#pragma bank=4
#include "StateMenu.h"
UINT8 bank_STATE_MENU = 4;


/* ----- Includes ----- */
#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "../res/src/menuMap.h"
#include "../res/src/tiles.h"
#include "..\res\src\font.h"


/* ----- Defines ----- */
#define MENU_SELECTION_LINE 1
#define MENU_SELECTION_COL  4
#define MENU_HELP_COL       2
#define MENU_HELP_LINE      6


/* ----- Types / Enums ----- */
enum {
	MENU_GOTO_GAME,
	MENU_GOTO_CREDITS,
	MENU_MAX_OPTIONS
};


/* ----- Prototypes ----- */
static UINT8 StateMenu_GetNextState(UINT8 selection);
static void StateMenu_UpdateSelection(UINT8 sel);


/* ----- Variables ----- */
static UINT8 selection = 0;


/* ----- Functions ----- */
void Start_STATE_MENU(void) {
	/* setup background logo */
	InitScrollTiles(0, 32, tiles, 3);
	InitScroll(menuMapWidth, menuMapHeight, menuMap, 0, 0, 4);
	RefreshScroll();

	INIT_WINDOW(font, 3, 0, 1);

	// clear screen
	Clear();
	PRINT_POS(MENU_SELECTION_COL, MENU_SELECTION_LINE);
	Printf("  Play");

	PRINT_POS(MENU_SELECTION_COL, MENU_SELECTION_LINE + 2);
	Printf("  Credits");

	PRINT_POS(MENU_HELP_COL, MENU_HELP_LINE);
	Printf("UP DOWN: change");
	PRINT_POS(MENU_HELP_COL, MENU_HELP_LINE + 1);
	Printf("A START: select");

	StateMenu_UpdateSelection(MENU_GOTO_GAME);
	SHOW_BKG;

	/* TODO: play menu sound or keep music from intro */
#if 0
	PlayMusic(test_mod_Data, 5, 1);
	NR52_REG = 0x80; // Enables sound, you should always setup this first
	NR51_REG = 0xFF;// Enables all channels (left and right)
	NR50_REG = 0x77;// Max volume
#endif
}

void Update_STATE_MENU(void) {
	BOTTOM_LINES(8);
	/* check for menu selection by pressing START or A */
	if (KEY_TICKED(J_A | J_START)) {
		/* get the next state from current menu selection */
		UINT8 nextState = StateMenu_GetNextState(selection);

		/* check if next state is valid */
		if (255 != nextState) {
			Clear();
			HIDE_WIN;
			/* switch to next state, abort this function */
			SetState(nextState);
			return;
		}
	}

	/* check for menu selection changes by pressing UP or DOWN */
	if (KEY_TICKED(J_UP | J_DOWN)) {
		/* get current selection */
		UINT8 newSelection = selection;

		if (KEY_TICKED(J_UP)) {
			/* decrement menu selection if possible */
			if (newSelection != 0) {
				newSelection--;
			}
		} else if (KEY_TICKED(J_DOWN)) {
			newSelection = selection;
			if (newSelection != (MENU_MAX_OPTIONS - 1)) {
				newSelection++;
			}
		}

		/* on change, update the menu screen */
		if (newSelection != selection) {
			StateMenu_UpdateSelection(newSelection);
		}
	}
}

/**
 * \brief Returns next state from given menu selection
 *
 * \return next state or 255 on any error
 */
UINT8 StateMenu_GetNextState(UINT8 selection) {
	UINT8 result = 255;

	if (MENU_GOTO_GAME == selection) {
		result = STATE_GAME;
	}

	return result;
}

/**
 * \brief Update the menu selection on the screen
 *
 * \param newSelection is the new menu selection changed by UP, DOWN ticks
 */
void StateMenu_UpdateSelection(UINT8 newSelection) {
	UINT8 line;

	/* clear old selection */
	line = MENU_SELECTION_LINE + (selection << 1);
	PRINT_POS(MENU_SELECTION_COL, line);
	Printf(" ");

	/* set new selection */
	line = MENU_SELECTION_LINE + (newSelection << 1);
	PRINT_POS(MENU_SELECTION_COL, line);
	Printf("X");

	/* finally store the new selection */
	selection = newSelection;
}
