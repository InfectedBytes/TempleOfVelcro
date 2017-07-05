#include "ZGBMain.h"
#include "Math.h"
#include "Sprite.h"
#include "BankManager.h"


// animated background
static UINT8 waterfallTimer = 0;
static UINT8 torchTimer = 0;
static UINT8 torchFrame = 0;
static UINT8 bkgBank;
static UINT8 waterfallBase;
static UINT8 waterfallEnd;
static UINT8 torchBase;
static unsigned char *animBkgTiles;

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

void AnimBkg_Setup(UINT8 _bkgBank, unsigned char *_animBkgTiles, UINT8 _waterfallBase, UINT8 _waterfallEnd, UINT8 _torchBase) {
	bkgBank = _bkgBank;
	animBkgTiles = _animBkgTiles;
	waterfallBase = _waterfallBase;
	waterfallEnd = _waterfallEnd;
	torchBase = _torchBase;
}

void AnimBkg_Update(void) {
	UINT8 waterfall = waterfallTimer++ >> 1;
	PUSH_BANK(bkgBank); // tileset bank
	set_bkg_data(waterfallBase, 1, &animBkgTiles[(waterfallBase + (UINT16)(waterfall & 0x7)) << 4]);
	set_bkg_data(waterfallEnd, 4, &animBkgTiles[(waterfallEnd + (UINT16)((waterfall & 1) << 2)) << 4]);
	if (++torchTimer == TORCH_SPEED) {
		torchTimer = 0;
		if (++torchFrame == 3) torchFrame = 0;
		set_bkg_data(torchBase, 1, &animBkgTiles[(torchBase + (UINT16)torchFrame) << 4]);
	}
	POP_BANK;
}
