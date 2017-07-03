#ifndef ZGB_CONFIG_H
#define ZGB_CONFIG_H

/*********************************************\
|           SPRITE / SPRITE MANAGER           |
\*********************************************/
// N_SPRITE_MANAGER_SPRITES (default: 20), defines how many sprites ZGB will manage
// The gameboy supports 40 hardware sprites of the size 8x8 or 8x16 so if you only plan 
// to use those small sprites, you can increase this value to 40.
// Larger sprites are composed of several smaller ones, therefore for those cases 
// you should decrease this constant
#define N_SPRITE_MANAGER_SPRITES 20

// CUSTOM_DATA_SIZE (default: 8), each sprite will have this many bytes for custom user data
#define CUSTOM_DATA_SIZE 8



// In order to decrease the size of the generated assembly, you can disable certain parts of the engine.
// To disable a certain part, just comment the corresponding define.

// This define is used to enable the drawing of 8x8 sprites
//#define SPRITES_8X8_ENABLED

// This define is used to enable the drawing of 8x16 sprites
#define SPRITES_8X16_ENABLED

// This define is used to enable the drawing of 16x16 sprites
//#define SPRITES_16X16_ENABLED

// This define is used to enable the drawing of 24x32 sprites
#define SPRITES_24X32_ENABLED

// This define is used to enable the drawing of 32x32 sprites
//#define SPRITES_32X32_ENABLED



/*********************************************\
|                    PRINT                    |
\*********************************************/
// This define is used to enable the hexadecimal print, for e.g.: Printf("%x", value)
//#define PRINT_HEX_ENABLED

// This define is used to enable the signed print, for e.g.: Printf("%d %i", value, value)
//#define PRINT_SIGNED_ENABLED
#endif