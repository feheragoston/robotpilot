#ifndef TYPE_H_
#define TYPE_H_





//------------------------------ tipus ELEJE ------------------------------
typedef struct progress_t{
	progress_t() : inProgress(false), finished(false) {}
	bool inProgress;
	bool finished;
}progress_t;
//------------------------------ tipus VEGE ------------------------------





//------------------------------ tipus ELEJE ------------------------------
//1	char
//2	short
//4	int
//4	long int
//8	long long int
//8	double
//4	float

typedef unsigned char		u8;
typedef signed char			s8;
typedef unsigned short		u16;
typedef signed short		s16;
typedef unsigned int		u32;
typedef signed int			s32;
//------------------------------ tipus VEGE ------------------------------





#endif /*TYPE_H_*/
