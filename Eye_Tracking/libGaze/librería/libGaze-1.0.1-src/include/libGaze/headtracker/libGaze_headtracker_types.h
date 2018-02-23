#ifndef LIBGAZE_HEADTRACKER_TYPES_H_
#define LIBGAZE_HEADTRACKER_TYPES_H_

#include <gmodule.h>
#include <glib/gthread.h>
#include <stdio.h>


#define HEADTRACKER_INIT_MODULE "init_head_tracker_module"
#define HEADTRACKER_FINI_MODULE "fini_head_tracker_module"
#define HEADTRACKER_GET_TRACKER_FREQUENCY "get_tracker_frequency"
#define HEADTRACKER_CONNECT "connect_to_head_tracker"
#define HEADTRACKER_DISCONNECT "disconnect_head_tracker"
#define HEADTRACKER_IS_CONNECTED "is_head_tracker_connected"
#define HEADTRACKER_CONFIGURE "configure_head_tracker"
#define HEADTRACKER_GET_TRACKING_DATA	"get_head_tracking_data"
#define HEADTRACKER_START_TRACKING "start_head_tracking"
#define HEADTRACKER_STOP_TRACKING "stop_head_tracking"


enum headtracker_modes{
	HEADTRACKER_MODE_NORMAL = 0,
	HEADTRACKER_MODE_DUMMY
};

/*
 * type definitions for the headtracker
 * */

typedef struct{
	unsigned long long time;
	double position[3];
	//double direction[3];
	double orientation[9];
}t_head_data;

typedef struct{
	//local timestamp in milliseconds. the timer starts counting at 0 when the library is initialized
	unsigned long long time;
	//position of the head in the 3D coordinatesystem of the headtracker
	double position[3]; //x= pos[0] y=pos[1] z=pos[2]
	//the rotation-matrix of the head
	double matrix[9];
	//The state of a sensor button 0=released, 1=pressed
	int button;
}t_raw_head_data;


typedef struct{
	double correct_x;
	double correct_y;
	double correct_z;

	double correct_matrix[9];

	double headtonasalbone[3];
	// relative vector from the nasalbone to the left and right eye (head related coordinate system)
	double headtoeye[2][3];
}t_head_correction;


typedef struct{
	GModule *module;
	/*
	 * indices which identifie the head tracking and
	 * eye pointer objects in the list of all tracked objects
	 */
	int ihead;
	int ieye;
	int num; // number of trackerd objects

	int (*get_tracker_frequency)(int*);
	int (*connect_to_head_tracker)(void);
	int (*disconnect_head_tracker)(void);
	int (*configure_head_tracker)(void*,int*,int*,int*);
	int (*start_head_tracking)(void);
	int (*stop_head_tracking)(void);
	int (*get_tracking_data)( t_raw_head_data*);

	int (*is_head_tracker_connected)(void);

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

	t_raw_head_data *tracking_data;

	enum headtracker_modes mode;

}t_headtracker;

typedef t_headtracker HEAD_TRACKER;


#endif /*LIBGAZE_HEADTRACKER_TYPES_H_*/
