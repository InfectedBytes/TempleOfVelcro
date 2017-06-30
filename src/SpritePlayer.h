#ifndef SPRITE_PLAYER_H
#define SPRITE_PLAYER_H

#include "main.h"

DECLARE_SPRITE(SPRITE_PLAYER);

#define GROUNDED_BIT 7u
#define DOUBLE_JUMP_BIT 6u
#define JUMP_STRENGTH -12
#define VELCRO_JUMP_STRENGTH 8
#define WALK_SPEED 4

typedef struct {
	UINT8 Flags;
	INT8 Jump;
	UINT8 Health;
	// more data, like lives, etc.
} PlayerData;

#endif