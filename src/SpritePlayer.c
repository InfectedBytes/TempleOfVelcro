#pragma bank=2
#include "SpritePlayer.h"
#include "Keys.h"
#include "ZGBMain.h"
#include "Utils.h"
#include "Scroll.h"
#include "Math.h"
#include "StateGame.h"
#include "Sound.h"
#include "../res/src/sheep.h"
UINT8 bank_SPRITE_PLAYER = 2;

#define COLL_Y 12

static UINT8 idle_anim[] = { 3, 1, 3, 5 };
static UINT8 walk_anim[] = { 8, 0, 1, 2, 3, 4, 5, 6, 7 };

// these variables are always pointing to the current player
static struct Sprite* player;
static PlayerData* data;

static UINT8 normalPalette = PAL_DEF(2, 0, 1, 3);
static UINT8 invertPalette = PAL_DEF(3, 2, 1, 0);

// for debugging: toggle autorun
static UINT8 autorun;

// If player is not at full health, this function will increment the player's health and updates the frame cache.
void HealPlayer() {
	if (data->Health < MAX_HEALTH) {
		data->Health++;
		UPDATE_FRAME_CACHE(MAX_HEALTH - data->Health);
	}
}

// If player is not invincible, this function will decrement the player's health and updates the frame cache.
void DamagePlayer() {
	if (data->Invincible) return;
	if (data->Health != 0) {
		// TODO: damage animation
		data->Health--;
		data->Invincible = INVINCIBLE_TIME;
		UPDATE_FRAME_CACHE(MAX_HEALTH - data->Health);
	} else {
		// TODO: gameover animation + screen
	}
}

// Checks whether the provided sprite collides with the player or not.
UINT8 HitsPlayer(struct Sprite* sprite) {
	return CheckCollision(sprite, player);
}

// Checks for velcros and updates the internal states if we switch between modes.
static UINT8 UpdateVelcro() {
	UINT8 tx, ty, trigger;
	trigger = FIND_TOP_TRIGGER(THIS, TILE_VELCRO, TILE_VELCRO_MASK, &tx, &ty);
	if (trigger && !GET_BIT_MASK(THIS->flags, OAM_HORIZONTAL_FLAG)) {
		SET_BIT_MASK(THIS->flags, OAM_HORIZONTAL_FLAG);
		THIS->coll_y = 0;
		THIS->y += COLL_Y;
	} else if (!trigger && GET_BIT_MASK(THIS->flags, OAM_HORIZONTAL_FLAG)) {
		UNSET_BIT_MASK(THIS->flags, OAM_HORIZONTAL_FLAG);
		THIS->coll_y = COLL_Y;
		THIS->y -= COLL_Y;
	}
	return trigger;
}

// Checks for trigger collisions (slopes, spikes, etc.)
static void UpdateTriggers() {
	UINT16 tx, ty;
	UINT8 trigger = FIND_TRIGGER(THIS, TILE_TRIGGERS, TILE_TRIGGERS_MASK, &tx, &ty);
	switch (trigger) {
	case TILE_SLOPE_UP:
		THIS->y -= 12;
		break;
	case TILE_SLOP_DOWN:
		THIS->y += 12;
		break;
	case TILE_SPIKES:
		DamagePlayer();
		break;
	}
}

static void PlayJumpSound(UINT8 velcro) {
	if (velcro)
		PlayFx(CHANNEL_4, 20, 0x06, 0xF2, 0x7D, 0xC0);
	else
		PlayFx(CHANNEL_1, 20, 0x56, 0x81, 0xF2, 0x92, 0x84);
}

void Start_SPRITE_PLAYER() {
	player = THIS;
	data = (PlayerData*)THIS->custom_data;
	data->Health = 2;
	data->Flags = 0;
	data->Jump = 0;
	data->Invincible = 0;
	OBP1_REG = normalPalette;
	PAL1;
	COLLISION_BORDER(6, COLL_Y, 10, 20);
	scroll_target = THIS;
}

void Update_SPRITE_PLAYER() {
	UINT8 velcro, i;
	struct Sprite* sprite;
	PlayerData* data = (PlayerData*)THIS->custom_data;
	BOTTOM_LINES(1); // for HUD

	if (data->Invincible > 0) {
		data->Invincible--;
		OBP1_REG = (data->Invincible & 4) ? invertPalette : normalPalette;
	}

	velcro = UpdateVelcro();
	UpdateTriggers();

	// apply jump
	if (data->Jump != 0) {
		if(data->Jump > 0) data->Jump--;
		else data->Jump++;
		if (data->Jump <= -8) {
			TranslateSprite(THIS, 0, -8);
			if (TranslateSprite(THIS, 0, data->Jump + 8))
				data->Jump = 0; // we hit a collider -> stop jump
		} else {
			if (TranslateSprite(THIS, 0, data->Jump))
				data->Jump = 0; // we hit a collider -> stop jump
		}
	}

	// apply gravity and check if sprite is grounded
	if (TranslateSprite(THIS, 0, velcro ? -3 : 3)) {
		SET_BIT(data->Flags, GROUNDED_BIT);
		UNSET_BIT(data->Flags, DOUBLE_JUMP_BIT);
	} else {
		UNSET_BIT(data->Flags, GROUNDED_BIT);
	}

	// for debugging: toggle autorun
	if (KEY_TICKED(J_B)) autorun = 1 - autorun;
	if (autorun) {
		UNSET_BIT_MASK(THIS->flags, OAM_VERTICAL_FLAG);
		SetSpriteAnim(THIS, walk_anim, 15);
		TranslateSprite(THIS, WALK_SPEED + delta_time, 0);
		TranslateSprite(THIS, WALK_SPEED + delta_time, 0);
	} else {
		// handle input
		if (KEY_PRESSED(J_LEFT)) {
			SET_BIT_MASK(THIS->flags, OAM_VERTICAL_FLAG);
			SetSpriteAnim(THIS, walk_anim, 15);
			// to prevent glitching, we just translate in two small steps instead of one large step
			TranslateSprite(THIS, -(WALK_SPEED + delta_time), 0);
			TranslateSprite(THIS, -(WALK_SPEED + delta_time), 0);
		} else if (KEY_PRESSED(J_RIGHT)) {
			UNSET_BIT_MASK(THIS->flags, OAM_VERTICAL_FLAG);
			SetSpriteAnim(THIS, walk_anim, 15);
			TranslateSprite(THIS, WALK_SPEED + delta_time, 0);
			TranslateSprite(THIS, WALK_SPEED + delta_time, 0);
		} else {
			SetSpriteAnim(THIS, idle_anim, 15);
		}
	}

	// jump
	if (KEY_TICKED(J_A)) {
		if (GET_BIT(data->Flags, GROUNDED_BIT)) {
			PlayJumpSound(velcro);
			if (velcro) data->Jump = VELCRO_JUMP_STRENGTH;
			else data->Jump = JUMP_STRENGTH;
		} else if (!GET_BIT(data->Flags, DOUBLE_JUMP_BIT)) {
			PlayJumpSound(FALSE);
			SET_BIT(data->Flags, DOUBLE_JUMP_BIT);
			data->Jump = JUMP_STRENGTH;
		}
	}

	PRINT_POS(0, 0);
	Printf("%dm    ", (UINT16)((THIS->x >> 3) - 4));
	PRINT_POS(10, 0);
	Printf("Lives:%d  ", (UINT16)data->Health);
}

void Destroy_SPRITE_PLAYER() {
}