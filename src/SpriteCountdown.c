#pragma bank=2
#include "SpriteCountdown.h"
#include "SpriteManager.h"
#include "SpritePlayer.h"
#include "Sound.h"
#include "../res/src/SoundEffects.h"

UINT8 bank_SPRITE_COUNTDOWN = 2;

static INT8 countdownTimer;

void Start_SPRITE_COUNTDOWN() {
	PAL1;
	countdownTimer = 0;
	THIS->current_frame = 0;
}

void Update_SPRITE_COUNTDOWN() {
	if (THIS->current_frame != 3) {
		if (countdownTimer++ == FRAMES_PER_SECOND) {
			countdownTimer = 0;
			THIS->current_frame++;

			if (THIS->current_frame == 3) {
				PLAYFX(countdown_start);
			} else {
				PLAYFX(countdown_tick);
			}
		}
	}
}

void Destroy_SPRITE_COUNTDOWN() {
}
