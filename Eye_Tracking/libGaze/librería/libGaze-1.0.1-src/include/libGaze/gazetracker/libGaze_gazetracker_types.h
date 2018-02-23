#ifndef LIBGAZE_GAZETRACKER_TYPES_H_
#define LIBGAZE_GAZETRACKER_TYPES_H_

#include "../libGaze_types.h"
#include "../calibration/libGaze_calibration_types.h"
#include "libGaze_gazetracker_defines.h"




typedef struct{
	int debug;
	int error;
	EYE_TRACKER* eyetracker;		//pointer to the eye tracker module
	HEAD_TRACKER* headtracker;	//pointer toe head tracker module
	//DISPLAY* display;						// pointer to the display module

	CALIBRATION* calibration;	// pointer to the calibration module

	t_calibration_data_set* calibdataset;	// the last used calibration data set
	t_calibration_data_set* validdataset; // the last used validation data set

	//t_calibration_object calibobject; //idea was to use this for drift and eye information

	t_head_correction hcorrection;	// data for the head correction //need some changes when we want
														// to use two different eye positions

	t_visualisation_hooks* visualhooks; // a pointer to the visualisation hooks (used to present something on the display)

	FILE *event_log;			// pointer to the event logging file
	FILE *calibration_log;	// pointer to the calibration logging file

	int is_calibrated;

	enum gt_modes mode; // mode of the eyeheadtracker
	enum lg_eyes used_eye;			//which eye is used for tracking

	GMutex *mutex_log_state;
	GMutex *mutex_log_file;
	GMutex *mutex_tracking_data;
	GMutex *mutex_log_thread;
	GMutex *mutex_track_state;
	GThread *log_thread;
	char *thread_error;

	int log_state;

	int track_state;

	FILE *log_file;
	int log_mode;
	int tracking_data_state; //gives the state of the data stored under tracking_data
							// NEW OLD or ERROR
	t_gaze_data_set *tracking_data;
}t_gazetracker;

typedef t_gazetracker GAZE_TRACKER;


typedef struct{
	int n;
	int m;
	int num;
	double range_alpha;
	double range_beta;
	double center_alpha;
	double center_beta;

	double *alphas;
	double *betas;
}t_grid;


#endif /*LIBGAZE_GAZETRACKER_TYPES_H_*/
