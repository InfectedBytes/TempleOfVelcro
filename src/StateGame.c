#pragma bank=2
#include "StateGame.h"
UINT8 bank_STATE_GAME = 2;

#include "..\res\src\tiles.h"
#include "..\res\src\level1.h"
#include "..\res\src\level2.h"
#include "..\res\src\level3.h"
#include "..\res\src\font.h"
#include "..\res\src\pauseTiles.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"

#define WATERFALL_BASE_ADDRESS 81
#define WATERFALL_END_ADDRESS 89
#define TORCH_BASE_ADDRESS 100
#define FIRE_PIT_BASE_ADDRESS 78

#define PAUSE_POS_X 6
// WY_REG is set to a value between these two, depending on whether the pause menu is visible or not
#define PAUSE_MENU_IN 128
#define PAUSE_MENU_OUT 136
// we are overriding the characters A-E with "continue" and F-K with "quit game"
#define FONT_OVERRIDE_ADDRESS font_idx + 1
#define PAUSE_CONTINUE_WIDTH 80
#define PAUSE_QUIT_WIDTH 96
#define PAUSE_QUIT_ADDRESS 160

extern UINT8 n_sprite_types;
extern UINT8 font_idx;

UINT8 collision_tiles[] = { 1, 2, 3, 11, 12, 13, 14, 15, 16, 18, 20, 21, 23, 26, 27, 29, 30, 31, 32, 33, 35, 36, 37, 38, 39, 0 };
UINT8 collision_tiles_down[] = { 46, 47, 48, 0 };

extern UINT8* level_mod_Data[];
UINT8 paused;
static UINT8 hudPosition; // used to smoothly scroll the pause menu in and out

typedef enum {
	CONTINUE,
	QUIT
} PauseState;
static PauseState pauseSelection;

void Start_STATE_GAME() {
	SET_PAL0(0, 1, 2, 3);
	SPRITES_8x16;
	SpriteManagerLoad(SPRITE_PLAYER);
	SpriteManagerLoad(SPRITE_HEART);
	SpriteManagerLoad(SPRITE_COUNTDOWN);
	SpriteManagerLoad(SPRITE_SPARKLES);
	SHOW_SPRITES;

	AnimBkg_Setup(3, tiles,
			WATERFALL_BASE_ADDRESS, WATERFALL_END_ADDRESS,
			TORCH_BASE_ADDRESS, FIRE_PIT_BASE_ADDRESS);
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
		SpriteManagerAdd(SPRITE_COUNTDOWN, (SCREENWIDTH >> 1) - 4, (SCREENHEIGHT >> 2) + 16);
	}
	paused = 0;
	pauseSelection = CONTINUE;
	hudPosition = PAUSE_MENU_OUT;
}

static void ReplaceFont() {
	if (pauseSelection == CONTINUE) {
		set_bkg_data(FONT_OVERRIDE_ADDRESS, 5, pauseTiles + PAUSE_CONTINUE_WIDTH);
		set_bkg_data(FONT_OVERRIDE_ADDRESS + 5, 6, pauseTiles + PAUSE_QUIT_ADDRESS);
	} else {
		set_bkg_data(FONT_OVERRIDE_ADDRESS, 5, pauseTiles);
		set_bkg_data(FONT_OVERRIDE_ADDRESS + 5, 6, pauseTiles + PAUSE_QUIT_ADDRESS + PAUSE_QUIT_WIDTH);
	}
}

void Update_STATE_GAME() {
	SHOW_WIN;
	AnimBkg_Update();
	WY_REG = hudPosition;
	if (paused) {
		if (hudPosition != PAUSE_MENU_IN) hudPosition--;
		if (KEY_TICKED(J_UP) && pauseSelection != CONTINUE) {
			pauseSelection = CONTINUE;
			ReplaceFont();
		} else if (KEY_TICKED(J_DOWN) && pauseSelection != QUIT) {
			pauseSelection = QUIT;
			ReplaceFont();
		} else if (KEY_TICKED(J_START) || KEY_TICKED(J_A)) {
			if (pauseSelection == CONTINUE) {
				paused = FALSE;
			} else {
				SetState(STATE_MENU, 1);
				HIDE_WIN;
			}
			// clear
			PRINT_POS(PAUSE_POS_X, 0);
			Printf("      ");
			PRINT_POS(PAUSE_POS_X, 1);
			Printf("      ");
		}
	} else {
		if (hudPosition != PAUSE_MENU_OUT) hudPosition++;
		if (KEY_TICKED(J_START)) {
			paused = TRUE;
			ReplaceFont();
			// we are overriding the characters A-F with "continue" and G-L with "quit game"
			PRINT_POS(PAUSE_POS_X, 0);
			Printf("ABCDE");
			PRINT_POS(PAUSE_POS_X, 1);
			Printf("FGHIJK");
		}
	}
}
