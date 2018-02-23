#ifndef LIBGAZE_EYETRACKER_TYPES_H_
#define LIBGAZE_EYETRACKER_TYPES_H_

#include <stdio.h>
#include <gmodule.h>
#include <glib/gthread.h>

#include "../libGaze_defines.h"
#include "../libGaze_input_types.h"
#include "../display/libGaze_display_types.h"
#include "libGaze_eyetracker_defines.h"


#define EYETRACKER_INIT_MODULE "init_eye_tracker_module"
#define EYETRACKER_FINI_MODULE "fini_eye_tracker_module"
#define EYETRACKER_CONNECT "connect_to_eye_tracker"
#define EYETRACKER_GET_TRACKER_FREQUENCY "get_tracker_frequency"
#define EYETRACKER_DISCONNECT "disconnect_eye_tracker"
#define EYETRACKER_IS_CONNECTED "is_eye_tracker_connected"
#define EYETRACKER_CONFIGURE "configure_eye_tracker"
#define EYETRACKER_GET_TRACKING_DATA	"get_eyetracking_data"
#define EYETRACKER_START_TRACKING "start_eye_tracking"
#define EYETRACKER_STOP_TRACKING "stop_eye_tracking"
#define EYETRACKER_EYE_AVAILABLE "eye_available"
#define EYETRACKER_THRESHOLDS "get_fixation_thresholds"


/*
 * type definitions for the eyetracker
 * */

typedef struct{
	unsigned long long time;	/// (if availible) the	time stamp of the eye tracker in millisec
	//int buttons;
	enum lg_eyes eye_available;	/// for which eyes there is data available in this data set
	double pupil_x[2];	/// the x position of the center of the pupil in the camera image of the eye tracker ([0] = left eye, [1] = right eye)
	double pupil_y[2]; /// the y position of the center of the pupil in the camera image of the eye tracker ([0] = left eye, [1] = right eye)
	double pupil_size[2]; /// (if availible) the pupil size ([0] = left eye, [1] = right eye)
}t_raw_eye_data;


typedef struct{
	unsigned long long time;
	int eye_available;
	double alpha[2];
	double beta[2];
	double size[2];
}t_eye_data;


typedef struct{
	int width;
	int height;
	int size;	/*the size of the image in byte*/
	unsigned int *pixels;
}t_image;




typedef struct{
	GModule *module;

	//int frequency;

	enum eyetracker_return (*connect_to_eye_tracker)(void);
	enum eyetracker_return (*disconnect_eye_tracker)(void);
	enum eyetracker_return (*is_eye_tracker_connected)(void);
	enum eyetracker_return (*configure_eye_tracker)(void*);

	//int (*add_draw_cal_target)(void*,void (void*,int, int));
	//int (*add_erase_cal_target)(void*,void (void*));
	//void *context;

	enum eyetracker_return (*get_tracker_frequency)(int*);
	enum eyetracker_return (*get_eyetracking_data)(t_raw_eye_data*);
	enum eyetracker_return (*start_eye_tracking)(void);
	enum eyetracker_return (*stop_eye_tracking)(void);
	enum eyetracker_return (*start_eye_tracker_calibration)(void);
	enum eyetracker_return (*go_to_setup)(void);
	enum eyetracker_return (*eye_available)(enum lg_eyes*);

	enum eyetracker_return (*get_fixation_thresholds)(float*,float*);
	/*
	enum eyetracker_return (*open_log_file)(char*,char*);
	enum eyetracker_return (*close_log_file)(void);
	enum eyetracker_return (*start_logging)(void);
	enum eyetracker_return (*stop_logging)(void);
	enum eyetracker_return (*add_log_msg)(char*);

	int (*send_input)(t_input);

	int (*get_camera_image)(int, t_image*);
	int (*enable_image_streaming_mode)(void);
	int (*disable_image_streaming_mode)(void);
	*/

	GMutex *mutex_log_state;
	GMutex *mutex_log_file;
	GMutex *mutex_tracking_data;
	GMutex *mutex_log_thread;
	GMutex *mutex_track_state;
	GThread *log_thread;
	char *thread_error;

	//struct timeval *log_base_time;

	int log_state;

	int track_state;

	FILE *log_file;

	int tracking_data_state; //gives the state of the data stored under tracking_data
							// NEW OLD or ERROR
	t_raw_eye_data *tracking_data;

	enum eyetracker_modes mode;

}t_eyetracker;

typedef t_eyetracker EYE_TRACKER;


#endif /*LIBGAZE_EYETRACKER_TYPES_H_*/
