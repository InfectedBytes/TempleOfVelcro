/* ----- Bank setup ----- */
#pragma bank=4
#include "StateDifficulty.h"
UINT8 bank_STATE_DIFFICULTY = 4;


/* ----- Includes ----- */
#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "../res/src/difficultyMap.h"
#include "../res/src/difficultyImages.h"
#include "../res/src/difficultyTiles.h"
#include "../res/src/font.h"


/* ----- Defines ----- */
#define BANK_DIFFICULTY_MAP 4
#define DIFFICULTY_MENU_ENTRIES 4
#define DIFFICULTY_TARGET_X 7
#define DIFFICULTY_TARGET_Y 5
#define DIFFICULTY_IMAGE_WIDTH 6
#define DIFFICULTY_IMAGE_HEIGHT 4
#define DIFFICULTY_IMAGE_TILES 24

/* ----- Types / Enums ----- */

/* ----- Prototypes ----- */
static void UpdateDifficulty();

/* ----- Variables ----- */
static UINT8 addresses[] = { 11, 19, 31, 39 };
static UINT8 lengths[] = { 4, 6, 4, 12 };
static UINT8 selection;

/* ----- Functions ----- */
void Start_STATE_DIFFICULTY(void) {
	/* setup background logo */
	InitScrollTiles(0, 121, difficultyTiles, BANK_MISC_TILES);
	InitScroll(difficultyMapWidth, difficultyMapHeight, difficultyMap, 0, 0, BANK_DIFFICULTY_MAP);
	SHOW_BKG;
	HIDE_WIN;
	selection = GetDifficulty();
	UpdateDifficulty();
	BGP_REG = PAL_DEF(0, 1, 2, 3);
}

void Update_STATE_DIFFICULTY(void) {
	if (KEY_TICKED(J_A) || KEY_TICKED(J_START)) {
		if (selection == DIFFICULTY_MENU_ENTRIES - 1) {
			SetState(STATE_MENU, 0);
		} else {
			SetDifficulty(selection);
			SetState(STATE_GAME, 1);
		}
	} else if (KEY_TICKED(J_UP) && selection > 0) {
		selection--;
		UpdateDifficulty();
	} else if (KEY_TICKED(J_DOWN) && selection < DIFFICULTY_MENU_ENTRIES - 1) {
		selection++;
		UpdateDifficulty();
	}
}

void UpdateDifficulty() {
	UINT8 i;
	for (i = 0; i < DIFFICULTY_MENU_ENTRIES; i++) {
		UINT8 offset = i == selection ? lengths[i] : 0;
		ReplaceTiles(BANK_MISC_TILES, addresses[i], lengths[i], difficultyTiles, offset);
	}
	if (selection != DIFFICULTY_MENU_ENTRIES - 1) { // not the "return to title" entry
		UINT16* data = difficultyImages + DIFFICULTY_IMAGE_TILES * selection;
		set_bkg_tiles(DIFFICULTY_TARGET_X, DIFFICULTY_TARGET_Y, DIFFICULTY_IMAGE_WIDTH, DIFFICULTY_IMAGE_HEIGHT, data);
	}
}