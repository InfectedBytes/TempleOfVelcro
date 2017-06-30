#pragma bank=2
#include "SpritePlayer.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Utils.h"
#include "Scroll.h"
#include "Math.h"
#include "StateGame.h"
#include "main.h"
UINT8 bank_SPRITE_PLAYER = 2;

static UINT8 idle_anim[] = { 2, 2, 4 };
static UINT8 walk_anim[] = { 4, 0, 1, 2, 3 };

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
	PAL1;
	COLLISION_BORDER(2, 8, 20, 24);
	scroll_target = THIS;
}

void Update_SPRITE_PLAYER() {
	UINT8 velcro;
	PlayerData* data = (PlayerData*)THIS->custom_data;
	BOTTOM_LINES(1); // for HUD

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
		TranslateSprite(THIS, -(WALK_SPEED << delta_time), 0);
	} else if (KEY_PRESSED(J_RIGHT)) {
		UNSET_BIT_MASK(THIS->flags, OAM_VERTICAL_FLAG);
		SetSpriteAnim(THIS, walk_anim, 15);
		TranslateSprite(THIS, WALK_SPEED << delta_time, 0);
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
	Printf("%d    ", (UINT16)velcro);
}

void Destroy_SPRITE_PLAYER() {
}