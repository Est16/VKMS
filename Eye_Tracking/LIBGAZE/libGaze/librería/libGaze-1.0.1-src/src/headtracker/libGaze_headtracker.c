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
#include <libGaze_types.h>
#include <libGaze_error.h>
#include <headtracker/libGaze_headtracker_logging.h>

HEAD_TRACKER* ht_load_module(char *module_path,int debug, int error, enum headtracker_modes mode){
	_DEBUG_("ht_load_module: %d\t%d\t%s\n",debug,error,module_path);
	GModule *mod;
	mod = g_module_open(module_path,G_MODULE_BIND_LOCAL);
	if(mod==NULL){
		_ERROR_("%s\n",g_module_error());
		return NULL;
	}


	HEAD_TRACKER *head;
	head = (HEAD_TRACKER*)calloc(1,sizeof(HEAD_TRACKER));
	if(head == NULL){
		_ERROR_("could allocate memory for HEAD_TRACKER\n");
		return NULL;
	}
	head->module = mod;

	int merror =0;

	if(!g_module_symbol(mod,HEADTRACKER_CONFIGURE,(gpointer*)&(head->configure_head_tracker))){

		_ERROR_("%s\n",g_module_error());
		merror ++;
	}

	if(!g_module_symbol(mod,HEADTRACKER_GET_TRACKER_FREQUENCY,(gpointer*)&(head->get_tracker_frequency))){

		_ERROR_("%s\n",g_module_error());
		merror ++;
	}

	if(!g_module_symbol(mod,HEADTRACKER_CONNECT,(gpointer*)&(head->connect_to_head_tracker))){

		_ERROR_("%s\n",g_module_error());
		merror ++;
	}

	if(!g_module_symbol(mod,HEADTRACKER_DISCONNECT,(gpointer*)&(head->disconnect_head_tracker))){

		_ERROR_("%s\n",g_module_error());
		merror ++;
	}

	if(!g_module_symbol(mod,HEADTRACKER_START_TRACKING,(gpointer*)&(head->start_head_tracking))){

		_ERROR_("%s\n",g_module_error());
		merror ++;
	}

	if(!g_module_symbol(mod,HEADTRACKER_STOP_TRACKING,(gpointer*)&(head->stop_head_tracking))){
		_ERROR_("%s\n",g_module_error());
		merror ++;
	}

	//printf("get_tracking_data\n");
	if(!g_module_symbol(mod,HEADTRACKER_GET_TRACKING_DATA,(gpointer*)&(head->get_tracking_data))){

		_ERROR_("%s\n",g_module_error());
		merror ++;
	}

	if(!g_module_symbol(mod,HEADTRACKER_IS_CONNECTED,(gpointer*)&(head->is_head_tracker_connected))){
		_ERROR_("%s\n",g_module_error());
		merror ++;
	}

	if(merror>0){
		_ERROR_("headtracker module does not implement all needed functions\n");
		return NULL;
	}

	int (*ht_init)(int, int ,FILE*,FILE*,struct timeval*);
	_DEBUG_("init\n");
	if(g_module_symbol(mod,HEADTRACKER_INIT_MODULE,(gpointer*)&(ht_init))){

		_DEBUG_("init: %p\tdebug: %p\terror:%p\n",ht_init,lg_debug,lg_error);
		(*ht_init)(debug,error,lg_debug,lg_error, log_base_time);
	}else{
		_ERROR_("%s\n",g_module_error());
		_ERROR_("ht_init = NULL\n");
		return NULL;
	}

	_DEBUG_("init glib threads, if needed.\n");
	if (!g_thread_supported()) g_thread_init(NULL);

	head->mutex_log_state = g_mutex_new();
	head->mutex_log_file = g_mutex_new();
	head->mutex_tracking_data = g_mutex_new();
	head->mutex_log_thread = g_mutex_new();
	head->mutex_track_state = g_mutex_new();
	head->log_thread = NULL;

	head->log_state=LOG_DISABLED;
	head->track_state=TRACKING_DISABLED;
	head->log_file = NULL;
	head->tracking_data = NULL;

	head->mode = mode;

	_DEBUG_("end_load_module\n");
	return head;
}

