#ifndef LIBGAZE_HEADTRACKER_LOGGING_H_
#define LIBGAZE_HEADTRACKER_LOGGING_H_


#include "libGaze_headtracker_types.h"


typedef struct{
	char *filename;
}log_t;


#define LOG_ENABLED 1
#define LOG_DISABLED 2
#define TRACKING_ENABLED 3
#define TRACKING_DISABLED 4
#define TRACKING_GET_DATA 5 


void headtracker_logging_main(HEAD_TRACKER*);



#endif /*LIBGAZE_HEADTRACKER_LOGGING_H_*/
