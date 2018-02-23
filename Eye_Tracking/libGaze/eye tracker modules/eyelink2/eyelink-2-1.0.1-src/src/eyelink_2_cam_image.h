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
#ifndef EYELINK_2_CAM_IMAGE_H_
#define EYELINK_2_CAM_IMAGE_H_

#include <glib/gthread.h>

extern int img_ncolours;

extern int img_width;
extern int img_height;

extern int img_size;	/*image size in byte*/

extern unsigned int *palette_rgba;

extern int get_image;

extern unsigned int *img_current;
extern unsigned int *img_tmp;

extern int current_eye;
extern int selected_eye;
extern int img_next;
extern int key_iterator;

extern unsigned int is_in_streaming_mode;

extern GThread *eyelink_2_streaming_thread;
extern char *thread_error;

extern GMutex *mutex_lock_image;
extern int img_number;

//int get_camera_image(int,t_image*);

//int enable_image_streaming_mode();

//int disable_image_streaming_mode();

#endif /*EYELINK_2_CAM_IMAGE_H_*/