enum headtracker_return ht_unload_module(HEAD_TRACKER* head){
	if(head != NULL){
		if (head->module != NULL){
			//g_module_close(head->module);
			int (*ht_fini)(void);
			_DEBUG_("call fini_module\n");
			if(g_module_symbol(head->module,HEADTRACKER_FINI_MODULE,(gpointer*)&(ht_fini)))
			{
//				_DEBUG_("finalize: %p\t\n",ht_fini);
			(*ht_fini)();
		}else{
				_ERROR_("%s\n",g_module_error());
				_ERROR_("ht_fini = NULL\n");
				return LG_HEADTRACKER_ERROR;
			}

			free(head);
		return LG_HEADTRACKER_OK;
		}else{
			_ERROR_("unload_module: module = NULL\n");
			free(head);
		}

	}
	_ERROR_("unload_module: headtracker = NULL\n");
	return LG_HEADTRACKER_ERROR;
}

enum headtracker_return ht_get_tracker_frequency(HEAD_TRACKER* head, int *frequency){
	_DEBUG_("ht_get_tracker_frequency\n");
	if (head->mode == HEADTRACKER_MODE_NORMAL){
		return (enum headtracker_return)(*(head->get_tracker_frequency))(frequency);
	}else if (head->mode == HEADTRACKER_MODE_DUMMY){
		*frequency = 60;
		return LG_HEADTRACKER_OK;
	}else{
		return LG_HEADTRACKER_ERROR;
	}
}

enum headtracker_return ht_connect_to_head_tracker(HEAD_TRACKER* head){
	if (head->mode == HEADTRACKER_MODE_NORMAL){
		return (enum headtracker_return)(*(head->connect_to_head_tracker))();
	}else if (head->mode == HEADTRACKER_MODE_DUMMY){
		return LG_HEADTRACKER_OK;
	}else{
		return LG_HEADTRACKER_ERROR;
	}
}

enum headtracker_return ht_disconnect_head_tracker(HEAD_TRACKER* head){
	if (head->mode == HEADTRACKER_MODE_NORMAL){
		return (enum headtracker_return)(*(head->disconnect_head_tracker))();
	}else if (head->mode == HEADTRACKER_MODE_DUMMY){
		return LG_HEADTRACKER_OK;
	}else{
		return LG_HEADTRACKER_ERROR;
	}
}

enum headtracker_return ht_configure_head_tracker(HEAD_TRACKER* head, char* filename){
	_DEBUG_("ht_configure_head_tracker: %s\n",filename);
	if (head->mode == HEADTRACKER_MODE_NORMAL){
			return (enum headtracker_return)(*(head->configure_head_tracker))(filename,&(head->ihead),&(head->ieye),&(head->num));
	}else if (head->mode == HEADTRACKER_MODE_DUMMY){
		return LG_HEADTRACKER_OK;
	}else{
		return LG_HEADTRACKER_ERROR;
	}
}

enum headtracker_return ht_start_head_tracking(HEAD_TRACKER* head){
	if (head->mode == HEADTRACKER_MODE_NORMAL){
		g_mutex_lock( head->mutex_track_state);
		head->track_state = TRACKING_ENABLED;
		g_mutex_unlock( head->mutex_track_state);
		free(head->tracking_data);
		head->tracking_data = (t_raw_head_data*)calloc(head->num,sizeof(t_raw_head_data));
		head->log_thread =g_thread_create((GThreadFunc)headtracker_logging_main,(gpointer)head,1,(GError**)&head->thread_error);
		g_thread_set_priority(head->log_thread,G_THREAD_PRIORITY_NORMAL);
		return (enum headtracker_return)(*(head->start_head_tracking))();
	}else if (head->mode == HEADTRACKER_MODE_DUMMY){
		return LG_HEADTRACKER_OK;
	}else{
		return LG_HEADTRACKER_ERROR;
	}
}

enum headtracker_return ht_stop_head_tracking(HEAD_TRACKER* head){
	if (head->mode == HEADTRACKER_MODE_NORMAL){
		g_mutex_lock( head->mutex_track_state);
		head->track_state = TRACKING_DISABLED;
		g_mutex_unlock( head->mutex_track_state);
		g_thread_join(head->log_thread);
		return (enum headtracker_return)(*(head->stop_head_tracking))();
	}else if (head->mode == HEADTRACKER_MODE_DUMMY){
		return LG_HEADTRACKER_OK;
	}else{
		return LG_HEADTRACKER_ERROR;
	}
}

