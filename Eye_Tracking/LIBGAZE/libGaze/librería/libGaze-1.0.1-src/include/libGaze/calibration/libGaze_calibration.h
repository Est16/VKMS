#ifndef LIBGAZE_CALIBRATION_H_
#define LIBGAZE_CALIBRATION_H_

#include "../libGaze_types.h"
#include "libGaze_calibration_types.h"
#include "../gazetracker/libGaze_gazetracker_types.h"

CALIBRATION* c_load_module(char*,int,int);

int c_processCalibrationData(CALIBRATION*,int,t_calibration_data_set*);

//int c_procesEyeAndHeadData(CALIBRATION*,t_raw_eye_data*,t_raw_head_data*, t_data*);

int c_processEyeData(CALIBRATION*,t_raw_eye_data*, t_eye_data*);


int c_validate_calibration(GAZE_TRACKER* , t_calibration_data_set*, t_validation_data_set*);

#endif /*LIBGAZE_CALIBRATION_H_*/
