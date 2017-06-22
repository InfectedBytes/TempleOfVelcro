#include "ZGBMain.h"
#include "Math.h"

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