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
#include <stdio.h>
#include <glib.h>
#include <gmodule.h>

#include <libGaze.h>
#include <libGaze_defines.h>
#include <libGaze_types.h>
#include <libGaze_error.h>
#include <eyetracker/libGaze_eyetracker_logging.h>


EYE_TRACKER *et_load_module(char* module_path, int debug, int error, enum eyetracker_modes mode){

	int merror = 0;

	//void *mod = NULL;
	GModule *mod = NULL;

	_DEBUG_("load eyetracker module: %s\n",module_path);
	mod = g_module_open(module_path,G_MODULE_BIND_LOCAL);
	if (mod == NULL){
		_ERROR_("%s\n",g_module_error ());
		return NULL;
	}

	EYE_TRACKER *eye;
	eye = (EYE_TRACKER*)calloc(1,sizeof(EYE_TRACKER));
	if(eye == NULL){
		_ERROR_("could allocate memory for EYE_TRACKER\n");
		return NULL;
	}
	eye->module=mod;

	if(!g_module_symbol(mod,EYETRACKER_GET_TRACKER_FREQUENCY,(gpointer*)&(eye->get_tracker_frequency))){
		_ERROR_("%s\n",g_module_error());
		merror++;
	}

	if(!g_module_symbol(mod,EYETRACKER_CONNECT,(gpointer*)&(eye->connect_to_eye_tracker))){
		_ERROR_("%s\n",g_module_error());
		merror++;
	}

	if(!g_module_symbol(mod,EYETRACKER_DISCONNECT,(gpointer*)&(eye->disconnect_eye_tracker))){
		_ERROR_("%s\n",g_module_error());
		merror++;
	}

	if(!g_module_symbol(mod,EYETRACKER_CONFIGURE,(gpointer*)&(eye->configure_eye_tracker))){
		_ERROR_("%s\n",g_module_error());
		merror++;
	}

	if(!g_module_symbol(mod,EYETRACKER_THRESHOLDS,(gpointer*)&(eye->get_fixation_thresholds))){
		_ERROR_("%s\n",g_module_error());
		merror++;
	}
	/*
	if(!g_module_symbol(mod,"add_draw_cal_target",(gpointer*)&(eye->add_draw_cal_target))){

		_ERROR_("%s\n",g_module_error());
		merror++;
	}

	if(!g_module_symbol(mod,"add_erase_cal_target",(gpointer*)&(eye->add_erase_cal_target))){

		_ERROR_("%s\n",g_module_error());
		merror++;
	}
*/
	if(!g_module_symbol(mod,EYETRACKER_GET_TRACKING_DATA,(gpointer*)&(eye->get_eyetracking_data))){

		_ERROR_("%s\n",g_module_error());
		merror++;
	}

	if(!g_module_symbol(mod,EYETRACKER_START_TRACKING,(gpointer*)&(eye->start_eye_tracking))){

		_ERROR_("%s\n",g_module_error());
		merror++;
	}
	if(!g_module_symbol(mod,EYETRACKER_STOP_TRACKING,(gpointer*)&(eye->stop_eye_tracking))){

		_ERROR_("%s\n",g_module_error());
		merror++;
	}

	if(!g_module_symbol(mod,EYETRACKER_IS_CONNECTED,(gpointer*)&(eye->is_eye_tracker_connected))){
		_ERROR_("%s\n",g_module_error());
		merror++;
	}

	if(!g_module_symbol(mod,EYETRACKER_EYE_AVAILABLE,(gpointer*)&(eye->eye_available))){
		_ERROR_("%s\n",g_module_error());
		merror++;
	}

	if(merror > 0){
		_ERROR_("eyetracker module does not implement all needed functions\n");
		return NULL;
	}

	int (*et_init)(int ,int ,FILE*,FILE*, struct timeval*);

	if(g_module_symbol(mod,EYETRACKER_INIT_MODULE,(gpointer*)&(et_init))){
		_DEBUG_("init: %p\n",et_init);
		(*et_init)(debug,error,lg_debug,lg_error, log_base_time);
	}else{
		_ERROR_("LibGaze:\tinit = NULL\n");
		return NULL;
	}

	_DEBUG_("init glib threads, if needed.\n");
	if (!g_thread_supported()) g_thread_init(NULL);

	eye->mutex_log_state = g_mutex_new();
	eye->mutex_log_file = g_mutex_new();
	eye->mutex_tracking_data = g_mutex_new();
	eye->mutex_log_thread = g_mutex_new();
	eye->mutex_track_state = g_mutex_new();
	eye->log_thread = NULL;

	eye->log_state=LOG_DISABLED;
	eye->track_state=TRACKING_DISABLED;
	eye->log_file = NULL;
	eye->tracking_data = NULL;

	eye->mode = mode;

	_DEBUG_("eyetracker: %p\n",eye);

	return eye;
}