enum headtracker_return ht_get_tracking_data(HEAD_TRACKER* head ,int idx, t_raw_head_data* hdata){
	if (head->mode == HEADTRACKER_MODE_NORMAL){
		//printf("HEADTRACKER_MODE_NORMAL\n");
		if(idx >=0 && idx < head->num){
				if(hdata != NULL){
					//t_body_data bd;
					g_mutex_lock(head->mutex_tracking_data);
					/*
					hdata->time = lg_get_time();

					hdata->position[0] = head->tracking_data[idx].position[0];
					hdata->position[2] = head->tracking_data[idx].position[1];
					hdata->position[1] = head->tracking_data[idx].position[2];

					hdata->matrix[0] = head->tracking_data[idx].matrix[0];
					hdata->matrix[2] = head->tracking_data[idx].matrix[1];
					hdata->matrix[1] = head->tracking_data[idx].matrix[2];

					hdata->matrix[6] = head->tracking_data[idx].matrix[3];
					hdata->matrix[8] = head->tracking_data[idx].matrix[4];
					hdata->matrix[7] = head->tracking_data[idx].matrix[5];

					hdata->matrix[3] = head->tracking_data[idx].matrix[6];
					hdata->matrix[5] = head->tracking_data[idx].matrix[7];
					hdata->matrix[4] = head->tracking_data[idx].matrix[8];
					*/

					hdata->time = lg_get_time();

					hdata->button = head->tracking_data[idx].button;

					hdata->position[0] = head->tracking_data[idx].position[0];
					hdata->position[1] = head->tracking_data[idx].position[1];
					hdata->position[2] = head->tracking_data[idx].position[2];

					hdata->matrix[0] = head->tracking_data[idx].matrix[0];
					hdata->matrix[1] = head->tracking_data[idx].matrix[1];
					hdata->matrix[2] = head->tracking_data[idx].matrix[2];

					hdata->matrix[3] = head->tracking_data[idx].matrix[3];
					hdata->matrix[4] = head->tracking_data[idx].matrix[4];
					hdata->matrix[5] = head->tracking_data[idx].matrix[5];

					hdata->matrix[6] = head->tracking_data[idx].matrix[6];
					hdata->matrix[7] = head->tracking_data[idx].matrix[7];
					hdata->matrix[8] = head->tracking_data[idx].matrix[8];

					//_ERROR_("hdata->matrix[6]%f",hdata->matrix[6]);
					//_ERROR_("hdata->matrix[7]%f",hdata->matrix[7]);
					//_ERROR_("hdata->matrix[8]%f",hdata->matrix[8]);

					g_mutex_unlock(head->mutex_tracking_data);

					return LG_HEADTRACKER_NEW_DATA;
				}else{
					return LG_HEADTRACKER_ERROR;
				}
			}else{
				return LG_HEADTRACKER_ERROR;
			}
	}else if (head->mode == HEADTRACKER_MODE_DUMMY){
		//printf("HEADTRACKER_MODE_DUMMY\n");
		hdata->time = lg_get_time();

		hdata->position[0] = 0;
		hdata->position[2] = 0;
		hdata->position[1] = 0;

		hdata->matrix[0] = 1;
		hdata->matrix[2] = 0;
		hdata->matrix[1] = 0;

		hdata->matrix[6] = 0;
		hdata->matrix[8] = 1;
		hdata->matrix[7] = 0;

		hdata->matrix[3] = 0;
		hdata->matrix[5] = 0;
		hdata->matrix[4] = 1;
		return LG_HEADTRACKER_NEW_DATA;
	}else{
		return LG_HEADTRACKER_ERROR;
	}
}

enum headtracker_return ht_is_head_tracker_connected(HEAD_TRACKER* head){
	if (head->mode == HEADTRACKER_MODE_NORMAL){
		return (enum headtracker_return)(*(head->is_head_tracker_connected))();
	}else if (head->mode == HEADTRACKER_MODE_DUMMY){
		return LG_HEADTRACKER_IS_CONNECTED;
	}else{
		return LG_HEADTRACKER_ERROR;
	}
}



