/* ----- Bank setup ----- */
#pragma bank=4
#include "StateMenu.h"
UINT8 bank_STATE_MENU = 4;


/* ----- Includes ----- */
#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "../res/src/map_menu.h"
#include "../res/src/tiles_title.h"


/* ----- Defines ----- */
#define MAP_BANK        4
#define TILES_BANK      3
#define MAX_MENU_INDEX  1

/* ----- Types / Enums ----- */


/* ----- Prototypes ----- */
static void StateMenu_UpdateSelection(UINT8 sel);
static void StateMenu_Select(UINT8 selection);


/* ----- Variables ----- */
static UINT8 selection = 0;
extern UINT8* menu_mod_Data[];


/* ----- Functions ----- */

void Start_STATE_MENU(void) {
	/* setup background logo */
	InitScrollTiles(0, 120, tiles_title, TILES_BANK);
	InitScroll(map_menuWidth, map_menuHeight, map_menu, 0, 0, MAP_BANK);
	//RefreshScroll();

	AnimBkg_Setup(TILES_BANK, tiles_title, // bank, tileset
				45,  // WATERFALL_BASE_ADDRESS
				53,  // WATERFALL_END_ADDRESS
				41); //TORCH_BASE_ADDRESS

	BGP_REG = PAL_DEF(0, 1, 2, 3);
	SHOW_BKG;

	/* play menu sound */
	PlayMusic(menu_mod_Data, 5, 1);
}

void Update_STATE_MENU(void) {
	/* Toggle autorun mode */
	if (KEY_TICKED(J_B) && KEY_PRESSED(J_SELECT)) {
		SetAutorun(!GetAutorun());
	}

	/* check for menu selection by pressing START or A */
	if (KEY_TICKED(J_A | J_START)) {
		StateMenu_Select(selection);
		return;
	}

	/* check for menu selection changes by pressing UP or DOWN */
	if (KEY_TICKED(J_UP | J_DOWN)) {
		/* get current selection */
		UINT8 newSelection = selection;

		if (KEY_TICKED(J_UP)) {
			/* decrement selection if possible */
			if (newSelection != 0) {
				newSelection--;
			}
		} else if (KEY_TICKED(J_DOWN)) {
			/* increment selection if possible */
			if (newSelection != MAX_MENU_INDEX) {
				newSelection++;
			}
		}

		/* on change, update the menu screen */
		if (newSelection != selection) {
			StateMenu_UpdateSelection(newSelection);
		}
	}

	AnimBkg_Update();
}

/**
 * \brief Update the menu selection on the screen
 *
 * \param newSelection is the new menu selection changed by UP, DOWN ticks
 */
void StateMenu_UpdateSelection(UINT8 newSelection) {
	// TODO: switch highlighted text

	/* finally store the new selection */
	selection = newSelection;
}

/**
 * \brief Handles the select event in the main menu (Play or Credits selected)
 *
 * \param selection is the menu to start
 */
static void StateMenu_Select(UINT8 selection) {
	switch (selection) {
		case 0: // Play Menu
			// TODO: SetState(STATE_DIFFICULTY, 0);
			SetDifficulty(EASY);
			SetState(STATE_GAME);
			break;
		case 1: // Credits
			SetState(STATE_CREDITS, 0);
			break;
	}
}
