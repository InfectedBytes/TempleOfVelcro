#pragma bank=2
#include "SpritePlayer.h"
#include "Keys.h"
#include "ZGBMain.h"
#include "Utils.h"
#include "Scroll.h"
#include "Math.h"
#include "StateGame.h"
UINT8 bank_SPRITE_PLAYER = 2;

static UINT8 idle_anim[] = { 2, 2, 4 };
static UINT8 walk_anim[] = { 4, 0, 1, 2, 3 };

// these variables are always pointing to the current player
static struct Sprite* player;
static PlayerData* data;

static UINT8 normalPalette = PAL_DEF(2, 0, 1, 3);
static UINT8 invertPalette = PAL_DEF(3, 2, 1, 0);

void HealPlayer() {
	data->Health++;
}

void DamagePlayer() {
	if (data->Invincible) return;
	data->Health--;
	data->Invincible = INVINCIBLE_TIME;
}

UINT8 HitsPlayer(struct Sprite* sprite) {
	return CheckCollision(sprite, player);
}

static UINT8 UpdateVelcro() {
	UINT16 tx, ty;
	UINT8 trigger = FIND_TRIGGER(THIS, 5, 0, &tx, &ty);
	if (trigger && !GET_BIT_MASK(THIS->flags, OAM_HORIZONTAL_FLAG)) {
		SET_BIT_MASK(THIS->flags, OAM_HORIZONTAL_FLAG);
		THIS->coll_y = 0;
		THIS->y += 8;
	} else if (!trigger && GET_BIT_MASK(THIS->flags, OAM_HORIZONTAL_FLAG)) {
		UNSET_BIT_MASK(THIS->flags, OAM_HORIZONTAL_FLAG);
		THIS->coll_y = 8;
		THIS->y -= 8;
	}
	return trigger;
}

void Start_SPRITE_PLAYER() {
	player = THIS;
	data = (PlayerData*)THIS->custom_data;
	data->Health = 3;
	data->Flags = 0;
	data->Jump = 0;
	data->Invincible = 0;
	OBP1_REG = normalPalette;
	PAL1;
	COLLISION_BORDER(2, 8, 20, 24);
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

	// apply jump
	if (data->Jump != 0) {
		if(data->Jump > 0) data->Jump--;
		else data->Jump++;
		if (TranslateSprite(THIS, 0, data->Jump))
			data->Jump = 0; // we hit a collider -> stop jump
	}
	// apply gravity and check if sprite is grounded
	if (TranslateSprite(THIS, 0, velcro ? -3 : 3)) {
		SET_BIT(data->Flags, GROUNDED_BIT);
		UNSET_BIT(data->Flags, DOUBLE_JUMP_BIT);
	} else {
		UNSET_BIT(data->Flags, GROUNDED_BIT);
	}

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

	// jump
	if (KEY_TICKED(J_A)) {
		if (GET_BIT(data->Flags, GROUNDED_BIT)) {
			if (velcro) data->Jump = VELCRO_JUMP_STRENGTH;
			else data->Jump = JUMP_STRENGTH;
		} else if (!GET_BIT(data->Flags, DOUBLE_JUMP_BIT)) {
			SET_BIT(data->Flags, DOUBLE_JUMP_BIT);
			data->Jump = JUMP_STRENGTH;
		}
	}

	PRINT_POS(0, 0);
	Printf("%d    ", (UINT16)((THIS->x >> 3) - 4));
	PRINT_POS(10, 0);
	Printf("Lives:%d  ", (UINT16)data->Health);
}

void Destroy_SPRITE_PLAYER() {
}