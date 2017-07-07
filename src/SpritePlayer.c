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
UINT8 bank_SPRITE_PLAYER = 2;

#define COLL_Y 12

static UINT8* spriteSheets[] = { sheep1, sheep2, sheep3 };

static UINT8 idle_anim[] = { 3, 1, 3, 3 };
UINT8 walk_anim[] = { 12, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 4, 3 }; // not static, because it is also used in credits
static UINT8 damage_anim[] = {2, 12, 13};
static UINT8 gameover_anim[] = { 2, 8, 9};
UINT8 jump_anim[] = { 3, 5, 6, 6 }; // not static, because it is also used in credits
UINT8 fall_anim[] = { 1, 7 }; // not static, because it is also used in credits
static UINT8 victory_anim[] = { 3, 11, 10, 11 };
// music
extern UINT8* death_mod_Data[];

// these variables are always pointing to the current player
static struct Sprite* player;
static PlayerData* data;

static AnimationState lastState, currentState;
static UINT8 gameoverTimer;
static UINT8 countdownTimer;

static UINT8 normalPalette = PAL_DEF(2, 0, 1, 3);
static UINT8 invertPalette = PAL_DEF(3, 2, 1, 0);

// difficulty values
static UINT8 speedSetting;
static UINT8 gravitySetting;

static UINT8 overwriteAutorunSetting;

// used to apply some cheats: increase health, invincible (mainly for debugging)
static UINT8 cheatCounter;
static UINT8 extraGravity; // hacky trick to improve gravity behavior
extern UINT8 paused;

static void SetAnimationState(AnimationState state) {
	lastState = currentState;
	currentState = state;
	switch (state) {
		case IDLE:    SetSpriteAnim(player, idle_anim, 10); break;
		case WALK:    SetSpriteAnim(player, walk_anim, WALK_ANIM_SPEED); break;
		case JUMP:    SetSpriteAnim(player, jump_anim, 10); break;
		case FALL:    SetSpriteAnim(player, fall_anim, 10); break;
		case DAMAGE:  SetSpriteAnim(player, damage_anim, 10); break;
		case DEAD:    SetSpriteAnim(player, gameover_anim, 10); break;
		case VICTORY: SetSpriteAnim(player, victory_anim, 8); break;
	}
}

static void RevertAnimationState(void) {
	SetAnimationState(lastState);
}

static AnimationState GetAnimationState(void) {
	return currentState;
}

static void OverwriteAutorun(UINT8 overwrite, UINT8 autorun) {
	overwriteAutorunSetting = 0;

	if (overwrite) {
		overwriteAutorunSetting |= 1;
	}

	if (autorun) {
		overwriteAutorunSetting |= 2;
	}
}

static UINT8 IsAutorunOverwritten(void) {
	return ((overwriteAutorunSetting & 1) != 0);
}

static UINT8 GetRealAutorun(void)
{
	if (overwriteAutorunSetting & 1) {
		return ((overwriteAutorunSetting & 2) != 0);
	} else {
		return GetAutorun();
	}
}

static void DrawGui(INT16 metersLeft) {
	UINT8 i;
	UINT8 icons[] = { font_idx + 57, font_idx + 58 };

	// print meters left
	PRINT_POS(13, 0);
	if (metersLeft >= 100) Printf("%um", (UINT16)metersLeft);
	else if (metersLeft >= 10) Printf(" %um", (UINT16)metersLeft);
	else Printf("  %um", (UINT16)metersLeft);
	// no space left in bank 0, therefore we can't add more signs to printf
	// as a result we just set the correct icons directly via set_win_tiles
	set_win_tiles(print_x, print_y, 2, 1, icons);

	// print health
	PRINT_POS(1, 0);
	for (i = 0; i < data->Health; i++) Printf("*");
	for (i = data->Health; i < MAX_HEALTH; i++) Printf("°");
}

void KillPlayer(void) {
	data->Health = 0;
	gameoverTimer = GAMEOVER_ANIM_TIME;
	PlayMusic(death_mod_Data, BANK_AUDIO, 1);
}

// If player is not at full health, this function will increment the player's health and updates the frame cache.
// Caution: THIS is not necessarily the player!
void HealPlayer(void) {
	if (data->Health < MAX_HEALTH) {
		data->Health++;
		UPDATE_FRAME_CACHE;
	}
	PLAYFX(player_heal);
}

