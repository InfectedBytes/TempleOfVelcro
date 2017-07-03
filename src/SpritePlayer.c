#pragma bank=2
#include "SpritePlayer.h"
#include "Keys.h"
#include "ZGBMain.h"
#include "Utils.h"
#include "Scroll.h"
#include "Math.h"
#include "StateGame.h"
#include "Sound.h"
#include "../res/src/SoundEffects.h"
#include "../res/src/sheep1.h"
#include "../res/src/sheep2.h"
#include "../res/src/sheep3.h"
#include "../res/src/map.h"
UINT8 bank_SPRITE_PLAYER = 2;

#define COLL_Y 12

static UINT8* spriteSheets[] = { sheep1, sheep2, sheep3 };

static UINT8 idle_anim[] = { 3, 1, 3, 3 };
static UINT8 walk_anim[] = { 12, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 4, 3 };
static UINT8 damage_anim[] = {2, 12, 13};
static UINT8 gameover_anim[] = { 2, 8, 9};
static UINT8 jump_anim[] = { 3, 5, 6, 6 };
static UINT8 fall_anim[] = { 1, 7 };
static UINT8 win_anim[] = { 3, 11, 10, 11 };

// these variables are always pointing to the current player
static struct Sprite* player;
static PlayerData* data;

static AnimationState lastState, currentState;
static UINT8 gameoverTimer;

static UINT8 normalPalette = PAL_DEF(2, 0, 1, 3);
static UINT8 invertPalette = PAL_DEF(3, 2, 1, 0);

// for debugging: toggle autorun
static UINT8 autorun;

// difficulty values
static UINT8 speedSetting;
static UINT8 gravitySetting;

static void SetAnimationState(AnimationState state) {
	lastState = currentState;
	currentState = state;
	switch (state) {
		case IDLE:   SetSpriteAnim(player, idle_anim, 10); break;
		case WALK:   SetSpriteAnim(player, walk_anim, WALK_ANIM_SPEED); break;
		case JUMP:   SetSpriteAnim(player, jump_anim, 10); break;
		case FALL:   SetSpriteAnim(player, fall_anim, 10); break;
		case DAMAGE: SetSpriteAnim(player, damage_anim, 10); break;
		case DEAD:   SetSpriteAnim(player, gameover_anim, 10); break;
		case WIN:    SetSpriteAnim(player, win_anim, 8); break;
	}
}

static void RevertAnimationState() {
	SetAnimationState(lastState);
}

static AnimationState GetAnimationState(void) {
	return currentState;
}

static void DrawGui(INT16 metersLeft) {
	UINT8 i;

	BOTTOM_LINES(1);

	// print meters left
	PRINT_POS(14, 0);
	Printf(" %um    ", (UINT16)metersLeft);

	// print health
	PRINT_POS(1, 0);
	for (i = 0; i < data->Health; i++) Printf("*");
	for (i = data->Health; i < MAX_HEALTH; i++) Printf("°");
}

void KillPlayer() {
	data->Health = 0;
	gameoverTimer = GAMEOVER_ANIM_TIME;
}

// If player is not at full health, this function will increment the player's health and updates the frame cache.
// Caution: THIS is not necessarily the player!
void HealPlayer() {
	if (data->Health < MAX_HEALTH) {
		data->Health++;
		UPDATE_FRAME_CACHE;
	}
	PLAYFX(player_heal);
}

// If player is not invincible, this function will decrement the player's health and updates the frame cache.
// Caution: THIS is not necessarily the player!
void DamagePlayer() {
	if (data->Invincible) return;
	if (data->Health == 1) {
		KillPlayer();
		return;
	}
	SetAnimationState(DAMAGE);
	PLAYFX(player_damage);
	data->Health--;
	data->Invincible = INVINCIBLE_TIME + DAMAGE_FREEZE_TIME;
}

// Checks whether the provided sprite collides with the player or not.
// Caution: THIS is not necessarily the player!
UINT8 HitsPlayer(struct Sprite* sprite) {
	return CheckCollision(sprite, player);
}

// Checks for velcros and updates the internal states if we switch between modes.
static UINT8 UpdateVelcro() {
	UINT8 tx, ty, trigger;
	trigger = FIND_TOP_TRIGGER(THIS, TILE_VELCRO, TILE_VELCRO_MASK, &tx, &ty);
	if (trigger && !GET_BIT_MASK(THIS->flags, OAM_HORIZONTAL_FLAG)) {
		SET_BIT_MASK(THIS->flags, OAM_HORIZONTAL_FLAG);
		THIS->coll_y = 0;
		THIS->y += COLL_Y;
	} else if (!trigger && GET_BIT_MASK(THIS->flags, OAM_HORIZONTAL_FLAG)) {
		UNSET_BIT_MASK(THIS->flags, OAM_HORIZONTAL_FLAG);
		THIS->coll_y = COLL_Y;
		THIS->y -= COLL_Y;
	}
	return trigger;
}

// Checks for trigger collisions (slopes, spikes, etc.)
static void UpdateTriggers() {
	UINT16 tx, ty;
	UINT8 trigger = FIND_TRIGGER(THIS, TILE_TRIGGERS, TILE_TRIGGERS_MASK, &tx, &ty);
	switch (trigger) {
	case TILE_SLOPE_UP:
		THIS->y -= 16;
		break;
	case TILE_SLOP_DOWN:
		THIS->y += 12;
		break;
	case TILE_SPIKES:
		DamagePlayer();
		break;
	case TILE_INSTANT_DEATH:
		KillPlayer();
		break;
	}
}

