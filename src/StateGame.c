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

UINT8 collision_tiles[] = { 1, 0 };
UINT8 gameover;

extern UINT8* test_mod_Data[];
  
void Start_STATE_GAME() {
	UINT8 i;
	SET_PAL0(0, 1, 2, 3);
	SET_PAL1(3, 2, 1, 0);
	SPRITES_8x16;
	for(i = 0; i != n_sprite_types; ++ i) {
		SpriteManagerLoad(i);
	}
	SHOW_SPRITES;

	//scroll_target = SpriteManagerAdd(SPRITE_PLAYER, 50, 50);

	InitScrollTiles(0, 5, tiles, 3);
	InitScroll(mapWidth, mapHeight, map, collision_tiles, 0, 3);
	SHOW_BKG;
	INIT_WINDOW(font, 3, 0, 1);
	PlayMusic(test_mod_Data, 5, 1);
	gameover = 0;
}

void Update_STATE_GAME() {
}