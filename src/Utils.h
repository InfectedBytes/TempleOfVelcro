#ifndef UTILS_H
#define UTILS_H

#include "Scroll.h"
#include "Sprite.h"

// velcros
#define TILE_VELCRO 4
#define TILE_VELCRO_MASK 3

// other trigger
#define TILE_TRIGGERS 120
#define TILE_TRIGGERS_MASK 0xF
#define TILE_INSTANT_DEATH 125
#define TILE_SLOPE_UP 126
#define TILE_SLOPE_DOWN 127
#define TILE_HEART_SPAWN 22

#define SET_PAL0(a, b, c, d) OBP0_REG = PAL_DEF(a, b, c, d)
#define SET_PAL1(a, b, c, d) OBP1_REG = PAL_DEF(a, b, c, d)
#define UsePalette(sprite, pal) (sprite)->flags = ((sprite)->flags & 0xEF) | ((pal & 0x01) << 4)
#define PAL0 UsePalette(THIS, 0)
#define PAL1 UsePalette(THIS, 1)
#define PAL(i) UsePalette(THIS, i)

#define BEHIND_BG SET_BIT(THIS->flags, 7)
#define ABOVE_BG UNSET_BIT(THIS->flags, 7)

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

#define FIND_TRIGGER(sprite, tile, mask, sx, sy) ScrollFindTile(scroll_tiles_w, scroll_map, scroll_bank, tile, mask, \
	(sprite->x + sprite->coll_x) >> 3, (sprite->y + sprite->coll_y) >> 3, \
	(sprite->coll_w >> 3) + 1, (sprite->coll_h >> 3) + 1, sx, sy)

#define FIND_TOP_TRIGGER(sprite, tile, mask, sx, sy) ScrollFindTile(scroll_tiles_w, scroll_map, scroll_bank, tile, mask, \
	(sprite->x + sprite->coll_x) >> 3, (sprite->y + sprite->coll_y) >> 3, \
	(sprite->coll_w >> 3) + 1, 1, sx, sy)

#endif