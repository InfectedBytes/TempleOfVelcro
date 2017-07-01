#pragma bank=2
#include "SpriteEnemy.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Utils.h"
#include "SpritePlayer.h"
UINT8 bank_SPRITE_ENEMY = 2;

void Start_SPRITE_ENEMY() {
	PAL0;
}

void Update_SPRITE_ENEMY() {
	if (HitsPlayer(THIS))
		DamagePlayer();
}

void Destroy_SPRITE_ENEMY() {
}