enum eyetracker_return et_unload_module(EYE_TRACKER* eye_tracker)
{
if (eye_tracker!= NULL)
{
	if (eye_tracker->mode == EYETRACKER_MODE_NORMAL)
	{
		if(eye_tracker->module !=NULL)
		{
			//g_module_close(eye_tracker->module);
			int (*et_fini)(void);
			_DEBUG_("call fini_module\n");
			if(g_module_symbol(eye_tracker->module,EYETRACKER_FINI_MODULE,(gpointer*)&(et_fini)))
			{
//				_DEBUG_("finalize: %p\t\n",et_fini);
				(*et_fini)();
			}
			else
			{
				_ERROR_("%s\n",g_module_error());
				_ERROR_("et_fini = NULL\n");
				return LG_EYETRACKER_ERROR;
			}
		}
		else
		{
			free(eye_tracker);
			return LG_EYETRACKER_ERROR;
		}
		free(eye_tracker);
		return LG_EYETRACKER_OK;
	}
	else if (eye_tracker->mode == EYETRACKER_MODE_DUMMY)
	{
		return LG_EYETRACKER_OK;
	}
	else
	{
		return LG_EYETRACKER_ERROR;
	}
}
return LG_EYETRACKER_ERROR;
}


enum eyetracker_return et_get_tracker_frequency(EYE_TRACKER* eye_tracker, int *frequency){
	_DEBUG_("et_get_tracker_frequency\n");
	if (eye_tracker->mode == EYETRACKER_MODE_NORMAL){
		return (enum eyetracker_return)(*(eye_tracker->get_tracker_frequency))(frequency);
	}else if (eye_tracker->mode == EYETRACKER_MODE_DUMMY){
		*frequency = 60;
		return LG_EYETRACKER_OK;
	}
	return LG_EYETRACKER_ERROR;
}

enum eyetracker_return et_connect_to_eye_tracker(EYE_TRACKER* eye_tracker){
	if (eye_tracker->mode == EYETRACKER_MODE_NORMAL){
		return (*(eye_tracker->connect_to_eye_tracker))();
	}else if (eye_tracker->mode == EYETRACKER_MODE_DUMMY){
		return LG_EYETRACKER_OK;
	}else{
		return LG_EYETRACKER_ERROR;
	}

}

enum eyetracker_return et_disconnect_eye_tracker(EYE_TRACKER* eye_tracker){
	if (eye_tracker->mode == EYETRACKER_MODE_NORMAL){
		return (*(eye_tracker->disconnect_eye_tracker))();
	}else if (eye_tracker->mode == EYETRACKER_MODE_DUMMY){
		return LG_EYETRACKER_OK;
	}else{
		return LG_EYETRACKER_ERROR;
	}
}

enum eyetracker_return et_configure_eye_tracker(EYE_TRACKER* eye_tracker, char* cfg){
	_DEBUG_("et_configure_eye_tracker: %s\n",cfg);
	if (eye_tracker->mode == EYETRACKER_MODE_NORMAL){
		return (*(eye_tracker->configure_eye_tracker))(cfg);
	}else if (eye_tracker->mode == EYETRACKER_MODE_DUMMY){
		return LG_EYETRACKER_OK;
	}
	return LG_EYETRACKER_ERROR;
}

/*
int et_add_draw_cal_target(EYE_TRACKER* eye_tracker,void *context,void draw_cal_target(void*, int, int)){
	return (*(eye_tracker->add_draw_cal_target))(context,draw_cal_target);
}

int et_add_erase_cal_target(EYE_TRACKER* eye_tracker,void *context,void erase_cal_target(void*)){
	return (*(eye_tracker->add_erase_cal_target))(context,erase_cal_target);
}
*/
enum eyetracker_return et_get_eyetracking_data(EYE_TRACKER* eye_tracker,t_raw_eye_data* data){
	if (eye_tracker->mode == EYETRACKER_MODE_NORMAL){
		g_mutex_lock(eye_tracker->mutex_tracking_data);

		data->eye_available = eye_tracker->tracking_data->eye_available;
		data->time = eye_tracker->tracking_data->time;

		data->pupil_x[0] = eye_tracker->tracking_data->pupil_x[0];
		data->pupil_y[0] = eye_tracker->tracking_data->pupil_y[0];
		data->pupil_size[0] = eye_tracker->tracking_data->pupil_size[0];

		data->pupil_x[1] = eye_tracker->tracking_data->pupil_x[1];
		data->pupil_y[1] = eye_tracker->tracking_data->pupil_y[1];
		data->pupil_size[1] = eye_tracker->tracking_data->pupil_size[1];
		int ireturn;
		ireturn = eye_tracker->tracking_data_state;
		eye_tracker->tracking_data_state = LG_EYETRACKER_NO_NEW_DATA_AVAILABLE;
		g_mutex_unlock(eye_tracker->mutex_tracking_data);
		//return (*(eye_tracker->get_eyetracking_data))(data);
		return (enum eyetracker_return)ireturn;
	}else if (eye_tracker->mode == EYETRACKER_MODE_DUMMY){
		data->eye_available = LG_EYE_BOTH;
		data->time =lg_get_time();

		data->pupil_x[0] = 0;
		data->pupil_y[0] = 0;
		data->pupil_size[0] = 0;

		data->pupil_x[1] = 0;
		data->pupil_y[1] = 0;
		data->pupil_size[1] = 0;
		return LG_EYETRACKER_NEW_DATA;
	}else{
		return LG_EYETRACKER_ERROR;
	}
}

