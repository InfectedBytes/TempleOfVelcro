#pragma bank=4
#include "StateVictory.h"
UINT8 bank_STATE_VICTORY = 4;

#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "../res/src/font.h"
#include "../res/src/gameoverMap.h"
#include "../res/src/tiles.h"

extern UINT8* victory_mod_Data[];

void Start_STATE_VICTORY() {
	HIDE_WIN;
	InitScrollTiles(0, 32, tiles, 3);
	InitScroll(gameoverMapWidth, gameoverMapHeight, gameoverMap, 0, 0, 4);
	RefreshScroll();
	BGP_REG = PAL_DEF(0, 1, 2, 3);
	INIT_FONT(font, 3, PRINT_BKG);
	PRINT_POS(6, 8);
	Printf("Victory!");
	PRINT_POS(3, 11);
	Printf("Press any key");
	SHOW_BKG;

	/* play menu sound */
	PlayMusic(victory_mod_Data, 5, 1);
}

void Update_STATE_VICTORY() {
	if (KEY_TICKED(-1)) {
		SetState(STATE_MENU, 1);
	}
}
