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
#include <sys/time.h>


#include<glib/gthread.h>

#include "eyelink_2_logging.h"
#include "eyelink_2_hooks.h"
#include "eyelink_2_cam_image.h"
#include "eyelink_2.h"

#if defined __WIN32__ || defined WIN32
#include <windows.h>
#define MILLISEC_SLEEP(s) Sleep(s)
#else
#include <sys/time.h>
#define MILLISEC_SLEEP(s) usleep(s*1000)
#endif


t_eye_tracker_config *et_config = NULL;

enum eyetracker_return init_eye_tracker_module(int adebug, int aerror, FILE *fdebug, FILE *ferror){
	//if(!g_thread_supported()) g_thread_init(NULL);

	//mutex_lock_image = g_mutex_new();

	eyeerror =aerror;
	//eyedebug = adebug;
	eyedebug = 0;
	//log_base_time = btime;
	//el2_error = ferror;
	//el2_debug = fdebug;
	el2_error = stderr;
	el2_debug = stdout;
	_DEBUG_("init_module\n");
	//enable_logging = 0;
	return LG_EYETRACKER_OK;
}


enum eyetracker_return connect_to_eye_tracker(void){
	_DEBUG_("connect_to_eye_tracker\n");
	if (et_config==NULL){
		return LG_EYETRACKER_IS_NOT_CONFIGURED;
	}
	set_eyelink_address(et_config->et_ip);
	_DEBUG_("connect to: %s\n",et_config->et_ip);
	/*checks if it can open a connection to the eye tracker*/
	if(open_eyelink_connection(0))
	  return LG_EYETRACKER_COULD_NOT_CONNECT;
	_DEBUG_("configure connected eyelink\n");
	configure(et_config);
	//eyelink_flush_keybuttons(0);
	//flush_getkey_queue();
	//eyelink_flush_input_event();
	/*sets tracker in idle mode*/
	//set_offline_mode();

	return LG_EYETRACKER_OK;
}

enum eyetracker_return disconnect_eye_tracker(void){

	_DEBUG_("disconnect_eye_tracker\n");
	if(eyelink_close(1)==0){
		return LG_EYETRACKER_OK;
	}else{
		return LG_EYETRACKER_ERROR;
	}

}


enum eyetracker_return configure_eye_tracker(void *config){

	_DEBUG_("configure_eye_tracker: %s\n",(char*)config);
	et_config = parse_config_file2((char*)config);

	if(et_config != NULL){
		//et_config->screen_h = display->height;
		//et_config->screen_w = display->width;
		et_config->screen_x = 0;
		et_config->screen_y = 0;
		return LG_EYETRACKER_OK;
	}
	return LG_EYETRACKER_CFG_FILE_FORMAT_ERROR;
}



enum eyetracker_return get_eyetracking_data(t_raw_eye_data* data){
	//_DEBUG_("get_eyetracking_data\n");
	//eyelink_send_message("get_curent_gaze_coordinates");
	int i;
	ALLF_DATA evt;
	eyelink_flush_keybuttons(0);

	i = eyelink_newest_sample(NULL);
	if(i==-1){	/*No Data arrived*/

		return LG_EYETRACKER_NO_DATA_AVAILABLE;
	}else if (i== 0){
		return LG_EYETRACKER_NO_NEW_DATA_AVAILABLE;
	}else if(i==1){

		eyelink_newest_float_sample(&evt);

		data->time = evt.fs.time;

		data->pupil_x[0]=evt.fs.px[LEFT_EYE];
		data->pupil_y[0]=evt.fs.py[LEFT_EYE];

		//_DEBUG_("px: %f\tpy: %f\n",data->pupil_x[0],data->pupil_y[0]);

		data->pupil_x[1]=evt.fs.px[RIGHT_EYE];
		data->pupil_y[1]=evt.fs.py[RIGHT_EYE];

		//_DEBUG_("px: %f\tpy: %f\n",data->pupil_x[0],data->pupil_y[0]);

		data->pupil_size[0]=evt.fs.pa[LEFT_EYE];
		data->pupil_size[1]=evt.fs.pa[RIGHT_EYE];


		//flags are not updated if pupil in not found
		//_DEBUG_("flags: %d\n",evt.fs.flags);
		if((data->pupil_x[0] != MISSING_DATA)&&(data->pupil_x[1]!=MISSING_DATA)){
			data->eye_available = LG_EYE_BOTH;
			return LG_EYETRACKER_NEW_DATA;
			//_DEBUG_("used BINOCULAR\n");
		}else if((data->pupil_x[0] != MISSING_DATA)&&(data->pupil_x[1]==MISSING_DATA)){
			data->eye_available = LG_EYE_LEFT;
			return LG_EYETRACKER_NEW_DATA;
			//_DEBUG_("used LEFT_EYE\n");
		}else if((data->pupil_x[0] == MISSING_DATA)&&(data->pupil_x[1]!=MISSING_DATA)){
			data->eye_available = LG_EYE_RIGHT;
			return LG_EYETRACKER_NEW_DATA;
			//_DEBUG_("used RIGHT_EYE\n");
		}else{
			data->eye_available = LG_EYE_NONE;
			return LG_EYETRACKER_NO_DATA_AVAILABLE;
		}


	}

