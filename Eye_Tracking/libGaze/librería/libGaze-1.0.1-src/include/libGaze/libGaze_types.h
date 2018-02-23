#ifndef LIBGAZE_TYPES_H_
#define LIBGAZE_TYPES_H_


typedef struct{
	unsigned long time;	// (if available) the	time stamp of the eye tracker in millisec
	double origin[3];	// Position vector of the eye, x= position[0] y=position[1] z=position[2]
	double direction[3];// Direction vector of the eyes gaze, x= direction[0] y=direction[1] z=direction[2]
	double dangles[2]; // The direction vector of the eyes gaze in horizontal and vertical angle
	double matrix[9];		// rotation matrix of the eye
}t_gaze_data;




typedef struct{
	double origin[3];
	double direction[3];
}t_ray;

#include "libGaze_input_types.h"
#include "visualisation/libGaze_visualisation_types.h"


#include "eyetracker/libGaze_eyetracker_types.h"
#include "headtracker/libGaze_headtracker_types.h"

typedef struct{
	unsigned long long time;
	int gaze_available;
	t_head_data head;
	t_eye_data eye;
	t_gaze_data gaze[2];
}t_gaze_data_set;

//old data type only used for validation
/*
typedef struct{
	int eye_available;	// for which eyes there is data available in this data set
	int head_available;	// number of head sensors for which there is data available in this data set
	int dis_x[2];				// x-Position of intersection of gaze vector with display plane. dis_x[0]: Left Eye,dis_x[1]: Rigth Eye if available
	int dis_y[2];				// y-Position of intersection of gaze vector with display plane. dis_y[0]: Left Eye,dis_y[1]: Rigth Eye if available
	double alpha[2];
	double beta[2];
	double gpos[2][3];
	t_raw_eye_data eye;				// One t_eye_data structure filled with the data from the eyetracker
	t_gaze_data gaze[2];		// Array of t_gaze_data structures of length eye_available with eye[0] filled with data for left eye and eye[1] filled with data for right eye
	t_raw_head_data head;			// Array of t_head_data structures of length head_available with head[0] filled with data for head sensor with number 0 and so in
}t_data;
*/
#endif /*LIBGAZE_TYPES_H_*/
