#pragma bank=2
#include "SpriteHeart.h"
#include "SpriteManager.h"
#include "SpritePlayer.h"

UINT8 bank_SPRITE_HEART = 2;

UINT8 anim[] = { 4, 0, 1, 2, 1 };

void Start_SPRITE_HEART() {
	SetSpriteAnim(THIS, anim, 7);
}

void Update_SPRITE_HEART() {
	if (HitsPlayer(THIS)) {
		HealPlayer();
		SpriteManagerRemoveSprite(THIS);
	}
}

void Destroy_SPRITE_HEART() {
}