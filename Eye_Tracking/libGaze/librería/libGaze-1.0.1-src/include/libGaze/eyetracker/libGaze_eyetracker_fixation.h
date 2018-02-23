/*
 * libGaze_eyetracker_fixation.h
 *
 *  Created on: Nov 6, 2008
 *      Author: racoon
 */

#ifndef LIBGAZE_EYETRACKER_FIXATION_H_
#define LIBGAZE_EYETRACKER_FIXATION_H_

#include "../libGaze_types.h"

typedef struct{
	int time[2];
	int type;
	double x[2];
	double y[2];
}t_eye_fix_data;

enum eyetracker_return et_get_fixation_thresholds(EYE_TRACKER* eyetracker, float* threshold, float* deviation);

//int et_wait_for_fixation(EYE_TRACKER* eyetracker, t_visualisation_hooks* vhooks, enum lg_eyes eye,t_eye_fix_data *fix_data,int fixdur,int timeout);
int et_wait_for_fixation(EYE_TRACKER* eyetracker, enum lg_eyes eye,t_eye_fix_data *fix_data,int fixdur,int timeout);

#endif /* LIBGAZE_EYETRACKER_FIXATION_H_ */
