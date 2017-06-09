#pragma bank=2
#include "SpritePlayer.h"
#include "Keys.h"
#include "SpriteManager.h"
UINT8 bank_SPRITE_PLAYER = 2;

void Start_SPRITE_PLAYER() {
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
}

void Destroy_SPRITE_PLAYER() {
}