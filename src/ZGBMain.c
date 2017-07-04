#include "ZGBMain.h"
#include "Math.h"
#include "Sprite.h"
#include "BankManager.h"


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