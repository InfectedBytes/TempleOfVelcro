#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#include "main.h"
#include "gb/hardware.h"
#include "palette.h"
#include "Print.h"

#define SET_PAL0(a, b, c, d) OBP0_REG = PAL_DEF(a, b, c, d)
#define SET_PAL1(a, b, c, d) OBP1_REG = PAL_DEF(a, b, c, d)
#define UsePalette(sprite, pal) (sprite)->flags = ((sprite)->flags & 0xEF) | ((pal & 0x01) << 4)
#define PAL0 UsePalette(THIS, 0)
#define PAL1 UsePalette(THIS, 1)
#define PAL(i) UsePalette(THIS, i)

#define BOTTOM_LINES(lines) WY_REG = (144 - (lines << 3))
#define INIT_WINDOW(font, fontBank, xOffset, lines) INIT_FONT(font, fontBank, PRINT_WIN); \
	PRINT_POS(0, 0); \
	WX_REG = 7 + (xOffset << 3); \
	WY_REG = (144 - (lines << 3)); \
	SHOW_WIN;

#define COLLISION_BORDER(x, y, w, h) THIS->coll_x = x; \
	THIS->coll_y = y; \
	THIS->coll_w = w; \
	THIS->coll_h = h;

typedef enum {
	STATE_GAME,

	N_STATES
} STATE;

typedef enum {
	SPRITE_PLAYER,

	N_SPRITE_TYPES
} SPRITE_TYPE;

#endif