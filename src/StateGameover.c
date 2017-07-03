#pragma bank=4
#include "StateGameover.h"
UINT8 bank_STATE_GAMEOVER = 4;

#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "../res/src/font.h"
#include "../res/src/gameoverMap.h"
#include "../res/src/tiles.h"

extern UINT8* gameover_mod_Data[];

void Start_STATE_GAMEOVER() {
	HIDE_WIN;
	InitScrollTiles(0, 32, tiles, 3);
	InitScroll(gameoverMapWidth, gameoverMapHeight, gameoverMap, 0, 0, 4);
	RefreshScroll();
	INIT_FONT(font, 3, PRINT_BKG);
	PRINT_POS(6, 8);
	Printf("Gameover");
	PRINT_POS(3, 11);
	Printf("Press any key");
	BGP_REG = PAL_DEF(0, 1, 2, 3);
	SHOW_BKG;

	/* play menu sound */
	PlayMusic(gameover_mod_Data, 5, 1);
}

void Update_STATE_GAMEOVER() {
	if (KEY_TICKED(-1)) {
		SetState(STATE_MENU, 1);
	}
}