	return LG_EYETRACKER_ERROR;
}

enum eyetracker_return start_eye_tracking(void){
	_DEBUG_("start_eye_tracking\n");


	int error;

//	//_DEBUG_("if\n");
//	if(enable_logging==1){
//		//_DEBUG_("(enable_logging == 1)\n");
//		error = start_recording(1,1,1,1);   /* send events and samples through link and log_file*/
//	}else{
//		//_DEBUG_("else\n");
//		error = start_recording(0,0,1,1);   /* send events and samples through link and log_file*/
//	}
//	//_DEBUG_("endif\n");

	error = start_recording(0,0,1,1);   /* send events and samples through link and log_file*/


	if(error != 0)           /* ERROR: couldn't start recording*/
	{
	  	_ERROR_("Error occured when starting recoding mode\n");
		return LG_EYETRACKER_COULD_NOT_START_TRACKING;        /* Return the error code*/
	}


	//_DEBUG_("realtime\n");							  /* record for 100 msec before displaying stimulus */
	//begin_realtime_mode(100);   /* Windows 2000/XP: no interruptions from now on*/

	//_DEBUG_("flush\n");
	/*recodring loop*/
	eyelink_flush_keybuttons(0);

	if(!eyelink_wait_for_block_start(1000,1,1)){
		_ERROR_("Recording didn't start correctly\n");
		return LG_EYETRACKER_COULD_NOT_START_TRACKING;
	}

	return LG_EYETRACKER_OK;
}

enum eyetracker_return stop_eye_tracking(void){
	_DEBUG_("end_eye_tracking\n");
	stop_recording();
	set_offline_mode();
	return LG_EYETRACKER_OK;

}

