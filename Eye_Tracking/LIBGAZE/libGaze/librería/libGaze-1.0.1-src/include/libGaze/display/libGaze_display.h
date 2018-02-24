#ifndef LIBGAZE_DISPLAY_H_
#define LIBGAZE_DISPLAY_H_

#include "../libGaze_types.h"


DISPLAY* d_load_module(char*,int debug, int error);
int d_configure(DISPLAY*, char*);
int d_unload_module(DISPLAY*);
double* d_get3DPositionFromXY(DISPLAY*, int*);
int* d_getXYPositionFrom3D(DISPLAY*, double*);
//int* d_getXYFromPositionAndDirection(DISPLAY*,double*,double*);
double* d_getIntersectionWithScreen(DISPLAY*,double*,double*);
int d_getDisplayCoordFromRay(DISPLAY*, t_ray *, t_display_coord* );
int d_getDisplayCoordsFromGazeSet(DISPLAY*, t_gaze_data_set *, t_display_coord_set* );

#endif /*LIBGAZE_DISPLAY_H_*/
