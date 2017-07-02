#include "ZGBMain.h"
#include "Math.h"
#include "Sprite.h"
#include "BankManager.h"

UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {
	if(current_state == STATE_GAME) {
		if(U_LESS_THAN(127 - (UINT16)*tile_ptr, N_SPRITE_TYPES)) {
			*tile = 0;
			return 127 - (UINT16)*tile_ptr;
		}

		*tile = *tile_ptr;
	}

	return 255u;
}

void ReplaceFrames(UINT8 firstTile, FrameSize frameSize, UINT8 frames, UINT8* data, UINT8 dataBank, UINT16 skipFrames) {
	PUSH_BANK(dataBank);
	set_sprite_data(firstTile, GetFrameCount(frames, frameSize), data + ((UINT16)GetFrameCount(skipFrames, frameSize) << 4));
	POP_BANK;
}