/*
enum eyetracker_return open_log_file(char* log_file,char* path){
	char btmsg[128];

	_DEBUG_("open_log_file\n");

	struct timeval t;
 	gettimeofday(&t,0);

	unsigned long long bt = ((t.tv_sec - log_base_time->tv_sec)*1000000 + (t.tv_usec - log_base_time->tv_usec));
	bt = (unsigned long long)(bt/1000) ;

	if(log_file != NULL){
		open_log=1;
		log_file_name = (char*)malloc(256*sizeof(char));
		log_file_path = (char*)malloc(256*sizeof(char));
		strcpy(log_file_name,log_file);
		strcpy(log_file_path,path);
		if (eyecmd_printf("open_data_file %s",log_file)==OK_RESULT){
			sprintf(btmsg,"basetime: %lu",(unsigned long)bt);
			//_DEBUG_("%s\n",btmsg);
			add_log_msg(btmsg);
			return LG_EYETRACKER_OK;
		}
	}
	return LG_EYETRACKER_COULD_NOT_OPEN_LOG_FILE;
}

enum eyetracker_return close_log_file(){
	_DEBUG_("close_log_file\n");
	_DEBUG_("log_file_name: %s\n",log_file_name);
	if(open_log == 1){
		_DEBUG_("close_data_file\n");
		eyecmd_printf("close_data_file");
		_DEBUG_("receive_data_file\n");
		int i;
		//WINDOWS problem
		i = receive_data_file(log_file_name,log_file_path,1);
		_DEBUG_("receive_data_file: %d\n",i);
		 //i = receive_data_file(log_file_name,NULL,0);
		//i = receive_data_file("","",0);
		if(i>=0){
			return LG_EYETRACKER_OK;
		}else{
			return LG_EYETRACKER_COULD_NOT_CLOSE_LOG_FILE;
		}
	}
	return LG_EYETRACKER_NO_LOG_FILE_OPEN;
}

enum eyetracker_return start_logging(){
	_DEBUG_("start_logging\n");
	if(check_recording()==0 && enable_logging == OK_RESULT){
		stop_eye_tracking();
	}
	enable_logging = 1;
	//_DEBUG_("end: start_logging\n");
	return start_eye_tracking();

}

enum eyetracker_return stop_logging(){
	_DEBUG_("stop_logging\n");
	if(enable_logging == 1 && check_recording()==OK_RESULT){
		stop_eye_tracking();
		enable_logging = 0;
		return start_eye_tracking();
	}
	enable_logging = 0;
	return LG_EYETRACKER_OK;
}



int start_eye_tracker_calibration(void){
	_DEBUG_("start_eye_tracker_calibration\n");
	do_tracker_setup();
	return LG_EYETRACKER_OK;
}


enum eyetracker_return add_log_msg(char *msg){
	if(log_file_name != NULL){
		if(eyelink_send_message(msg)== OK_RESULT){
			return LG_EYETRACKER_OK;
		}
	}
	return LG_EYETRACKER_NO_LOG_FILE_OPEN;

}

*/

enum eyetracker_return is_eye_tracker_connected(void){
	if (eyelink_is_connected()!=0){
		_DEBUG_("is_eye_tracker_connected: et is connected\n");
		return LG_EYETRACKER_IS_CONNECTED;

	}else {
		_DEBUG_("is_eye_tracker_connected: et is not connected\n");
		return LG_EYETRACKER_NOT_CONNECTED;
	}
}

enum eyetracker_return eye_available(enum lg_eyes* eye){

	if(eyelink_is_connected()){

		int eyeused = eyelink_eye_available();
		switch (eyeused){

			case RIGHT_EYE:
				_DEBUG_("used RIGHT_EYE\n");
				*eye = LG_EYE_RIGHT;
				return LG_EYETRACKER_OK;
				break;

			case BINOCULAR:
				_DEBUG_("used BINOCULAR\n");
				*eye = LG_EYE_BOTH;
				return LG_EYETRACKER_OK;
				break;

			case LEFT_EYE:
				_DEBUG_("used LEFT_EYE\n");
				*eye = LG_EYE_LEFT;
				return LG_EYETRACKER_OK;
				break;
			default:
				*eye = LG_EYE_NONE;
				return LG_EYETRACKER_OK;
				break;
		}
	}
	return LG_EYETRACKER_NOT_CONNECTED;
}


enum eyetracker_return get_tracker_frequency(int* frequency){
	*frequency = et_config->frequency;
	return LG_EYETRACKER_OK;

}

enum eyetracker_return get_fixation_thresholds(float* threshold, float* deviation){
	*threshold = et_config->fix_threshold;
	*deviation = et_config->fix_deviation;
	return LG_EYETRACKER_OK;

}

/*
int send_input(t_input input){

	if(input.type == LG_INPUT_KEY){

		if(is_in_streaming_mode != 1){

			kstate = KEY_DOWN;
			key = input.key;
			kmod = input.mod;
			echo_key();
			echo_key();

		}else{
			while(kstate != KEY_NONE){
			}
			key = input.key;
			kstate = KEY_DOWN;
			kmod = input.mod;
		}

	}
	return 0;
}


int go_to_setup(){
	//do_tracker_setup();
	_DEBUG_("eyelink_start_setup()\n");
	eyelink_start_setup();

	eyelink_wait_for_mode_ready(1000);



	while (eyelink_in_setup()!=0){

		MILLISEC_SLEEP(500);
	}
*/
	//_DEBUG_("return 0\n");
/*
return 0;

}

*/
