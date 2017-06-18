#pragma bank=2
#include "SpritePlayer.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
UINT8 bank_SPRITE_PLAYER = 2;

int pal = 0;

void Start_SPRITE_PLAYER() {
	PAL0;
	COLLISION_BORDER(2, 0, 12, 16);
}

void Update_SPRITE_PLAYER() {
	if (KEY_PRESSED(J_UP)) {
		TranslateSprite(THIS, 0, -1);
	}
	if (KEY_PRESSED(J_DOWN)) {
		TranslateSprite(THIS, 0, 1);
	}
	if (KEY_PRESSED(J_LEFT)) {
		TranslateSprite(THIS, -1, 0);
	}
	if (KEY_PRESSED(J_RIGHT)) {
		TranslateSprite(THIS, 1, 0);
	}

	if (KEY_TICKED(J_A)) PAL(pal = 1 - pal);
}

void Destroy_SPRITE_PLAYER() {
}