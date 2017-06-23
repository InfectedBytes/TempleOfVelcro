# ZGB
This directory contains the ZGB engine, which is a little engine for the original *Game Boy* and *Game Boy Color*.
We have modified a few small parts of it. The following changes were made:
 - Added hexadecimal output to the Printf function (Print.h/.c)
 - Added support for 24x32 and 32x32 sprites
  - affected files: SpriteManager.h/.c, Frame.h/.c, OAMManager.c, Sprite.c, main.c
  - added oam index management => each sprite keeps track of it's oam indices
  - added oam index defragmentation
  - now it is possible to use 40 small sprites (8x8, 8x16), you only have to change the `N_SPRITE_MANAGER_SPRITES` define in SpriteManager.h
    - The original engine only supported 20 sprites, because each sprite always uses two oam indices.
    - Now each sprite uses 1 to 8 oam indices, depending on the size (8x8 to 32x32)
  - If needed, larger frame sizes can be added easily (but I guess 32x32 is already pretty large)
 - Modified ScrollFindTile function (Scroll.c)
  - Removed the multiplication in each iteration
  - It can now search for several tiles at once. This is done by using an additional masking parameter
  - Each bit of the mask that is set to one it used as a wildcard bit.
   - For example if the mask has a binary value of `0b00000011` it would ignore the two lower most bits of any tile
   - `ScrollFindTile(map_w, map, bank, 0b00001100, 0b00000011, ...)` would match all tiles from `0b00001100` to `0b00001111`
   - The return value is the found tile id or 0 if no tile were found.
    - **Limitation**: Because of this you can't search for the tile id 0

## Features
 - Coding in C using [gbdk](http://gbdk.sourceforge.net/)
 - Based on States and Sprites
 - 4-directional scroll
 - Support maps bigger than 32x32
 - Transparent sprites pool allowing 20 sprites 8x16 or 16x16
 - Sprite animations
 - Collisions sprite vs sprite and sprite vs background and configurable rectangle collider
 - Easy makefile support (just 3 lines for most projects)
 - Bank management
 - Fonts
 - Music support using [gbt-player](https://github.com/AntonioND/gbt-player)
 - Sound Effects

## Official Repository
For more information have a look at the official GitHub repository: [ZGB](https://github.com/Zal0/ZGB)
