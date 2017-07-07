#pragma bank=4
#include "SpriteCreditSheep.h"
#include "SpriteManager.h"
#include "SpritePlayer.h"
#include "Sound.h"
#include "StateCredits.h"
#include "../res/src/SoundEffects.h"

UINT8 bank_SPRITE_CREDIT_SHEEP = 4;

#define COLL_Y 12

extern UINT16 creditSheepCounter;
extern UINT8 creditsOrTutorial;

extern UINT8 walk_anim[];
extern UINT8 jump_anim[];
extern UINT8 fall_anim[];

static UINT8 jump; // only one sheep at a time is jumping
static UINT8 velcro;

void Start_SPRITE_CREDIT_SHEEP() {
	THIS->lim_x = 32;
	SetSpriteAnim(THIS, walk_anim, 10);
	COLLISION_BORDER(6, COLL_Y, 10, 20);
	velcro = 0;
}

static void UpdateCredits() {
	THIS->x += 2;
	if (THIS->x < 20) return;
	if (THIS->x == 66) {
		SetSpriteAnim(THIS, jump_anim, 5);
		PLAYFX_player_jump;
		jump = 10;
	}
	if (jump != 0) {
		TranslateSprite(THIS, 0, -jump);
		if (--jump == 0) {
			creditSheepCounter++;
			SetSpriteAnim(THIS, fall_anim, 5);
		}
	}
	if (TranslateSprite(THIS, 0, 2)) { // gravity
		if (THIS->anim_data == fall_anim)
			PLAYFX_player_grounded;
		SetSpriteAnim(THIS, walk_anim, 10);
	}
}

// more or less duplicate code...very messy, but time is running out, so we do it the ugly way...
static void UpdateTutorial() {
	THIS->x += 2;
	if (THIS->x < 20) return;
	if (THIS->x == 60) {
		SetSpriteAnim(THIS, jump_anim, 5);
		PLAYFX_player_jump;
		jump = 10;
	} else if (THIS->x == 120) {
		velcro = FALSE;
		SetSpriteAnim(THIS, fall_anim, 5);
		THIS->flags &= ~OAM_HORIZONTAL_FLAG;
		THIS->coll_y = COLL_Y;
	}
	if (jump != 0) {
		TranslateSprite(THIS, 0, -jump);
		if (--jump == 0) {
			THIS->flags |= OAM_HORIZONTAL_FLAG;
			THIS->coll_y = 0;
			creditSheepCounter++;
			velcro = TRUE;
			SetSpriteAnim(THIS, walk_anim, 10);
		}
	}
	if (velcro) {
		TranslateSprite(THIS, 0, -2);
	} else {
		if (TranslateSprite(THIS, 0, 2)) { // gravity
			if (THIS->anim_data == fall_anim)
				PLAYFX_player_grounded;
			SetSpriteAnim(THIS, walk_anim, 10);
		}
	}
}

void Update_SPRITE_CREDIT_SHEEP() {
	if (creditsOrTutorial == CREDITS_MODE)
		UpdateCredits();
	else
		UpdateTutorial();
}

void Destroy_SPRITE_CREDIT_SHEEP() {
}