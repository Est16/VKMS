#ifndef LIBGAZE_DISPLAY_TYPES_H_
#define LIBGAZE_DISPLAY_TYPES_H_

#include <gmodule.h>


#define DISPLAY_INIT_MODULE "display_init_module"
#define DISPLAY_FINI_MODULE "display_fini_module"
#define DISPLAY_CONFIGURE "display_configure"
#define DISPLAY_GET_3D_POSITION_FROM_XY "display_get3DPositionFromXY"
#define DISPLAY_GET_XY_POSITION_FROM_3D "display_getXYPositionFrom3D"
//#define DISPLAY_GET_XY_FROM_POSITION_DIRECTION "display_getXYFromPrositionAndDirection"
#define DISPLAY_GET_INTERSECTION "display_getIntersectionWithScreen"

/*
 *
 *
 */

typedef struct{
	GModule* module;
	int width;
	int height;
	int (*configure_display)(char*, int*,int*);
	double* (*get3DPositionFromXY)( int*);
	int* (*getXYPositionFrom3D)( double*);
	//int* (*getXYFromPrositionAndDirection)(double*,double*);
	double* (*getIntersectionWithScreen)(double*,double*);
}t_display;

typedef t_display DISPLAY;


typedef struct{
	int x;
	int y;
	double pos3D[3];
}t_display_coord;

typedef struct{
	int display_coords_available;
	int avg_x;
	int avg_y;
	t_display_coord displayCoords[2];
}t_display_coord_set;

#endif /*LIBGAZE_DISPLAY_TYPES_H_*/
