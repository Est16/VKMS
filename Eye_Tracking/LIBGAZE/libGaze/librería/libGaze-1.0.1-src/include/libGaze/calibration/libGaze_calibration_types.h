#ifndef LIBGAZE_CALIBRATION_TYPES_H_
#define LIBGAZE_CALIBRATION_TYPES_H_

#include <gmodule.h>

#include "../libGaze_types.h"
#include "../eyetracker/libGaze_eyetracker_types.h"
#include "../headtracker/libGaze_headtracker_types.h"
#include "../display/libGaze_display_types.h"

enum lg_cdata_flags{
	LG_CDATA_GOOD,
	LG_CDATA_BAD,
	LG_CDATA_OUT_OF_DISPLAY
};


typedef struct{
	enum lg_cdata_flags flag;		//flag which describes the quality of the data
	//int dis_x;
	//int dis_y;
	double ctarget_pos[3];
	double fov_alpha;
	double fov_beta;
	//double *dis_vec;
	t_raw_eye_data eye;
	t_raw_head_data head;
}t_calibration_data;

typedef struct{
	//unsigned long long time;
	int num;		//number of calibration points
	int outside;	//numner of calibration points which were outside the display plane
	int bad;		//number of calibration points were no data could be collected
	t_calibration_data *cdata;
}t_calibration_data_set;

/* not in use at the moment but will come in future
typedef struct{
	//vectors wich points from the head position to the eyes
	double v_left_eye[3];
	double v_right_eye[3];
	//the position of the head when the calibration was done
	double calib_pos[3];
	//rotation matrix of the head when the calibration was done
	double calib_matrtix[3][3];

	//the angles for the left and right eye, which represent the local center
	double calib_angle_l[2];
	double calib_angle_r[2];


	//camera drift-correction information for the left and right eye
	int cam_drift_enable;
	double cam_drift_left[2];
	double cam_drift_right[2];
}t_calibration_object;
*/

#define CALIBRATION_INIT_MODULE "calibration_init_module"
#define CALIBRATION_FINI_MODULE "calibration_fini_module"
#define CALIBRATION_PROCESS_CALIBRATION_DATA "calibration_processCalibrationData"
#define CALIBRATION_PROCESS_EYE_DATA "calibration_processEyeData"

typedef struct{
	GModule* module;
	int (*processCalibrationData)(int,t_calibration_data_set*);
	//int (*procesEyeAndHeadData)(t_raw_eye_data*,t_raw_head_data*,t_data*);
	int (*procesEyeData)(t_raw_eye_data*,t_eye_data*);
}t_calibration;

typedef t_calibration CALIBRATION;

typedef struct{
	enum lg_cdata_flags flag;
	//int x;
	//int y;
	//int cx[2];
	//int cy[2];
	double vtarget_pos[3];
	double fov_alpha;
	double fov_beta;
	double alpha[2];
	double beta[2];
	double calpha[2];
	double cbeta[2];
	double cdrift[2];
	t_gaze_data_set gaze;
}t_validation_data;

typedef struct{
	int num;
	int bad;
	int good;
	double avg_drift[2];
	double max_drift[2];
	double avg_drift_alpha[2];
	double avg_drift_beta[2];
	t_validation_data *vdata;

}t_validation_data_set;

#endif /*LIBGAZE_CALIBRATION_TYPES_H_*/
