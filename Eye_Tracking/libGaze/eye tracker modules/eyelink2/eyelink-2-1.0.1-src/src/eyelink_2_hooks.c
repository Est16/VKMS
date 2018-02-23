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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libGaze/libGaze_keys.h>
#include "eyelink_2.h"
#include "eyelink_2_error.h"
#include "eyelink_2_cam_image.h"
#include "eyelink_2_hooks.h"


int key = 0;
int kstate = 0;
int kmod = 0;

void* __context = NULL;
void (*context_draw_cal_target)(void*, INT16, INT16) = NULL;
void (*context_erase_cal_target)(void*)= NULL;

static int catch_image;

int add_draw_cal_target(void *context, void draw_cal_target(void*,int, int)){
	_DEBUG_("Eyeink2:\tadd_draw_cal_target\n");
	context_draw_cal_target = draw_cal_target;
	__context = context;
	return 0;
}

int add_erase_cal_target(void *context,void erase_cal_target(void*)){
	_DEBUG_("Eyeink2:\tadd_erase_cal_target\n");
	context_erase_cal_target = erase_cal_target;
	__context = context;
	return 0;
}


INT16 my_setup_cal_display_hook(void){
	_DEBUG_("Eyeink2:\tmy_setup_cal_display_hook\n");
	return 0;
}
void my_exit_cal_display_hook(void){
	_DEBUG_("Eyeink2:\tmy_exit_cal_display_hook\n");
}
void my_record_abort_hide_hook(void){
	_DEBUG_("Eyeink2:\tmy_record_abort_hide_hook\n");
}
INT16 my_setup_image_display_hook(INT16 width, INT16 height){
	//_DEBUG_("Eyeink2:\tmy_setup_image_display_hook: %d \t %d\n",width,height);
	img_width = width;
	img_height = height;

	img_size = width*height*sizeof(unsigned int);

	img_current = (unsigned int*)malloc(img_size);

	img_tmp = (unsigned int*)malloc(img_size);

	return 0;
}
void my_image_title_hook(INT16 threshold, char *cam_name){
	_DEBUG_("my_image_title_hook: %d\t%s\n",threshold, cam_name);

	if(strstr(cam_name,"LEFT")!=NULL){
		current_eye = LG_EYE_LEFT;
	}else if(strstr(cam_name,"RIGHT")!=NULL){
		current_eye = LG_EYE_RIGHT;
	}
	if(current_eye == selected_eye){
		catch_image=1;
	}else{
		catch_image=0;
		//key = CURS_LEFT;
		//kstate = KEY_DOWN;
	}
	img_next=0;
	_DEBUG_("my_image_title_hook: end\n");
}
int count = 0;
int end = 0;
void my_draw_image_line_hook(INT16 width, INT16 line, INT16 totlines, byte *pixels){
	//_DEBUG_("Eyeink2:\tmy_draw_image_line_hook: %d\t%d\t%d\n",width,line,totlines);
	if(catch_image){
		if(count == 0 && line ==1){
			//_DEBUG_("start drawing\n");
			count =1;
		}
		if(count > 0){
			if (count== line){
				int l;
				//int tmp = (line-1)*img_width;
				//int tmp = (totlines-line)*img_width;
				int tmp = (line-1)*img_width;
				for (l=0;l<width;l++){
					img_tmp[tmp+l] = palette_rgba[pixels[l]&0x000000FF];
					//img_left[tmp+l] = (unsigned int)0x00FF0000;
				}
				count++;
			}else{
				_ERROR_("missed line\n");
			}
		}

		if(count > totlines){
			//_DEBUG_("end drawing\n");
			g_mutex_lock(mutex_lock_image);
			memcpy(img_current,img_tmp,(img_width*img_height*sizeof(unsigned int)));
			img_number++;
			g_mutex_unlock(mutex_lock_image);

			count = 0;
			end=1;
		}
	}


}
void my_set_image_palette_hook(INT16 ncolors, byte r[],
                         byte g[], byte b[]){
	//_DEBUG_("Eyeink2:\tmy_set_image_palette_hook\n");
	img_ncolours = ncolors;
	//_DEBUG_("ncolors: %d\n",ncolors);
	//_DEBUG_("sizeof(r): %d\n",sizeof(r));
	//_DEBUG_("sizeof(g): %d\n",sizeof(g));
	//_DEBUG_("sizeof(b): %d\n",sizeof(b));

	palette_rgba = (unsigned int*)malloc(ncolors*sizeof(unsigned int));
	int k;
	for(k=0;k<ncolors;k++){
		palette_rgba[k] =(((r[k])|(g[k]<<8)| (b[k]<<16)) | 0x000000FF<<24);
		//palette_rgba[k] =(((0<<24)|(0<<16)| (0<<8)) | 0x000000FF);
		//palette_rgba[k] =0x000000FF;
	}
}
void my_exit_image_display_hook(void){
	_DEBUG_("Eyeink2:\tmy_exit_image_display_hook\n");
	/*
	int l;
	int tmp = (img_height*img_width);
	for(l=0;l<tmp;l++){
		printf("%d\n",img_left[l]);
	}
	*/

}
void my_clear_cal_display_hook(){
	_DEBUG_("Eyeink2:\tmy_clear_cal_display_hook\n");
}
void my_erase_cal_target_hook(){
	_DEBUG_("Eyeink2:\tmy_erase_cal_target_hook\n");
	(*context_erase_cal_target)(__context);
}
void my_draw_cal_target_hook(INT16 x, INT16 y){
	_DEBUG_("Eyeink2:\tmy_draw_cal_target_hook:\tx: %d\ty: %d\n",x,y);
	(*context_draw_cal_target)(__context,x,y);
}
void my_cal_target_beep_hook(void){
	_DEBUG_("Eyeink2:\tmy_cal_target_beep_hook\n");
}
void my_cal_done_beep_hook(INT16 error){
	_DEBUG_("Eyeink2:\tmy_cal_done_beep_hook\n");
}
void my_dc_done_beep_hook(INT16 error){
	_DEBUG_("Eyeink2:\tmy_dc_done_beep_hook\n");
}
void my_dc_target_beep_hook(void){
	_DEBUG_("Eyeink2:\tmy_dc_target_beep_hook\n");
}


