#ifndef UTILS_H
#define UTILS_H

#define BIT_SET(target, bitPos) target |= (1u << bitPos)
#define BIT_CLEAR(target, bitPos) target &= ~(1u << bitPos)
#define BIT_FLIP(target, bitPos) target ^= (1u << bitPos)
#define BIT_GET(target, bitPos) (UINT8)((UINT16)(target >> bitPos) & 1u)

#define SET_BIT(target, bitPos, bitValue) if(bitValue) BIT_SET(target, bitPos); else BIT_CLEAR(target, bitPos);

#endif