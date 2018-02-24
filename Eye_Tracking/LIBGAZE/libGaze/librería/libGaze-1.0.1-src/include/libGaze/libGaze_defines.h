#ifndef LIBGAZE_DEFINES_H_
#define LIBGAZE_DEFINES_H_

#define	LG_ERROR					5
#define	LG_OK							6
#define	LG_ACCEPT					7
#define	LG_ABORT					8
#define LG_QUIT						9
#define LG_TIMEOUT				10

enum lg_eyes{
	LG_EYE_BOTH 	= 1,
	LG_EYE_LEFT,
	LG_EYE_RIGHT,
	LG_EYE_NONE
};

#include "eyetracker/libGaze_eyetracker_defines.h"
#include "headtracker/libGaze_headtracker_defines.h"
#include "gazetracker/libGaze_gazetracker_defines.h"



#endif /*LIBGAZE_DEFINES_H_*/
