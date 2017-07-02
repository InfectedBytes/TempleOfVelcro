#pragma bank=2
#include "StateGameover.h"
UINT8 bank_STATE_GAMEOVER = 2;

#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "../res/src/font.h"
#include "../res/src/gameoverMap.h"
#include "../res/src/tiles.h"

void Start_STATE_GAMEOVER() {
	InitScrollTiles(0, 32, tiles, 3);
	InitScroll(gameoverMapWidth, gameoverMapHeight, gameoverMap, 0, 0, 4);
	RefreshScroll();
	SHOW_BKG;
	HIDE_WIN;
	INIT_FONT(font, 3, PRINT_BKG);
	PRINT_POS(6, 8);
	Printf("Gameover");
	PRINT_POS(3, 12);
	Printf("Press any key");
	BGP_REG = PAL_DEF(0, 1, 2, 3);
}

void Update_STATE_GAMEOVER() {
	if (KEY_TICKED(-1)) {
		SetState(STATE_MENU);
	}
}