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

#include <stdio.h>
#include <stdlib.h>
#include <glib/gthread.h>

#include <libGaze.h>
#include <libGaze_types.h>
#include <libGaze_error.h>
#include <libGaze_time.h>
#include <headtracker/libGaze_headtracker_logging.h>

void headtracker_logging_main(HEAD_TRACKER *headtracker){

	//int count =0;
	int n;

	n = headtracker->num;
	int hfrequency;
	ht_get_tracker_frequency(headtracker,&hfrequency);
	int timetosleep = (int)(1000.0/(2.0*(double)hfrequency));
	_DEBUG_("timetosleep: %d\n",timetosleep);
	int state;
	g_mutex_lock(headtracker->mutex_log_state);

	if(headtracker->log_state == LOG_ENABLED){
		//fprintf(log_file,"MSG: let the logging begin\n");
		fprintf(headtracker->log_file,"M\tlocaltime(milisec)\ttrackertime(milisec)\tnum\tposition: \tx,\ty,\tz\tmatrix: \t00,\t01,\t02,\t10,\t11,\t12,\t20,\t21,\t22\n");
	}
	g_mutex_unlock(headtracker->mutex_log_state);

	g_mutex_lock( headtracker->mutex_track_state);
	state = headtracker->track_state;
	g_mutex_unlock( headtracker->mutex_track_state);
	/*
	struct timeval t;
 	gettimeofday(&t,0);

	unsigned long long lot = ((t.tv_sec - log_base_time.tv_sec)*1000000 + (t.tv_usec - log_base_time.tv_usec));
	lot = (unsigned long long)(lot/1000) ;
	*/
	unsigned long long lot = lg_get_time();
	_DEBUG_("time: %llu\n",lot);



	int i;

	while(state != TRACKING_DISABLED){

		unsigned long long lt = lg_get_time();
		g_mutex_lock(headtracker->mutex_tracking_data);
		int ireturn;
		ireturn = (*(headtracker->get_tracking_data))(headtracker->tracking_data);
		g_mutex_unlock(headtracker->mutex_tracking_data);
		if(ireturn ==LG_HEADTRACKER_NEW_DATA){


			g_mutex_lock(headtracker->mutex_log_state);

			if(headtracker->log_state == LOG_ENABLED){

				g_mutex_lock(headtracker->mutex_log_file);
				if(headtracker->log_file != NULL){

					//_DEBUG_("%lu:\t%f,\t%f,\t%f,\t%f,\t%f,\t%f\n",(unsigned long )lt,vicon_tracking_data.tx,vicon_tracking_data.ty,vicon_tracking_data.tz,vicon_tracking_data.qx,vicon_tracking_data.qy,vicon_tracking_data.qz);
					for (i=0;i<n;i++){

						fprintf(headtracker->log_file,"H\t%lu\t%lu\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n"
													,(unsigned long)lt,(unsigned long)headtracker->tracking_data[i].time,i,headtracker->tracking_data[i].position[0],headtracker->tracking_data[i].position[1],headtracker->tracking_data[i].position[2],
													headtracker->tracking_data[i].matrix[0],headtracker->tracking_data[i].matrix[1],headtracker->tracking_data[i].matrix[2],
													headtracker->tracking_data[i].matrix[3],headtracker->tracking_data[i].matrix[4],headtracker->tracking_data[i].matrix[5],
													headtracker->tracking_data[i].matrix[6],headtracker->tracking_data[i].matrix[7],headtracker->tracking_data[i].matrix[8]);

					}
				}
				g_mutex_unlock(headtracker->mutex_log_file);

			}
			g_mutex_unlock(headtracker->mutex_log_state);


			g_mutex_lock( headtracker->mutex_track_state);
			state = headtracker->track_state;
			g_mutex_unlock(headtracker->mutex_track_state);
		}
		MILLISEC_SLEEP(timetosleep);
	}
}

enum headtracker_return ht_add_log_msg(HEAD_TRACKER* headtracker,char* msg){
	_DEBUG_("ht_add_log_msg: %s\n",msg);
	g_mutex_lock(headtracker->mutex_log_file);
	if(headtracker->log_file!=NULL){
		unsigned long long time = lg_get_time();
		fprintf(headtracker->log_file,"M\t%llu\t%s\n",time,msg);
	}
	g_mutex_unlock(headtracker->mutex_log_file);
	_DEBUG_("ht_add_log_msg: end\n");
	return LG_HEADTRACKER_OK;
}


enum headtracker_return ht_open_log_file(HEAD_TRACKER* headtracker, char* log_filename,char* path){
	_DEBUG_("ht_open_log_file: %s%s\n",path,log_filename);
	g_mutex_lock( headtracker->mutex_log_file);
	headtracker->log_file = NULL;

	if (log_filename != NULL){
		char tmp[256];
		sprintf(tmp,"%s%s",path,log_filename);
		headtracker->log_file = fopen(tmp,"w");
	}

	if(headtracker->log_file == NULL){
		g_mutex_unlock( headtracker->mutex_log_file);
		return LG_HEADTRACKER_ERROR;
	}
	g_mutex_unlock( headtracker->mutex_log_file);
	return LG_HEADTRACKER_OK;
}


enum headtracker_return ht_close_log_file(HEAD_TRACKER* headtracker){
	_DEBUG_("ht_close_log_file\n");
	g_mutex_lock( headtracker->mutex_log_file);

	if (headtracker->log_file != NULL){
		fprintf(headtracker->log_file,"\n");
		fclose(headtracker->log_file);
		headtracker->log_file = NULL;
	}
	g_mutex_unlock( headtracker->mutex_log_file);

	return LG_HEADTRACKER_OK;
}

enum headtracker_return ht_start_logging(HEAD_TRACKER* headtracker){
	_DEBUG_("ht_start_logging\n");
	g_mutex_lock( headtracker->mutex_log_state);
	headtracker->log_state = LOG_ENABLED;
	g_mutex_unlock( headtracker->mutex_log_state);
	return LG_HEADTRACKER_OK;
}


enum headtracker_return ht_stop_logging(HEAD_TRACKER* headtracker){
	_DEBUG_("ht_stop_logging\n");
	g_mutex_lock( headtracker->mutex_log_state);
	headtracker->log_state = LOG_DISABLED;
	g_mutex_unlock( headtracker->mutex_log_state);
	return LG_HEADTRACKER_OK;
}



