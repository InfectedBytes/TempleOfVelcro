#include "ZGBMain.h"
#include "Math.h"
#include "Sprite.h"
#include "BankManager.h"
#include "../res/src/tiles.h"


// animated background
static UINT8 waterfallTimer = 0;
static UINT8 torchTimer = 0;
static UINT8 torchFrame = 0;

static Difficulty difficulty = NORMAL;
static UINT8 autorun = TRUE;

void SetDifficulty(Difficulty dif) { difficulty = dif; }
Difficulty GetDifficulty() { return difficulty; }

void SetAutorun(UINT8 run) { autorun = run; }
UINT8 GetAutorun() { return autorun; }

UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {
	if(current_state == STATE_GAME) {
		if((UINT16)TILE_HEART_SPAWN == (UINT16)*tile_ptr) {
			*tile = 0;
			return SPRITE_HEART;
		}

		*tile = *tile_ptr;
	}

	return 255u;
}

void ReplaceFrames(UINT8 firstTile, FrameSize frameSize, UINT8 frames, UINT8* data, UINT8 dataBank) {
	PUSH_BANK(dataBank);
	set_sprite_data(firstTile, GetFrameCount(frames, frameSize), data);
	POP_BANK;
}

void Clear() {
	UINT8 i;
	for (i = 0; i < 18; i++) {
		PRINT_POS(0, i);
		Printf("                    ");
	}
}

void AnimateBackground() {
	PUSH_BANK(3); // tileset bank
	waterfallTimer++;
	if(waterfallTimer == WATERFALL_SPEED) {
		set_bkg_data(WATERFALL_BASE_ADDRESS, WATERFALL_TILES, &tiles[(WATERFALL_BASE_ADDRESS + WATERFALL_TILES) << 4]);
		set_bkg_data(WATERFALL_END_ADDRESS, WATERFALL_TILES, &tiles[(WATERFALL_END_ADDRESS + WATERFALL_TILES) << 4]);
	} else if (waterfallTimer == WATERFALL_SPEED << 1) {
		waterfallTimer = 0;
		set_bkg_data(WATERFALL_BASE_ADDRESS, WATERFALL_TILES, &tiles[WATERFALL_BASE_ADDRESS << 4]);
		set_bkg_data(WATERFALL_END_ADDRESS, WATERFALL_TILES, &tiles[WATERFALL_END_ADDRESS << 4]);
	}
	torchTimer++;
	if (torchTimer == TORCH_SPEED) {
		torchTimer = 0;
		if (++torchFrame == 3) torchFrame = 0;
		set_bkg_data(TORCH_BASE_ADDRESS, 1, &tiles[(TORCH_BASE_ADDRESS + (UINT16)torchFrame) << 4]);
	}
	POP_BANK;
}