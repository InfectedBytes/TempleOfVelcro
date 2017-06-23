#pragma bank=2
#include "SpriteEnemy.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Utils.h"
#include "Math.h"
UINT8 bank_SPRITE_ENEMY = 2;

void Start_SPRITE_ENEMY() {
	PAL0;
}

void Update_SPRITE_ENEMY() {
	UINT16 trigger, tx, ty;
	EnemyData* data = (EnemyData*)THIS->custom_data;

	if (GET_BIT(data->Flags, MOVE_DIR_BIT)) {
		TranslateSprite(THIS, ENEMY_WALK_SPEED, 0);
	} else {
		TranslateSprite(THIS, -ENEMY_WALK_SPEED, 0);
	}

	// find control tiles (id 100 and 101)
	trigger = FIND_TRIGGER(THIS, 100, 1, &tx, &ty);
	if (trigger == 100) SET_BIT(data->Flags, MOVE_DIR_BIT);
	else if (trigger == 101) UNSET_BIT(data->Flags, MOVE_DIR_BIT);
}

void Destroy_SPRITE_ENEMY() {
}