short my_get_input_key_hook(InputEvent * event){

	//_DEBUG_("Eyeink2:\tmy_get_input_key_hook\n");


	if(kstate!=KEY_NONE){
		event->type = KEYINPUT_EVENT;
		event->key.key =  key;
		if(kstate == KEY_DOWN){
			_DEBUG_("KEY_DOWN: %d\n",key);
			event->key.state = 0;
			kstate = KEY_UP;
		}else if (kstate == KEY_UP){
			_DEBUG_("KEY_UP\n");
			event->key.state = 1;
			kstate = KEY_NONE;
			kmod = KMOD_NONE;
		}
		return 1;

	}
	return 0;

}

void my_alert_printf_hook(const char * c){
	_DEBUG_("Eyeink2:\tmy_alert_printf_hook\n");
}

void init_my_hooks(HOOKFCNS *hooks){

	kstate = KEY_NONE;

	hooks->setup_cal_display_hook=my_setup_cal_display_hook;
	hooks->exit_cal_display_hook=my_exit_cal_display_hook;
	hooks->record_abort_hide_hook=my_record_abort_hide_hook;
	hooks->setup_image_display_hook=my_setup_image_display_hook;
	hooks->image_title_hook=my_image_title_hook;
	hooks->draw_image_line_hook=my_draw_image_line_hook;
	hooks->set_image_palette_hook=my_set_image_palette_hook;
	hooks->exit_image_display_hook=my_exit_image_display_hook;
	hooks->clear_cal_display_hook=my_clear_cal_display_hook;
	hooks->erase_cal_target_hook=my_erase_cal_target_hook;
	hooks->draw_cal_target_hook=my_draw_cal_target_hook;
	hooks->cal_target_beep_hook=my_cal_target_beep_hook;
	hooks->cal_done_beep_hook=my_cal_done_beep_hook;
	hooks->dc_done_beep_hook=my_dc_done_beep_hook;
	hooks->dc_target_beep_hook=my_dc_target_beep_hook;
	hooks->get_input_key_hook=my_get_input_key_hook;
	hooks->alert_printf_hook=my_alert_printf_hook;
}
