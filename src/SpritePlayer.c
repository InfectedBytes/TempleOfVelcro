#pragma bank=2
#include "SpritePlayer.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Utils.h"
#include "Scroll.h"
#include "Math.h"
UINT8 bank_SPRITE_PLAYER = 2;

int pal = 0;
const UINT8 anim[] = {4, 0, 1, 2, 3};

void Start_SPRITE_PLAYER() {
	PAL0;
	COLLISION_BORDER(2, 3, 12, 13);
	SetSpriteAnim(THIS, anim, 5);
	scroll_target = THIS;
}

void Update_SPRITE_PLAYER() {
	PlayerData* data = (PlayerData*)THIS->custom_data;
	// apply jump
	if (data->Jump != 0) {
		data->Jump--;
		if (TranslateSprite(THIS, 0, -data->Jump))
			data->Jump = 0; // we hit a collider -> stop jump
	}
	// apply gravity and check if sprite is grounded
	if (TranslateSprite(THIS, 0, 3)) {
		SET_BIT(data->Flags, GROUNDED_BIT);
		UNSET_BIT(data->Flags, DOUBLE_JUMP_BIT);
	} else {
		UNSET_BIT(data->Flags, GROUNDED_BIT);
	}

	// handle input
	if (KEY_PRESSED(J_LEFT)) {
		TranslateSprite(THIS, -WALK_SPEED, 0);
	}
	if (KEY_PRESSED(J_RIGHT)) {
		TranslateSprite(THIS, WALK_SPEED, 0);
	}

	// for testing: toggle palette
	if (KEY_TICKED(J_B)) PAL(pal = 1 - pal);
	// jump
	if (KEY_TICKED(J_A)) {
		if (GET_BIT(data->Flags, GROUNDED_BIT)) {
			data->Jump = JUMP_STRENGTH;
		} else if (!GET_BIT(data->Flags, DOUBLE_JUMP_BIT)) {
			SET_BIT(data->Flags, DOUBLE_JUMP_BIT);
			data->Jump = JUMP_STRENGTH;
		}
	}
}

void Destroy_SPRITE_PLAYER() {
}