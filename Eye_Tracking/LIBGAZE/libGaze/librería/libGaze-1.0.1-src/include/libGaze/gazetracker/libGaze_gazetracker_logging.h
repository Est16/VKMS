/*
 * libGaze_eyeheadtracker_logging.h
 *
 *  Created on: Oct 23, 2008
 *      Author: racoon
 */

#ifndef LIBGAZE_GAZETRACKER_LOGGING_H_
#define LIBGAZE_GAZETRACKER_LOGGING_H_

#include "libGaze_gazetracker_types.h"
/*
typedef struct{
	char *filename;
}log_t;
*/
#define LOG_ENABLED 1
#define LOG_DISABLED 2
#define TRACKING_ENABLED 3
#define TRACKING_DISABLED 4
#define TRACKING_GET_DATA 5

#define NO_LOG_FILE	0
#define GAZE_LOG_FILE	1
#define HEAD_LOG_FILE	2
#define EYE_LOG_FILE		4
#define EVENT_LOG_FILE	8
#define CALIB_LOG_FILE 16
#define ALL_LOG_FILE	31

//non-sense


void gazetracker_logging_main(GAZE_TRACKER*);

#endif /* LIBGAZE_GAZETRACKER_LOGGING_H_ */
