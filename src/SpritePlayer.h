#ifndef SPRITE_PLAYER_H
#define SPRITE_PLAYER_H

#include "main.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "ZGBMain.h"

DECLARE_SPRITE(SPRITE_PLAYER);

#define GRAVITY 5
#define VELCRO_GRAVITY -3
#define MAX_HEALTH 3
#define WALK_ANIM_SPEED 30
// Helper macro used to switch the player's frames
#define UPDATE_FRAME_CACHE ReplaceFrames(player->first_tile, player->size, 14, spriteSheets[data->Health - 1], 3)

#define GROUNDED_BIT 7u
#define DOUBLE_JUMP_BIT 6u
#define JUMP_STRENGTH -14
#define VELCRO_JUMP_STRENGTH 8
// This is only half of the walkspeed. It is applied in two small steps, in order to prevent glitching
#define WALK_SPEED 4

// Time the player is invincible after he got hit
#define INVINCIBLE_TIME 50
// Time the movement is freezed after player took damage (damage animation)
#define DAMAGE_FREEZE_TIME 10
// gameover animation time
#define GAMEOVER_ANIM_TIME 80

void HealPlayer();
void DamagePlayer();
UINT8 HitsPlayer(struct Sprite* sprite);

typedef struct {
	UINT8 Flags;
	INT8 Jump;
	UINT8 Health;
	UINT8 Invincible;
} PlayerData;

typedef enum {
	IDLE, // only for debugging
	WALK, 
	JUMP,
	FALL,
	DAMAGE,
	DEAD
} AnimationState;

#endif