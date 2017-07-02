#ifndef SOUNDEFFECTS_H_
#define SOUNDEFFECTS_H_

#define PLAYFX_player_jump_velcro  PlayFx(CHANNEL_4, 20, 0x06, 0xF2, 0x7D, 0xC0)
#define PLAYFX_player_jump         PlayFx(CHANNEL_1, 20, 0x56, 0x81, 0xF2, 0x92, 0x84)

#define PLAYFX(sound) PLAYFX_##sound

#endif /* SOUNDEFFECTS_H_ */
