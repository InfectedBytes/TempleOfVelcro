#pragma bank=2
#include "SpriteSparkles.h"
#include "SpriteManager.h"

UINT8 bank_SPRITE_SPARKLES = 2;
static UINT8 sparkle_anim[4]; // no array initializer, because we want to hold it in bank memory
void Start_SPRITE_SPARKLES() {
	sparkle_anim[0] = 3;
	sparkle_anim[1] = 0;
	sparkle_anim[2] = 1;
	sparkle_anim[3] = 2;
	SetSpriteAnim(THIS, sparkle_anim, 10);
}

void Update_SPRITE_SPARKLES() {
}

void Destroy_SPRITE_SPARKLES() {
}