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
#include <core_expt.h>

#include "eyelink_2.h"
#include "eyelink_2_cam_image.h"
#include "eyelink_2_hooks.h"

#include <sys/time.h>
int img_ncolours = 0;

int img_width = 0;
int img_height = 0;

int img_size = 0;	/*image size in byte*/

unsigned int *palette_rgba = NULL;

int get_image = 0;

unsigned int *img_current = NULL;
unsigned int *img_tmp = NULL;

int selected_eye = LG_EYE_NONE;
int current_eye = LG_EYE_NONE;
int img_next=1;
int key_iterator = 0;

unsigned int is_in_streaming_mode = 0;

GThread *eyelink_2_streaming_thread = NULL;
char *thread_error = NULL;

GMutex *mutex_lock_image = NULL;

int img_number = -1;

int streaming_mode(){
	
	do_tracker_setup();

	return 0;	
}

int enable_image_streaming_mode(){
	
	img_number = -1;
	
	eyelink_2_streaming_thread =g_thread_create((GThreadFunc)streaming_mode,NULL,1,(GError**)&thread_error);
	
	key = ENTER_KEY;
	kstate = KEY_DOWN;
	
	is_in_streaming_mode = 1;
	
	return 0;	
}

int disable_image_streaming_mode(){
	
	key = ESC_KEY;
	kstate = KEY_DOWN;
	
	g_thread_join(eyelink_2_streaming_thread);
	
	is_in_streaming_mode =0;
	
	return 0;
}

int get_camera_image(int eye,t_image *image){
	
	int current_mode = eyelink_current_mode();
	int disable_streaming = 0;
	
	selected_eye = eye;
	if (is_in_streaming_mode != 1){
		enable_image_streaming_mode();
		disable_streaming =1;
		eyelink_wait_for_mode_ready(1000);
		int mode;
		mode =  eyelink_current_mode();
		while(mode!=34/*IN_IMAGE_MODE*/){
			//_DEBUG_("mode: %d\n",mode);
			mode= eyelink_current_mode();
		}
	}
	
	while(current_eye != selected_eye){
		while(kstate!=KEY_NONE){
		}
		_DEBUG_("current_eye: %d\tselected_eye: %d\n",current_eye,selected_eye);
		img_next = 1;
		key = CURS_LEFT;
		kstate = KEY_DOWN;
		while (img_next==1){
			
		}

	}
	
	_DEBUG_("current_eye: %d\tselected_eye: %d\n",current_eye,selected_eye);
/*
	int mode;
	
	mode = eyelink_current_mode();
	
	
	switch(mode)
	{
	case IN_DISCONNECT_MODE:     // Recording mode
	    // Code for processing recording mode ;
	    _DEBUG_("IN_DISCONNECT_MODE\n");
	    break;
	case IN_IDLE_MODE:      // Image mode
	    // Code for processing image mode ;
	    _DEBUG_("IN_IDLE_MODE\n");
	    break;
	case IN_SETUP_MODE: // Setup menu mode
	    // Code for processing setup menu mode ;
	    _DEBUG_("IN_SETUP_MODE\n");
	    break;
	case IN_USER_MENU: // Calibration, validation, DC mode
		_DEBUG_("IN_USER_MENU\n");
		break;
	case IN_IMAGE_MODE:
		_DEBUG_("IN_IMAGE_MODE\n");
		break;
	case IN_TARGET_MODE:
		_DEBUG_("IN_TARGET_MODE\n");
		break;
	    // Code for processing calibration, validation, dc mode ;
	    break;
	case IN_RECORD_MODE: // Option menu mode
	    // Code for processing calibration mode ;
	    _DEBUG_("IN_RECORD_MODE\n");
	    break;
	default:
	    // Code for processing default case ;
	    break;
	}

	
	mode = eyelink_tracker_mode();
	
	
	switch(mode)
	{
	case EL_RECORD_MODE:     // Recording mode
	    // Code for processing recording mode ;
	    _DEBUG_("EL_RECORD_MODE\n");
	    break;
	case EL_IMAGE_MODE:      // Image mode
	    // Code for processing image mode ;
	    _DEBUG_("EL_IMAGE_MODE\n");
	    break;
	case EL_SETUP_MENU_MODE: // Setup menu mode
	    // Code for processing setup menu mode ;
	    _DEBUG_("EL_SETUP_MENU_MODE\n");
	    break;
	case EL_CALIBRATE_MODE: // Calibration, validation, DC mode
		_DEBUG_("EL_CALIBRATE_MODE\n");
		break;
	case EL_VALIDATE_MODE:
		_DEBUG_("EL_VALIDATE_MODE\n");
		break;
	case EL_DRIFT_CORR_MODE:
		_DEBUG_("EL_DRIFT_CORR_MODE\n");
		break;
	    // Code for processing calibration, validation, dc mode ;
	    break;
	case EL_OPTIONS_MENU_MODE: // Option menu mode
	    // Code for processing calibration mode ;
	    _DEBUG_("EL_OPTIONS_MENU_MODE\n");
	    break;
	default:
	    // Code for processing default case ;
	    break;
	}
*/	
	
	while(img_number<=0){
			//_DEBUG_("t1\n");
	}
	g_mutex_lock(mutex_lock_image);
	image->width = img_width;
	image->height = img_height;
	image->pixels = img_current;
	image->size = img_size;
	img_number = -1;
	g_mutex_unlock(mutex_lock_image);
	
	if (disable_streaming ==1){
		disable_image_streaming_mode();
		if(current_mode== IN_RECORD_MODE){
			start_eye_tracking();
			
		}	
	}
	
	return 0;
}
