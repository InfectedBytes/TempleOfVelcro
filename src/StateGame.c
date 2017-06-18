#pragma bank=2
#include "StateGame.h"
UINT8 bank_STATE_GAME = 2;

#include "..\res\src\tiles.h"
#include "..\res\src\map.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"

extern UINT8 n_sprite_types;

UINT8 collision_tiles[] = { 1, 0 };

extern const unsigned char * const test_mod_Data[];
  
void Start_STATE_GAME() {
	UINT8 i;
	SET_PAL0(0, 1, 2, 3);
	SET_PAL1(3, 2, 1, 0);
	SPRITES_8x16;
	for(i = 0; i != n_sprite_types; ++ i) {
		SpriteManagerLoad(i);
	}
	SHOW_SPRITES;

	scroll_target = SpriteManagerAdd(SPRITE_PLAYER, 50, 50);

	InitScrollTiles(0, 3, tiles, 3);
	InitScroll(mapWidth, mapHeight, map, collision_tiles, 0, 3);
	SHOW_BKG;
  
  PlayMusic(test_mod_Data, 3, 1);
}

void Update_STATE_GAME() {
}