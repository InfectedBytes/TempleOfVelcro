#pragma bank=1
#include "ZGBMain.h"
UINT8 init_bank = 1;


#include "StateGame.h"
#include "StateMenu.h"
#include "StateGameover.h"

#include "SpritePlayer.h"
#include "SpriteHeart.h"

#include "../res/src/sheep3.h"
#include "../res/src/pickups.h"

UINT8 next_state = STATE_MENU;

SET_N_STATES(N_STATES);
SET_N_SPRITE_TYPES(N_SPRITE_TYPES);

void InitStates() {
	INIT_STATE(STATE_GAME);
	INIT_STATE(STATE_MENU);
	INIT_STATE(STATE_GAMEOVER);
}

void InitSprites() {
	INIT_SPRITE(SPRITE_PLAYER, sheep3, 3, FRAME_24x32, 14);
	INIT_SPRITE(SPRITE_HEART, pickups, 3, FRAME_8x16, 6);
}