// If player is not invincible, this function will decrement the player's health and updates the frame cache.
// Caution: THIS is not necessarily the player!
void DamagePlayer(void) {
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
static UINT8 UpdateVelcro(void) {
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
static void UpdateTriggers(void) {
	UINT16 tx, ty;
	UINT8 trigger = FIND_TRIGGER(THIS, TILE_TRIGGERS, TILE_TRIGGERS_MASK, &tx, &ty);
	switch (trigger) {
	case TILE_TRIGGERS:
	case TILE_TRIGGERS + 1:
	case TILE_TRIGGERS + 2:
	case TILE_TRIGGERS + 3:
	case TILE_TRIGGERS + 4:
		DamagePlayer();
		break;
	case TILE_INSTANT_DEATH:
		KillPlayer();
		break;
	case TILE_SLOPE_UP:
		THIS->y -= 16;
		break;
	case TILE_SLOPE_DOWN:
		THIS->y += 12;
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

static UINT8 StartupInProgress(void) {
	if (countdownTimer != 0) {
		countdownTimer--;
		return TRUE;
	}
	return FALSE;
}

static UINT8 HandleDying(void) {
	if (data->Health == 0) {
		SetAnimationState(DEAD);
		if (gameoverTimer-- == 0) {
			SetState(STATE_GAMEOVER, 1);
		} else if (gameoverTimer > (GAMEOVER_ANIM_TIME >> 1) + 5) {
			THIS->y--;
		} else {
			THIS->y += 2;
		}
		return TRUE;
	} else {
		return FALSE;
	}
}

static void HandleInput(PlayerData* data, UINT8 velcro) {
	// move
	if (GetRealAutorun()) {
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
			// during victory, we dont need the idle animation
			if (!IsAutorunOverwritten()) {
				SetAnimationState(IDLE);
			}
		}
	}

	// jump
	if (KEY_TICKED(J_A)) {
		if (GET_BIT(data->Flags, GROUNDED_BIT)) {
			PlayJumpSound(velcro);
			SetAnimationState(JUMP);
			if (velcro)
				data->Jump = VELCRO_JUMP_STRENGTH;
			else
				data->Jump = JUMP_STRENGTH;
		} else if (!GET_BIT(data->Flags, DOUBLE_JUMP_BIT)) {
			PlayJumpSound(FALSE);
			SetAnimationState(JUMP);
			SET_BIT(data->Flags, DOUBLE_JUMP_BIT);
			data->Jump = JUMP_STRENGTH;
		}
	}
}

static void ApplyGravity(PlayerData* data, UINT8 velcro) {
	UINT8 grounded = 0;
	INT8 gravity = velcro ? VELCRO_GRAVITY : (gravitySetting + delta_time + (extraGravity >> 1));
	while (gravity > 8) {
		grounded |= TranslateSprite(THIS, 0, 8);
		gravity -= 8;
	}
	grounded |= TranslateSprite(THIS, 0, gravity);
	// apply gravity and check if sprite is grounded
	if (grounded) {
		if (!velcro && !GET_BIT(data->Flags, GROUNDED_BIT)) {
			PLAYFX(player_grounded);
		}
		SET_BIT(data->Flags, GROUNDED_BIT);
		UNSET_BIT(data->Flags, DOUBLE_JUMP_BIT);
		SetAnimationState(WALK);
		extraGravity = 0;
	} else {
		UNSET_BIT(data->Flags, GROUNDED_BIT);
		if (data->Jump >= -gravitySetting) {
			SetAnimationState(FALL);
			extraGravity++; // as long as we are falling, we increase the gravity
		}
	}

	// if we don't check for upper and lower bound, the player will also die if he leaves the top of the screen
	if (THIS->y > 120 && THIS->y < 160) {
		KillPlayer();
	}
}

static void ApplyJump(PlayerData* data) {
	if (data->Jump != 0) {
		if (data->Jump > 0) {
			data->Jump--;
		} else {
			data->Jump++;
		}

		if (data->Jump <= -8) {
			TranslateSprite(THIS, 0, -8);
			if (TranslateSprite(THIS, 0, data->Jump + 8)) {
				// we hit a collider -> stop jump
				data->Jump = 0;
			}
		} else {
			if (TranslateSprite(THIS, 0, data->Jump)) {
				// we hit a collider -> stop jump
				data->Jump = 0;
			}
		}
	}
}

static UINT8 HandleVictory(PlayerData* data, INT16 metersLeft) {
	if (0 == metersLeft) {
		if (0 == gameoverTimer) {
			OverwriteAutorun(TRUE, FALSE);
			OBP1_REG = normalPalette;
			gameoverTimer = VICTORY_ANIM_TIME;
			TranslateSprite(THIS, 0, GRAVITY); // we don't want to stop mid air
			data->Jump = 0;
		}
		// jump for 3 times (why is it 3? timer starts with 103...)
		if (gameoverTimer > 99) {
			// initiate a jump by faking key input :)
			if (!data->Jump && GET_BIT(data->Flags, GROUNDED_BIT)) {
				UNSET_BIT(data->Flags, GROUNDED_BIT);
				keys = J_A;
				gameoverTimer--;
			} else {
				// prevent any key input handled below
				keys = 0;
			}
		} else {
			// play victory animation
			SetAnimationState(VICTORY);
			gameoverTimer--;
			if (gameoverTimer == 0) {
				OverwriteAutorun(FALSE, FALSE);
				if (++currentLevel == LEVEL_COUNT)
					SetState(STATE_OUTRO, 1);
				else
					SetState(STATE_GAME, 0);
			}
			return TRUE;
		}
	}

	return FALSE;
}

static UINT8 HandleInvincible(PlayerData* data)
{
	// handle invincible animation
	if (data->Invincible > 0) {
		data->Invincible--;
		// blink effect
		OBP1_REG = (data->Invincible & 4) ? invertPalette : normalPalette;

		if (data->Invincible > INVINCIBLE_TIME) {
			// freeze and animate
			return TRUE;
		}

		if (data->Health == 0) {
			SetState(STATE_GAMEOVER, 1);
			HIDE_WIN;
			return TRUE;
		}

		if ((UINT16)data->Invincible == INVINCIBLE_TIME) { // set new frames
			UPDATE_FRAME_CACHE;
			RevertAnimationState();
		}
	}

	return FALSE;
}

static void HandleCheats() {
	UINT8 cheat;
	if (KEY_TICKED(J_UP)) cheatCounter++;
	else if (KEY_TICKED(J_DOWN)) cheatCounter--;
	else if (KEY_TICKED(J_B)) {
		switch (cheatCounter) {
			case 7: HealPlayer(); break;
			case 8: DamagePlayer(); break;
			case 9: data->Invincible = INVINCIBLE_TIME; break;
			case 10: SetAutorun(!GetAutorun());
			default:
				cheat = cheatCounter - 11;
				if (cheat < LEVEL_COUNT) {
					currentLevel = cheat;
					SetState(STATE_GAME, 0);
					break;
				}
		}
	}
}

void Start_SPRITE_PLAYER() {
	player = THIS;
	data = (PlayerData*)THIS->custom_data;
	data->Health = MAX_HEALTH;
	data->Flags = (1 << GROUNDED_BIT);
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

	if(GetRealAutorun()) countdownTimer = START_DELAY;
	else countdownTimer = 0;

	gameoverTimer = 0;
	cheatCounter = 0;
	extraGravity = 0;
}

void Update_SPRITE_PLAYER() {
	UINT8 velcro;
	INT16 metersLeft;
	PlayerData* data = (PlayerData*)THIS->custom_data;

	// calculate meters left
	metersLeft = LEVEL_WIDTH - (THIS->x >> 3) - 10;
	if (metersLeft < 0) metersLeft = 0;

	// draw information on window
	DrawGui(metersLeft);

	if (paused) return; // do nothing if game is paused

	// if startup sequence is not yet done => return
	if (StartupInProgress()) return;

	// handle dying animation
	if (HandleDying()) return;

	if (HandleVictory(data, metersLeft)) return;

	velcro = UpdateVelcro();
	UpdateTriggers();
	if (data->Health == 0) return; // TODO: why is this necessary?

	if (HandleInvincible(data)) return;

	ApplyJump(data);

	ApplyGravity(data, velcro, gravitySetting);

	HandleInput(data, velcro);
	HandleCheats();
}

void Destroy_SPRITE_PLAYER(void) {
}
