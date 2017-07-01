#ifndef SPRITE_PLAYER_H
#define SPRITE_PLAYER_H

#include "main.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "ZGBMain.h"

DECLARE_SPRITE(SPRITE_PLAYER);

#define MAX_HEALTH 2
#define UPDATE_FRAME_CACHE(health) ReplaceFrames(scroll_target->first_tile, scroll_target->size, 8, sheep, sheepBank, 8*(health));

#define GROUNDED_BIT 7u
#define DOUBLE_JUMP_BIT 6u
#define JUMP_STRENGTH -12
#define VELCRO_JUMP_STRENGTH 8
#define WALK_SPEED 4

#define INVINCIBLE_TIME 50

void HealPlayer();
void DamagePlayer();
UINT8 HitsPlayer(struct Sprite* sprite);

typedef struct {
	UINT8 Flags;
	INT8 Jump;
	UINT8 Health;
	UINT8 Invincible;
	// more data, like lives, etc.
} PlayerData;

#endif