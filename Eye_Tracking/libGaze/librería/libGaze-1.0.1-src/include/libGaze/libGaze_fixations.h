#ifndef LIBGAZE_FIXATIONS_H_
#define LIBGAZE_FIXATIONS_H_


#include "libGaze_types.h"
#include "eyetracker/libGaze_eyetracker_fixation.h"
#include "gazetracker/libGaze_gazetracker_types.h"
/*
typedef struct{
	int time[2];
	int type;
	double x[2];
	double y[2];
}t_eye_fix_data;
*/
typedef struct{
	unsigned long long time;
	//int x;
	//int y;
	double angle;
	t_ray ray;
}t_head_fix_data;

//eye, fixdur,deviation, threashold,timeout
//int wait_for_eye_fixation(EYE_TRACKER*, t_visualisation_hooks*,enum lg_eyes,t_eye_fix_data*,int,float,float,int);
//target threshold timeout
int wait_for_anti_eye_fixation(EYE_TRACKER*, t_visualisation_hooks*,enum lg_eyes,t_eye_fix_data*,float ,int );

int wait_for_head_fixation(GAZE_TRACKER* headtracker,t_visualisation_hooks* vhooks, double* target,double* position_correction, double* orientation_correction, int fixdur,float deviation,float threshold,int timeout);


#endif /*LIBGAZE_FIXATIONS_H_*/
