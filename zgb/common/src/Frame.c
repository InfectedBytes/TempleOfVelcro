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

void DrawFrame(UINT8 oam_idx, FrameSize size, int idx, UINT16 x, UINT16 y, UINT8 flags) {
	UINT8 xShift;
	x = x - scroll_x;
	y = y - scroll_y;

	//It might sound stupid adding 32 in both sides but remember the values are unsigned! (and maybe truncated after substracting scroll_)
	if ((x + 32u > SCREENWIDTH_PLUS_32) || (y + 32 > SCREENHEIGHT_PLUS_32)) {
		x = 200u;
		y = 200u;
	}
	//flags |= OAM_VERTICAL_FLAG;
	switch (size) {
	case FRAME_8x8:
		DrawOAMSprite(oam_idx, idx, x + 8u, y + 16u, flags);
		break;

	case FRAME_8x16:
		DrawOAMSprite(oam_idx, idx, x + 8u, y + 16u, flags);
		break;

	case FRAME_16x16:
		xShift = (flags & OAM_VERTICAL_FLAG) >> 2; // 8 if flag is set, 0 otherwise
		DrawOAMSprite(oam_idx, idx, x + 8u + xShift, y + 16u, flags);
		DrawOAMSprite(oam_idx + 1, idx + 2u, x + 16u - xShift, y + 16u, flags);
		break;

	case FRAME_24x32:
		xShift = (flags & OAM_VERTICAL_FLAG) >> 1; // 16 if flag is set, 0 otherwise
		// top left 16x16
		DrawOAMSprite(oam_idx, idx, x + 8u + xShift, y + 16u, flags);
		DrawOAMSprite(oam_idx + 1u, idx + 2u, x + 16u, y + 16u, flags);
		// bottom left 16x16
		DrawOAMSprite(oam_idx + 2u, idx + 4u, x + 8u + xShift, y + 32u, flags);
		DrawOAMSprite(oam_idx + 3u, idx + 6u, x + 16u, y + 32u, flags);
		// top right 8x16
		DrawOAMSprite(oam_idx + 4u, idx + 8u, x + 24u - xShift, y + 16u, flags);
		// bottom right 8x16
		DrawOAMSprite(oam_idx + 5u, idx + 10u, x + 24u - xShift, y + 32u, flags);
		break;

	case FRAME_32x32:
		xShift = (flags & OAM_VERTICAL_FLAG) >> 2; // 8 if flag is set, 0 otherwise
		// second 8x32
		DrawOAMSprite(oam_idx + 1u, idx + 2u, x + 16u + xShift, y + 16u, flags);
		DrawOAMSprite(oam_idx + 3u, idx + 6u, x + 16u + xShift, y + 32u, flags);
		// third 8x32
		DrawOAMSprite(oam_idx + 4u, idx + 8u, x + 24u - xShift, y + 16u, flags);
		DrawOAMSprite(oam_idx + 6u, idx + 12u, x + 24u - xShift, y + 32u, flags);

		xShift |= (xShift << 1); // 24 if flag is set, 0 otherwise
		// first 8x32
		DrawOAMSprite(oam_idx, idx, x + 8u + xShift, y + 16u, flags);
		DrawOAMSprite(oam_idx + 2u, idx + 4u, x + 8u + xShift, y + 32u, flags);
		// last 8x32
		DrawOAMSprite(oam_idx + 5u, idx + 10u, x + 32u - xShift, y + 16u, flags);
		DrawOAMSprite(oam_idx + 7u, idx + 14u, x + 32u - xShift, y + 32u, flags);
		break;
	}
}