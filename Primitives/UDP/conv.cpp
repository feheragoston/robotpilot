#include "conv.h"





float GET_FLOAT(u8* data){

	return (*(float*)data);

}


void SET_FLOAT(u8* data, float num){

	(*(float*)data) = num;

}


u32 GET_U32(u8* data){

	return (*(u32*)data);

}


void SET_U32(u8* data, u32 num){

	(*(u32*)data) = num;

}


s32 GET_S32(u8* data){

	return (*(s32*)data);

}


void SET_S32(u8* data, s32 num){

	(*(s32*)data) = num;

}


u16 GET_U16(u8* data){

	return (*(u16*)data);

}


void SET_U16(u8* data, u16 num){

	(*(u16*)data) = num;

}


s16 GET_S16(u8* data){

	return (*(s16*)data);

}


void SET_S16(u8* data, s16 num){

	(*(s16*)data) = num;

}


u8 GET_U8(u8* data){

	return (*(u8*)data);

}


void SET_U8(u8* data, u8 num){

	(*(u8*)data) = num;

}


s8 GET_S8(u8* data){

	return (*(s8*)data);

}


void SET_S8(u8* data, s8 num){

	(*(s8*)data) = num;

}


bool GET_BOOL(u8* data, u16 bitPos){

	while(bitPos >= 8){
		data++;
		bitPos -= 8;
	}

	return ((((*data) & (0x01 << bitPos)) != 0) ? true : false);

}


void SET_BOOL(u8* data, u16 bitPos, bool bit){

	while(bitPos >= 8){
		data++;
		bitPos -= 8;
	}

	//ha 1-es bit
	if(bit)
		*data |= (0x01 << bitPos);

	//ha 0-s bit
	else
		*data &= ~(0x01 << bitPos);

}
