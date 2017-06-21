#ifndef SPRITE_PLAYER_H
#define SPRITE_PLAYER_H

#include "main.h"

DECLARE_SPRITE(SPRITE_PLAYER);

#define GROUNDED_BIT 7u
#define DOUBLE_JUMP_BIT 6u
#define JUMP_STRENGTH 12u
#define WALK_SPEED 2

typedef struct {
	UINT8 Flags;
	UINT8 Jump;
	// more data, like lives, etc.
} PlayerData;

#endif