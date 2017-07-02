#pragma bank=2
#include "SpriteHeart.h"
#include "SpriteManager.h"
#include "SpritePlayer.h"

UINT8 bank_SPRITE_HEART = 2;

UINT8 anim[] = { 4, 0, 1, 2, 1 };
static UINT8 pickup_anim[] = { 4, 3, 4, 5, 4 };

void Start_SPRITE_HEART() {
	SetSpriteAnim(THIS, anim, 7);
	THIS->custom_data[0] = 255;
}

void Update_SPRITE_HEART() {
	if (THIS->custom_data[0] == 255) {
		if (HitsPlayer(THIS)) {
			HealPlayer();
			SetSpriteAnim(THIS, pickup_anim, 20);
			THIS->custom_data[0] = 40;
		}
	} else if (THIS->custom_data[0] != 0) {
		THIS->custom_data[0]--;
		THIS->x = scroll_target->x - 6;
		if (THIS->y < scroll_target->y - 3) THIS->y+=3;
		else if (THIS->y > scroll_target->y + 3) THIS->y-=3;
	} else {
		SpriteManagerRemoveSprite(THIS);
	}
}

void Destroy_SPRITE_HEART() {
}