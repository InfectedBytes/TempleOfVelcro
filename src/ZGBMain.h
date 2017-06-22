#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#include "main.h"
#include "gb/hardware.h"
#include "palette.h"
#include "Print.h"
#include "Utils.h"
#include "Math.h"

typedef enum {
	STATE_GAME,

	N_STATES
} STATE;

typedef enum {
	SPRITE_PLAYER,

	N_SPRITE_TYPES
} SPRITE_TYPE;

#endif