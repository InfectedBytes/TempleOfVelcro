#pragma bank=2
#include "SpritePlayer.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Utils.h"
UINT8 bank_SPRITE_PLAYER = 2;

int pal = 0;

void Start_SPRITE_PLAYER() {
	PAL0;
	COLLISION_BORDER(2, 3, 12, 13);
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
		BIT_SET(data->Flags, GROUNDED_BIT);
		BIT_CLEAR(data->Flags, DOUBLE_JUMP_BIT);
	} else {
		BIT_CLEAR(data->Flags, GROUNDED_BIT);
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
		if (BIT_GET(data->Flags, GROUNDED_BIT)) {
			data->Jump = JUMP_STRENGTH;
		} else if (!BIT_GET(data->Flags, DOUBLE_JUMP_BIT)) {
			BIT_SET(data->Flags, DOUBLE_JUMP_BIT);
			data->Jump = JUMP_STRENGTH;
		}
	}
}

void Destroy_SPRITE_PLAYER() {
}