static void PlayJumpSound(UINT8 velcro) {
	if (velcro) {
		PLAYFX(player_jump_velcro);
	} else {
		PLAYFX(player_jump);
	}
}

void Start_SPRITE_PLAYER() {
	player = THIS;
	data = (PlayerData*)THIS->custom_data;
	data->Health = MAX_HEALTH;
	data->Flags = 0;
	data->Jump = 0;
	data->Invincible = 0;
	OBP1_REG = normalPalette;
	PAL1;
	COLLISION_BORDER(6, COLL_Y, 10, 20);
	THIS->lim_x = THIS->lim_y = 100;
	scroll_target = THIS;
	lastState = currentState = IDLE;
	SetAnimationState(currentState);
	// difficulty
	speedSetting = WALK_SPEED + GetDifficulty();
	gravitySetting = GRAVITY + GetDifficulty();
}

void Update_SPRITE_PLAYER() {
	UINT8 velcro;
	INT16 metersLeft;
	PlayerData* data = (PlayerData*)THIS->custom_data;

	// handle dying animation
	if (data->Health == 0) {
		SetAnimationState(DEAD);
		if (gameoverTimer-- == 0) SetState(STATE_GAMEOVER);
		else if (gameoverTimer > (GAMEOVER_ANIM_TIME >> 1) + 5) THIS->y--;
		else THIS->y+=2;
		return;
	}

	// calculate meters left
	metersLeft = mapWidth - (THIS->x >> 3) - 10;
	if (metersLeft < 0) metersLeft = 0;

	// draw information on window
	DrawGui(metersLeft);

	// check if finished
	if (0 == metersLeft) {
		if (WIN != GetAnimationState()) {
			OBP1_REG = normalPalette;
			gameoverTimer = WIN_ANIM_TIME;
		}

		SetAnimationState(WIN);
		TranslateSprite(THIS, 0, GRAVITY); // we don't want to stop mid air

		if (gameoverTimer-- == 0) {
			SetState(STATE_GAMEOVER);
		}

		return;
	}

	velcro = UpdateVelcro();
	UpdateTriggers();
	if (data->Health == 0) return;

	// handle invincible animation
	if (data->Invincible > 0) {
		data->Invincible--;
		// blink effect
		OBP1_REG = (data->Invincible & 4) ? invertPalette : normalPalette;
		if (data->Invincible > INVINCIBLE_TIME) // freeze and animate
			return;
		if (data->Health == 0) {
			SetState(STATE_GAMEOVER);
			HIDE_WIN;
			return;
		}
		if ((UINT16)data->Invincible == INVINCIBLE_TIME) { // set new frames
			UPDATE_FRAME_CACHE;
			RevertAnimationState();
		}
	}

	// apply jump
	if (data->Jump != 0) {
		if(data->Jump > 0) data->Jump--;
		else data->Jump++;
		if (data->Jump <= -8) {
			TranslateSprite(THIS, 0, -8);
			if (TranslateSprite(THIS, 0, data->Jump + 8))
				data->Jump = 0; // we hit a collider -> stop jump
		} else {
			if (TranslateSprite(THIS, 0, data->Jump))
				data->Jump = 0; // we hit a collider -> stop jump
		}
	}

	// apply gravity and check if sprite is grounded
	if (TranslateSprite(THIS, 0, velcro ? VELCRO_GRAVITY : (gravitySetting + delta_time))) {
		if (!velcro && !GET_BIT(data->Flags, GROUNDED_BIT)) {
			PLAYFX(player_grounded);
		}
		SET_BIT(data->Flags, GROUNDED_BIT);
		UNSET_BIT(data->Flags, DOUBLE_JUMP_BIT);
		SetAnimationState(WALK);
	} else {
		UNSET_BIT(data->Flags, GROUNDED_BIT);
		if(data->Jump >= -gravitySetting) SetAnimationState(FALL);
	}

	// for debugging: toggle autorun
	if (KEY_TICKED(J_B)) autorun = 1 - autorun;
	if (autorun) {
		UNSET_BIT_MASK(THIS->flags, OAM_VERTICAL_FLAG);
		TranslateSprite(THIS, speedSetting + delta_time, 0);
		TranslateSprite(THIS, speedSetting + delta_time, 0);
	} else {
		// handle input
		if (KEY_PRESSED(J_LEFT)) {
			SET_BIT_MASK(THIS->flags, OAM_VERTICAL_FLAG);
			// to prevent glitching, we just translate in two small steps instead of one large step
			TranslateSprite(THIS, -(speedSetting + delta_time), 0);
			TranslateSprite(THIS, -(speedSetting + delta_time), 0);
		} else if (KEY_PRESSED(J_RIGHT)) {
			UNSET_BIT_MASK(THIS->flags, OAM_VERTICAL_FLAG);
			TranslateSprite(THIS, speedSetting + delta_time, 0);
			TranslateSprite(THIS, speedSetting + delta_time, 0);
		} else {
			SetAnimationState(IDLE);
		}
	}

	// jump
	if (KEY_TICKED(J_A)) {
		if (GET_BIT(data->Flags, GROUNDED_BIT)) {
			PlayJumpSound(velcro);
			SetAnimationState(JUMP);
			if (velcro) data->Jump = VELCRO_JUMP_STRENGTH;
			else data->Jump = JUMP_STRENGTH;
		} else if (!GET_BIT(data->Flags, DOUBLE_JUMP_BIT)) {
			PlayJumpSound(FALSE);
			SetAnimationState(JUMP);
			SET_BIT(data->Flags, DOUBLE_JUMP_BIT);
			data->Jump = JUMP_STRENGTH;
		}
	}
}

void Destroy_SPRITE_PLAYER() {
}
