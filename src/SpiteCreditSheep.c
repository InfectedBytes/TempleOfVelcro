#pragma bank=4
#include "SpriteCreditSheep.h"
#include "SpriteManager.h"
#include "SpritePlayer.h"

UINT8 bank_SPRITE_CREDIT_SHEEP = 4;

#define CREDIT_START_JUMP_TIME 40

extern UINT16 creditSheepCounter;

extern UINT8 walk_anim[];
extern UINT8 jump_anim[];
extern UINT8 fall_anim[];

static UINT8 jump; // only one sheep at a time is jumping

void Start_SPRITE_CREDIT_SHEEP() {
	THIS->lim_x = 32;
	SetSpriteAnim(THIS, walk_anim, 10);
}

void Update_SPRITE_CREDIT_SHEEP() {
	THIS->x+=2;
	if (THIS->x < 20) return;
	if (THIS->x == 70) {
		SetSpriteAnim(THIS, jump_anim, 5);
		jump = 12;
	}
	if (jump != 0) {
		TranslateSprite(THIS, 0, -jump);
		if (--jump == 0) {
			creditSheepCounter++;
			SetSpriteAnim(THIS, fall_anim, 5);
		}
	}
	if (TranslateSprite(THIS, 0, 2)) // gravity
		SetSpriteAnim(THIS, walk_anim, 10);
}

void Destroy_SPRITE_CREDIT_SHEEP() {
}