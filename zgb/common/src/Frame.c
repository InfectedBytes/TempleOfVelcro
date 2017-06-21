#include "Frame.h"
#include "OAMManager.h"
#include "Scroll.h"

#define SCREENWIDTH_PLUS_32 192 //160 + 32
#define SCREENHEIGHT_PLUS_32 176 //144 + 32

UINT8 GetFrameCount(UINT8 frames, FrameSize size) {
	if (size == FRAME_24x32) return frames * 12;
	return frames << size;
}

UINT8 GetOamCount(UINT8 frames, FrameSize size) {
	if (size == FRAME_8x8) return frames << size;
	return GetFrameCount(frames, size) >> 1;
}

void DrawFrame(UINT8 oam_idx, FrameSize size, int idx, UINT16 x, UINT16 y, UINT8 flags){
	x = x - scroll_x;
	y = y - scroll_y;

	//It might sound stupid adding 32 in both sides but remember the values are unsigned! (and maybe truncated after substracting scroll_)
	if((x + 32u > SCREENWIDTH_PLUS_32) || (y + 32 > SCREENHEIGHT_PLUS_32)) {
		x = 200u;
		y = 200u;
	}
	
	switch(size) {
		case FRAME_8x8:
			DrawOAMSprite(oam_idx, idx, x + 8u, y + 16u, flags);
			break;

		case FRAME_8x16:
			DrawOAMSprite(oam_idx,     idx, x + 8u, y + 16u, flags);
			break;

		case FRAME_16x16:
			if (flags & OAM_VERTICAL_FLAG) {
				DrawOAMSprite(oam_idx + 1, idx + 2u, x + 8u, y + 16u, flags);
				DrawOAMSprite(oam_idx, idx, x + 8u + 8u, y + 16u, flags);
			} else {
				DrawOAMSprite(oam_idx, idx, x + 8u, y + 16u, flags);
				DrawOAMSprite(oam_idx + 1, idx + 2u, x + 8u + 8u, y + 16u, flags);
			}
			break;

		case FRAME_24x32:
			if (flags & OAM_VERTICAL_FLAG) {
				DrawFrame(oam_idx, FRAME_16x16, idx, x + 8u, y, flags);
				DrawFrame(oam_idx + 2u, FRAME_16x16, idx + 4u, x + 8u, y + 16u, flags);
				DrawOAMSprite(oam_idx + 4u, idx + 8u, x + 8u, y + 16u, flags);
				DrawOAMSprite(oam_idx + 5u, idx + 10u, x + 8u, y + 32u, flags);
			} else {
				DrawFrame(oam_idx, FRAME_16x16, idx, x, y, flags);
				DrawFrame(oam_idx + 2u, FRAME_16x16, idx + 4u, x, y + 16u, flags);
				DrawOAMSprite(oam_idx + 4u, idx + 8u, x + 24u, y + 16u, flags);
				DrawOAMSprite(oam_idx + 5u, idx + 10u, x + 24u, y + 32u, flags);
			}
			break;

		case FRAME_32x32:
			if (flags & OAM_VERTICAL_FLAG) {
				DrawFrame(oam_idx, FRAME_16x16, idx, x + 16u, y, flags);
				DrawFrame(oam_idx + 2u, FRAME_16x16, idx + 4u, x + 16u, y + 16u, flags);
				DrawFrame(oam_idx + 4u, FRAME_16x16, idx + 8u, x, y, flags);
				DrawFrame(oam_idx + 6u, FRAME_16x16, idx + 12u, x, y + 16u, flags);
			} else {
				DrawFrame(oam_idx, FRAME_16x16, idx, x, y, flags);
				DrawFrame(oam_idx + 2u, FRAME_16x16, idx + 4u, x, y + 16u, flags);
				DrawFrame(oam_idx + 4u, FRAME_16x16, idx + 8u, x + 16u, y, flags);
				DrawFrame(oam_idx + 6u, FRAME_16x16, idx + 12u, x + 16u, y + 16u, flags);
			}
			break;
	}
}