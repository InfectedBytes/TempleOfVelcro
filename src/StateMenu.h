#ifndef STATE_MENU_H
#define STATE_MENU_H

#include "main.h"

DECLARE_STATE(STATE_MENU);

/* Callback function for menu selection */
typedef void(*CallbackFunction)(UINT8 selection);

/* Describes a single menu entry*/
typedef struct {
	UINT8 Line;
	UINT8* Text;
} MenuEntry;

/* Describes a complete menu */
typedef struct {
	UINT8 Count; // Number of entries in this menu
	MenuEntry* Entries;
	CallbackFunction Select;
} Menu;
#endif
