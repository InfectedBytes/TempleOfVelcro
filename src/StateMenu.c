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
#include "../res/src/font.h"


/* ----- Defines ----- */
#define MENU_SELECTION_COL  4
#define MENU_INFO_COL 5
#define MENU_INFO_LINE 7


/* ----- Types / Enums ----- */


/* ----- Prototypes ----- */

static void StateMenu_Select(UINT8 selection);
static void StateMenu_SelectDifficulty(UINT8 selection);
static void StateMenu_UpdateSelection(UINT8 sel);


/* ----- Variables ----- */
static UINT8 selection = 0;
static MenuEntry mainMenuEntries[] = {
	{1, "  Play"},
	{3, "  Credits"}
};
static MenuEntry difficultyMenuEntries[] = {
	{1, "  Easy"},
	{2, "  Normal"},
	{3, "  Hard"},
	{5, "  Go Back"}
};

static Menu mainMenu = { 2, mainMenuEntries, StateMenu_Select };
static Menu difficultyMenu = { 4, difficultyMenuEntries, StateMenu_SelectDifficulty };

static Menu* currentMenu;

/* ----- Functions ----- */

/**
* \brief Selects the specified menu and draws it.
*
* \param menu is the new menu
* \param sel is the new menu selection
*/
static void SelectMenu(Menu* menu, UINT8 sel) {
	UINT8 i;
	currentMenu = menu;
	Clear();
	for (i = 0; i < menu->Count; i++) {
		MenuEntry* entry = &menu->Entries[i];
		PRINT_POS(MENU_SELECTION_COL, entry->Line);
		Printf(entry->Text);
	}
	StateMenu_UpdateSelection(sel);
	PRINT_POS(MENU_INFO_COL, MENU_INFO_LINE);
	Printf("BitBitJam4");
}

void Start_STATE_MENU(void) {
	/* setup background logo */
	InitScrollTiles(0, 32, tiles, 3);
	InitScroll(menuMapWidth, menuMapHeight, menuMap, 0, 0, 4);
	RefreshScroll();

	INIT_WINDOW(font, 3, 0, 8);

	SelectMenu(&mainMenu, 0);

	SHOW_BKG;
	BGP_REG = PAL_DEF(0, 1, 2, 3);

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
		currentMenu->Select(selection);
		return;
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
			if (newSelection != (currentMenu->Count - 1)) {
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
 * \brief Update the menu selection on the screen
 *
 * \param newSelection is the new menu selection changed by UP, DOWN ticks
 */
void StateMenu_UpdateSelection(UINT8 newSelection) {
	UINT8 line;

	/* clear old selection */
	line = currentMenu->Entries[selection].Line;
	PRINT_POS(MENU_SELECTION_COL, line);
	Printf(" ");

	/* set new selection */
	line = currentMenu->Entries[newSelection].Line;
	PRINT_POS(MENU_SELECTION_COL, line);
	Printf("X");

	/* finally store the new selection */
	selection = newSelection;
}


/* Menu select functions */

/* Handles the select event in the main menu (Play or Credits selected) */
void StateMenu_Select(UINT8 selection) {
	switch (selection) {
		case 0: // Play Menu
			SelectMenu(&difficultyMenu, GetDifficulty());
			break;
		case 1: // Credits
			HIDE_WIN;
			SetState(STATE_CREDITS);
			break;
	}
}

/* Handles the select event in the difficulty menu */
void StateMenu_SelectDifficulty(UINT8 selection) {
	if (selection == currentMenu->Count - 1) { // Back
		SelectMenu(&mainMenu, 0);
	} else { // 0-2 Difficulty
		HIDE_WIN;
		SetDifficulty(selection);
		SetState(STATE_GAME);
	}
}