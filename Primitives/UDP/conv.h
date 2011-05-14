#ifndef CONV_H_
#define CONV_H_





//------------------------------ include ELEJE ------------------------------
#include "../define/type.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ fuggveny ELEJE ------------------------------
float GET_FLOAT(u8* data);
void SET_FLOAT(u8* data, float num);

u32 GET_U32(u8* data);
void SET_U32(u8* data, u32 num);

u16 GET_U16(u8* data);
void SET_U16(u8* data, u16 num);

u8 GET_U8(u8* data);
void SET_U8(u8* data, u8 num);

bool GET_BOOL(u8* data, u16 bitPos);
void SET_BOOL(u8* data, u16 bitPos, bool bit);
//------------------------------ fuggveny VEGE ------------------------------





#endif	//CONV_H_
