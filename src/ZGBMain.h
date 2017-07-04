#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#include "main.h"
#include "gb/hardware.h"
#include "palette.h"
#include "Print.h"
#include "Utils.h"
#include "Math.h"

#define FRAMES_PER_SECOND 50
#define START_DELAY 150

#define MAX_TILE_ID 99

// animated background
// one tile costs 16 bytes => 4 tiles = 64 bytes
#define WATERFALL_BASE_ADDRESS 81
#define WATERFALL_END_ADDRESS 89
#define TORCH_BASE_ADDRESS 100
#define TORCH_SPEED 5

void AnimateBackground();

// Helper function, used to replace the players frame cache.
// This function can't be in switchable ROM, because it will swap banks in order to copy the frames.
void ReplaceFrames(UINT8 firstTile, FrameSize frameSize, UINT8 frames, UINT8* data, UINT8 dataBank);
void Clear();

typedef enum {
	EASY,
	NORMAL,
	HARD
} Difficulty;

void SetDifficulty(Difficulty dif);
Difficulty GetDifficulty();

void SetAutorun(UINT8 run);
UINT8 GetAutorun();

typedef enum {
	STATE_GAME,
	STATE_MENU,
	STATE_GAMEOVER,
	STATE_CREDITS,
	STATE_VICTORY,

	N_STATES
} STATE;

typedef enum {
	SPRITE_PLAYER,
	SPRITE_HEART,
	SPRITE_CREDIT_SHEEP,
	SPRITE_COUNTDOWN,

	N_SPRITE_TYPES
} SPRITE_TYPE;

#endif