enum eyetracker_return et_start_eye_tracking(EYE_TRACKER* eye_tracker){
	if (eye_tracker->mode == EYETRACKER_MODE_NORMAL){
		g_mutex_lock( eye_tracker->mutex_track_state);
		eye_tracker->track_state = TRACKING_ENABLED;
		g_mutex_unlock( eye_tracker->mutex_track_state);
		free(eye_tracker->tracking_data);
		eye_tracker->tracking_data = (t_raw_eye_data*)calloc(1,sizeof(t_raw_eye_data));
		eye_tracker->log_thread =g_thread_create((GThreadFunc)eyetracker_logging_main,(gpointer)eye_tracker,1,(GError**)&eye_tracker->thread_error);
		g_thread_set_priority(eye_tracker->log_thread,G_THREAD_PRIORITY_NORMAL);

		return (*(eye_tracker->start_eye_tracking))();
	}else if (eye_tracker->mode == EYETRACKER_MODE_DUMMY){
		return LG_EYETRACKER_OK;
	}else{
		return LG_EYETRACKER_ERROR;
	}
}

enum eyetracker_return et_stop_eye_tracking(EYE_TRACKER* eye_tracker){
	if (eye_tracker->mode == EYETRACKER_MODE_NORMAL){
		g_mutex_lock( eye_tracker->mutex_track_state);
		eye_tracker->track_state = TRACKING_DISABLED;
		g_mutex_unlock( eye_tracker->mutex_track_state);
		g_thread_join(eye_tracker->log_thread);
		return (*(eye_tracker->stop_eye_tracking))();
	}else if (eye_tracker->mode == EYETRACKER_MODE_DUMMY){
		return LG_EYETRACKER_OK;
	}else{
		return LG_EYETRACKER_ERROR;
	}
}

/*
int et_start_eye_tracker_calibration(EYE_TRACKER* eye_tracker){
	return (*(eye_tracker->start_eye_tracker_calibration))();
}

int	et_go_to_setup(EYE_TRACKER* eye_tracker){
		return (*(eye_tracker->go_to_setup))();
}

*/
enum eyetracker_return et_is_eye_tracker_connected(EYE_TRACKER* eye_tracker){
	if (eye_tracker->mode == EYETRACKER_MODE_NORMAL){
		return (*(eye_tracker->is_eye_tracker_connected))();
	}else if (eye_tracker->mode == EYETRACKER_MODE_DUMMY){
		return LG_EYETRACKER_IS_CONNECTED;
	}else{
		return LG_EYETRACKER_ERROR;
	}
}

enum eyetracker_return et_eye_available(EYE_TRACKER* eye_tracker, enum lg_eyes* eye){
	if (eye_tracker->mode == EYETRACKER_MODE_NORMAL){
		return (*(eye_tracker->eye_available))(eye);
	}else if (eye_tracker->mode == EYETRACKER_MODE_DUMMY){
		return (enum eyetracker_return)LG_EYE_BOTH;
	}else{
		return LG_EYETRACKER_ERROR;
	}
}


/*
int et_get_camera_image(EYE_TRACKER* eye_tracker, int eye, t_image* img){
		return (*(eye_tracker->get_camera_image))(eye,img);

}

int et_enable_image_streaming_mode(EYE_TRACKER* eye_tracker){
		return (*(eye_tracker->enable_image_streaming_mode))();

}

int et_disable_image_streaming_mode(EYE_TRACKER* eye_tracker){
		return (*(eye_tracker->disable_image_streaming_mode))();

}

int et_send_input(EYE_TRACKER* eye_tracker,t_input input){
		return (*(eye_tracker->send_input))(input);

}
*/

