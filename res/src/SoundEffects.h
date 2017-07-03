#ifndef SOUNDEFFECTS_H_
#define SOUNDEFFECTS_H_

/* the list of all sound effects and their parameters */
#define PLAYFX_player_jump_velcro  PlayFx(CHANNEL_4, 20, 0x06, 0xF2, 0x7D, 0xC0)
#define PLAYFX_player_jump         PlayFx(CHANNEL_1, 20, 0x56, 0x81, 0xF2, 0x92, 0x84)
#define PLAYFX_player_heal         PlayFx(CHANNEL_4, 20, 0x06, 0xF2, 0x7D, 0xC0)
#define PLAYFX_player_damage       PlayFx(CHANNEL_4, 20, 0x06, 0xF6, 0x3F, 0xC0)
#define PLAYFX_player_grounded     PlayFx(CHANNEL_1, 20, 0x19, 0x81, 0xF2, 0x73, 0x86)

/* helper macro to play a sound effect with symbolic name
 * example: PLAYFX(player_jump)
 */
#define PLAYFX(sound) PLAYFX_##sound

#endif /* SOUNDEFFECTS_H_ */
