#ifndef SPRITE_PLAYER_H
#define SPRITE_PLAYER_H

#include "main.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "ZGBMain.h"

DECLARE_SPRITE(SPRITE_PLAYER);

#define MAX_HEALTH 2
#define WALK_ANIM_SPEED 40
// Helper macro used to switch the player's frames
#define UPDATE_FRAME_CACHE(health) ReplaceFrames(scroll_target->first_tile, scroll_target->size, 7, sheep, sheepBank, 7*(health));

#define GROUNDED_BIT 7u
#define DOUBLE_JUMP_BIT 6u
#define JUMP_STRENGTH -12
#define VELCRO_JUMP_STRENGTH 8
// This is only half of the walkspeed. It is applied in two small steps, in order to prevent glitching
#define WALK_SPEED 4

// Time the player is invincible after he got hit
#define INVINCIBLE_TIME 50
// Time the movement is freezed after player took damage (damage animation)
#define DAMAGE_FREEZE_TIME 10

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
	IDLE, // only for debug
	WALK, 
	//JUMP,
	DAMAGE
} AnimationState;

#endif