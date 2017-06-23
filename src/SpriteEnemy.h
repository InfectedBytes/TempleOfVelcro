#ifndef SPRITE_ENEMY_H
#define SPRITE_ENEMY_H

#include "main.h"

DECLARE_SPRITE(SPRITE_ENEMY);

#define MOVE_DIR_BIT 7u
#define ENEMY_WALK_SPEED 1

typedef struct {
	UINT8 Flags;
} EnemyData;

#endif