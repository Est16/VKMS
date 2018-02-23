#ifndef LIBGAZE_EYETRACKER_LOGGING_H_
#define LIBGAZE_EYETRACKER_LOGGING_H_


#include "../libGaze_types.h"

typedef struct{
	char *filename;
}log_t;


#define LOG_ENABLED 1
#define LOG_DISABLED 2
#define TRACKING_ENABLED 3
#define TRACKING_DISABLED 4
#define TRACKING_GET_DATA 5


void eyetracker_logging_main(EYE_TRACKER*);


#endif /*LIBGAZE_EYETRACKER_LOGGING_H_*/
