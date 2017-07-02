#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#include "main.h"
#include "gb/hardware.h"
#include "palette.h"
#include "Print.h"
#include "Utils.h"
#include "Math.h"

#define MAX_TILE_ID 99

// Helper function, used to replace the players frame cache.
// This function can't be in switchable ROM, because it will swap banks in order to copy the frames.
void ReplaceFrames(UINT8 firstTile, FrameSize frameSize, UINT8 frames, UINT8* data, UINT8 dataBank);
void Clear();

typedef enum {
	STATE_GAME,
	STATE_MENU,
	STATE_GAMEOVER,

	N_STATES
} STATE;

typedef enum {
	SPRITE_PLAYER,
	SPRITE_HEART,

	N_SPRITE_TYPES
} SPRITE_TYPE;

#endif
