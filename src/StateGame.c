#pragma bank=2
#include "StateGame.h"
UINT8 bank_STATE_GAME = 2;

#include "..\res\src\tiles.h"
#include "..\res\src\map.h"
#include "..\res\src\font.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"

extern UINT8 n_sprite_types;

UINT8 collision_tiles[] = { 1, 4, 0 };

extern UINT8* level_mod_Data[];
  
void Start_STATE_GAME() {
	UINT8 i;
	SET_PAL0(0, 1, 2, 3);
	SPRITES_8x16;
	for(i = 0; i != n_sprite_types; ++ i) {
		SpriteManagerLoad(i);
	}
	SHOW_SPRITES;

	InitScrollTiles(0, 32, tiles, 3);
	InitScroll(mapWidth, mapHeight, map, collision_tiles, 0, 6);
	adjustableOffsetX = 16; // player should be left of the screen
	RefreshScroll();
	SHOW_BKG;
	INIT_WINDOW(font, 3, 0, 1);
	HIDE_WIN; // hide for now
	PlayMusic(level_mod_Data, 5, 1);
	BGP_REG = PAL_DEF(0, 1, 2, 3);
	Clear();
}

void Update_STATE_GAME() {
	SHOW_WIN;
}
