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
#include "../res/src/difficultyTiles.h"
#include "../res/src/font.h"


/* ----- Defines ----- */
#define BANK_DIFFICULTY_MAP 4
#define DIFFICULTIES 3

/* ----- Types / Enums ----- */

/* ----- Prototypes ----- */
static void UpdateDifficulty();

/* ----- Variables ----- */
static UINT8 addresses[] = { 11,19,31 };
static UINT8 lengths[] = { 4,6,4 };
static UINT8 selection;

/* ----- Functions ----- */
void Start_STATE_DIFFICULTY(void) {
	/* setup background logo */
	InitScrollTiles(0, 120, difficultyTiles, BANK_MISC_TILES);
	InitScroll(difficultyMapWidth, difficultyMapHeight, difficultyMap, 0, 0, BANK_DIFFICULTY_MAP);
	SHOW_BKG;
	HIDE_WIN;
	selection = GetDifficulty();
	UpdateDifficulty();
}

void Update_STATE_DIFFICULTY(void) {
	if (KEY_TICKED(J_A) || KEY_TICKED(J_START)) {
		SetState(STATE_INTRO, 0);
	} else if (KEY_TICKED(J_UP) && selection > 0) {
		selection--;
		UpdateDifficulty();
	} else if (KEY_TICKED(J_DOWN) && selection < DIFFICULTIES - 1) {
		selection++;
		UpdateDifficulty();
	}
}

void UpdateDifficulty() {
	UINT8 i;
	for (i = 0; i < DIFFICULTIES; i++) {
		UINT8 offset = i == selection ? lengths[i] : 0;
		ReplaceTiles(BANK_MISC_TILES, addresses[i], lengths[i], difficultyTiles, offset);
	}
	SetDifficulty(selection);
}