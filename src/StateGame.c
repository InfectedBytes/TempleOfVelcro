#pragma bank=2
#include "StateGame.h"
UINT8 bank_STATE_GAME = 2;

#include "..\res\src\tiles.h"
#include "..\res\src\level1.h"
#include "..\res\src\level2.h"
#include "..\res\src\level3.h"
#include "..\res\src\font.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"

#define WATERFALL_BASE_ADDRESS 81
#define WATERFALL_END_ADDRESS 89
#define TORCH_BASE_ADDRESS 100

extern UINT8 n_sprite_types;

UINT8 collision_tiles[] = { 11, 12, 13, 14, 15, 16, 18, 26, 27, 30, 31, 32, 33, 35, 36, 37, 38, 39, 0 };
UINT8 collision_tiles_down[] = { 46, 47, 48, 0 };

extern UINT8* level_mod_Data[];
  
void Start_STATE_GAME() {
	SET_PAL0(0, 1, 2, 3);
	SPRITES_8x16;
	SpriteManagerLoad(SPRITE_PLAYER);
	SpriteManagerLoad(SPRITE_HEART);
	SpriteManagerLoad(SPRITE_COUNTDOWN);
	SHOW_SPRITES;

	AnimBkg_Setup(3, tiles,
			WATERFALL_BASE_ADDRESS, WATERFALL_END_ADDRESS,
			TORCH_BASE_ADDRESS);
	InitScrollTiles(0, 128, tiles, 3);
	InitScroll(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_MAP_ADDRESS, collision_tiles, collision_tiles_down, LEVEL_FIRST_BANK + currentLevel);
	adjustableOffsetX = 16; // player should be left of the screen
	RefreshScroll();
	SHOW_BKG;
	INIT_WINDOW(font, BANK_FONT, 0, 1);
	HIDE_WIN; // hide for now
	PlayMusic(level_mod_Data, BANK_AUDIO, 1);
	BGP_REG = PAL_DEF(0, 1, 2, 3);
	Clear();

	SpriteManagerAdd(SPRITE_PLAYER, 2 << 3, 12 << 3);

	if (GetAutorun()) {
		SpriteManagerAdd(SPRITE_COUNTDOWN, (SCREENWIDTH >> 1) + 8, SCREENHEIGHT >> 2);
	}
}

void Update_STATE_GAME() {
	SHOW_WIN;
	AnimBkg_Update();
}
