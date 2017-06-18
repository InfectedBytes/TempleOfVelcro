#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#include "main.h"
#include "gb/hardware.h"
#include "palette.h"

#define SET_PAL0(a, b, c, d) OBP0_REG = PAL_DEF(a, b, c, d)
#define SET_PAL1(a, b, c, d) OBP1_REG = PAL_DEF(a, b, c, d)
#define UsePalette(sprite, pal) (sprite)->flags = ((sprite)->flags & 0xEF) | ((pal & 0x01) << 4)
#define PAL0 UsePalette(THIS, 0)
#define PAL1 UsePalette(THIS, 1)
#define PAL(i) UsePalette(THIS, i)

typedef enum {
	STATE_GAME,

	N_STATES
} STATE;

typedef enum {
	SPRITE_PLAYER,

	N_SPRITE_TYPES
} SPRITE_TYPE;

#endif