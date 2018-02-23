/****************************************************************************
* 	libGaze
*  A framework to combine off-the-shelf video-based eye tracking systems
*	with motion caputing systems
*
*	Copyright(C) 2008
*	Max Planck Institute for Biological Cybernetics Tuebingen
*	BWFIT
*
*	All rights reserved.
*
*	This file is part of libGaze.
*
*	libGaze is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  libGaze is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with libGaze.  If not, see <http://www.gnu.org/licenses/>.
*
****************************************************************************/

#include <stdlib.h>
#include <glib.h>
#include <gmodule.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>

#include <libGaze.h>
#include <libGaze_types.h>
#include <libGaze_error.h>
#include <display/libGaze_display.h>

DISPLAY* d_load_module(char* module_path,int debug, int error){
	_DEBUG_("d_load_module: %s\n",module_path);
	//void *mod;
	GModule *mod;
	mod = g_module_open(module_path,G_MODULE_BIND_LOCAL);
	if(mod==NULL){
		_ERROR_("%s\n",g_module_error());
		return NULL;
	}


	DISPLAY *display;
	display = (DISPLAY*)calloc(1,sizeof(DISPLAY));
	if(display == NULL){
		_ERROR_("could allocate memory for DISPLAY\n");
		return NULL;
	}
	display->module = mod;

	int merror =0;



	if(!g_module_symbol(mod,DISPLAY_GET_3D_POSITION_FROM_XY,(gpointer*)&(display->get3DPositionFromXY))){

		_ERROR_("%s\n",g_module_error());
		merror ++;
	}


	if(!g_module_symbol(mod,DISPLAY_GET_XY_POSITION_FROM_3D,(gpointer*)&(display->getXYPositionFrom3D))){

		_ERROR_("%s\n",g_module_error());
		merror ++;
	}
/*
	if(!g_module_symbol(mod,DISPLAY_GET_XY_FROM_POSITION_DIRECTION,(gpointer*)&(display->getXYFromPrositionAndDirection))){

		_ERROR_("%s\n",g_module_error());
		merror ++;
	}
*/
	if(!g_module_symbol(mod,DISPLAY_GET_INTERSECTION,(gpointer*)&(display->getIntersectionWithScreen))){

		_ERROR_("%s\n",g_module_error());
		merror ++;
	}
	if(!g_module_symbol(mod,DISPLAY_CONFIGURE,(gpointer*)&(display->configure_display))){

		_ERROR_("%s\n",g_module_error());
		merror ++;
	}

	if(merror>0){
		_ERROR_("display module does not implement all needed functions\n");
		return NULL;
	}

	int (*d_init)(int, int ,FILE*,FILE*,struct timeval*);
	_DEBUG_("init\n");
	if(g_module_symbol(mod,DISPLAY_INIT_MODULE,(gpointer*)&(d_init))){

		 _DEBUG_("init: %p\tdebug: %d\terror:%d\n",d_init,debug,error);
		(*d_init)(debug,error,lg_debug,lg_error, log_base_time);
	}else{
		_ERROR_("%s\n",g_module_error());
		_ERROR_("d_init = NULL\n");
		return NULL;
	}
	_DEBUG_("DISPLAY: end_load_module\n");
	return display;
}

int d_configure(DISPLAY* display, char* filename){
	return (*(display->configure_display))(filename,&(display->width),&(display->height));
}

int d_unload_module(DISPLAY* display){
	if(display != NULL){
		if (display->module != NULL){
			//g_module_close(head->module);
			free(display);
		return 0;
		}else{
			_ERROR_("unload_module: module = NULL\n");
			free(display);
		}

	}
	_ERROR_("unload_module: display = NULL\n");
	return -1;
}
double* d_get3DPositionFromXY(DISPLAY* display, int* xy){
	return (*(display->get3DPositionFromXY))(xy);
}

int* d_getXYPositionFrom3D(DISPLAY* display, double* pos){
	return (*(display->getXYPositionFrom3D))(pos);
}

//int *d_getXYFromPrositionAndDirection(DISPLAY*display ,double* pos ,double* direction){
//	return (*(display->getXYFromPrositionAndDirection))(pos,direction);
//}

double* d_getIntersectionWithScreen(DISPLAY*display ,double* pos ,double* direction){
	return (*(display->getIntersectionWithScreen))(pos,direction);
}

int d_getDisplayCoordFromRay(DISPLAY*display, t_ray *ray, t_display_coord* dcoord){
	double *dpos = d_getIntersectionWithScreen(display,ray->origin,ray->direction);
	dcoord->pos3D[0]  = dpos[0];
	dcoord->pos3D[1]  = dpos[1];
	dcoord->pos3D[2]  = dpos[2];
	//printf("left_dpos: %f\t%f\t%f\n",left_dpos[0],left_dpos[1],left_dpos[2]);
	int *xy = d_getXYPositionFrom3D(display, dcoord->pos3D);

	dcoord->x = xy[0];
	dcoord->y = xy[1];
	return LG_OK;
}

int d_getDisplayCoordsFromGazeSet(DISPLAY*display, t_gaze_data_set *gds, t_display_coord_set* dcoords){
	//printf("d_getDisplayCoordsFromGazeSet");
	double *left_dpos = d_getIntersectionWithScreen(display,gds->gaze[0].origin,gds->gaze[0].direction);
	dcoords->displayCoords[0].pos3D[0]  = left_dpos[0];
	dcoords->displayCoords[0].pos3D[1]  = left_dpos[1];
	dcoords->displayCoords[0].pos3D[2]  = left_dpos[2];
	//printf("left_dpos: %f\t%f\t%f\n",left_dpos[0],left_dpos[1],left_dpos[2]);
	int *left_xy = d_getXYPositionFrom3D(display, dcoords->displayCoords[0].pos3D);

	dcoords->displayCoords[0].x = left_xy[0];
	dcoords->displayCoords[0].y = left_xy[1];

	double *right_dpos = d_getIntersectionWithScreen(display,gds->gaze[1].origin,gds->gaze[1].direction);
	dcoords->displayCoords[1].pos3D[0]  = right_dpos[0];
	dcoords->displayCoords[1].pos3D[1]  = right_dpos[1];
	dcoords->displayCoords[1].pos3D[2]  = right_dpos[2];
	int *right_xy = d_getXYPositionFrom3D(display, dcoords->displayCoords[1].pos3D);

	dcoords->displayCoords[1].x = right_xy[0];
	dcoords->displayCoords[1].y = right_xy[1];

	dcoords->avg_x = (int)((double)(dcoords->displayCoords[0].x+dcoords->displayCoords[1].x)/2.0);
	dcoords->avg_y = (int)((double)(dcoords->displayCoords[0].y+dcoords->displayCoords[1].y)/2.0);

	return LG_OK;
}

