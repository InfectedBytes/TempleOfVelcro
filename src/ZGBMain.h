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

#define BANK_MISC_TILES 7


// level selection
#define LEVEL_FIRST_BANK 10
#define LEVEL_COUNT 3
#define LEVEL_MAP_ADDRESS 0x4000
#define LEVEL_HEIGHT 18

#define LEVEL_WIDTH levelWidths[currentLevel]

extern UINT8 currentLevel;
extern UINT16 levelWidths[];

// animated background
// one tile costs 16 bytes => 4 tiles = 64 bytes
#define TORCH_SPEED 5

void AnimBkg_Setup(UINT8 _bkgBank, unsigned char *_animBkgTiles, UINT8 _waterfallBase, UINT8 _waterfallEnd, UINT8 _torchBase);
void AnimBkg_Update(void);
void ReplaceTiles(UINT8 bank, UINT8 address, UINT8 tiles, UINT8* data, UINT8 offset);

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
	STATE_INTRO,
	STATE_OUTRO,
	STATE_DIFFICULTY,

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
