#ifndef LIBGAZE_H_
#define LIBGAZE_H_

#include "libGaze_api.h"
#include "libGaze_time.h"
#include "libGaze_types.h"
#include "libGaze_error.h"
#include "eyetracker/libGaze_eyetracker.h"
#include "headtracker/libGaze_headtracker.h"
#include "gazetracker/libGaze_gazetracker.h"
#include "gazetracker/libGaze_gazetracker_types.h"
#include "display/libGaze_display.h"
#include "calibration/libGaze_calibration.h"
#include "visualisation/libGaze_visualisation.h"
#include "libGaze_defines.h"


extern struct timeval *log_base_time;

extern short int __is_init;	/*if libGaze is initialized (1) or not (0)*/

/**
 * @fn int init_LibGaze(void)
 * @brief brief description
 * @return returns what ever
 * Used to initialize the libGaze, so that it is ready to be used.
 */
LIBGAZE_API int init_LibGaze(int, int);

/**
 * This function is used to unload libGaze.
 */
LIBGAZE_API int fin_LibGaze(GAZE_TRACKER* gazetracker);

LIBGAZE_API unsigned long long lg_get_time();

#endif /*LIBGAZE_H_*/
