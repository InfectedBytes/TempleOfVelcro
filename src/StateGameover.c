#pragma bank=4
#include "StateGameover.h"
UINT8 bank_STATE_GAMEOVER = 4;

#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "../res/src/gameoverMap.h"
#include "../res/src/gameoverTiles.h"

extern UINT8* gameover_mod_Data[];

void Start_STATE_GAMEOVER() {
	HIDE_WIN;
	InitScrollTiles(0, 75, gameoverTiles, 4);
	InitScroll(gameoverMapWidth, gameoverMapHeight, gameoverMap, 0, 0, BANK_MISC_MAPS);
	BGP_REG = PAL_DEF(0, 1, 2, 3);
	SHOW_BKG;

	/* play menu sound */
	PlayMusic(gameover_mod_Data, BANK_AUDIO, 1);
}

void Update_STATE_GAMEOVER() {
	if (KEY_TICKED(-1)) {
		SetState(STATE_MENU, 1);
	}
}
