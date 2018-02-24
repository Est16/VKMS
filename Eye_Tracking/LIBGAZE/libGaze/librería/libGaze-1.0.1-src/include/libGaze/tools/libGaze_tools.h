#ifndef LIBGAZE_TOOLS_H_
#define LIBGAZE_TOOLS_H_


#include "../headtracker/libGaze_headtracker_types.h"
#include "../gazetracker/libGaze_gazetracker_types.h"
#include "libGaze_vector_and_angles.h"
#include "libGaze_matrix.h"



int lg_tools_correct_head_data(t_raw_head_data*,double* position_correction, double* orientation_correction);
void lg_get_fov_ray_from_angles(GAZE_TRACKER *gazetracker,double alpha, double beta, double *origin, double *direction);

#endif /*LIBGAZE_TOOLS_H_*/
