#pragma bank=2
#include "SpriteCountdown.h"
#include "SpriteManager.h"
#include "SpritePlayer.h"

UINT8 bank_SPRITE_COUNTDOWN = 2;

static INT8 countdownTimer;

void Start_SPRITE_COUNTDOWN() {
	PAL1;
	countdownTimer = 0;
	THIS->current_frame = 0;
}

void Update_SPRITE_COUNTDOWN() {
	if (countdownTimer++ == FRAMES_PER_SECOND && THIS->current_frame < 3) {
		countdownTimer = 0;
		THIS->current_frame++;
	}
}

void Destroy_SPRITE_COUNTDOWN() {
}