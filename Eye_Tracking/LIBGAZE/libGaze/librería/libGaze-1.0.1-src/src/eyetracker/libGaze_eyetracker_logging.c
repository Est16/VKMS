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
#include <eyetracker/libGaze_eyetracker_logging.h>


void eyetracker_logging_main(EYE_TRACKER *eyetracker){

	int count =0;
	int efrequency;
	et_get_tracker_frequency(eyetracker,&efrequency);
	int timetosleep = (int)(1000.0/(1.5*(double)efrequency));
	//int timetosleep = (int)(1000.0/(1.5*(double)(500)));
	_DEBUG_("timetosleep: %d\n",timetosleep);
	int state;
	g_mutex_lock(eyetracker->mutex_log_state);

	if(eyetracker->log_state == LOG_ENABLED){
		//fprintf(log_file,"MSG: let the logging begin\n");
		fprintf(eyetracker->log_file,"M\tlocaltime(milisec)\ttrackertime\tavailableeye\tlefteye_x\tlefteye_y\tlefteye_size\trighteye_x\trighteye_y\trighteye_size\n");
	}
	g_mutex_unlock(eyetracker->mutex_log_state);

	g_mutex_lock( eyetracker->mutex_track_state);
	state = eyetracker->track_state;
	g_mutex_unlock( eyetracker->mutex_track_state);

	/*struct timeval t;
 	gettimeofday(&t,0);

	unsigned long long lot = ((t.tv_sec - log_base_time.tv_sec)*1000000 + (t.tv_usec - log_base_time.tv_usec));
	lot = (unsigned long long)(lot/1000) ;
	*/
	unsigned long long lot = lg_get_time();
	_DEBUG_("time: %llu\n",lot);

	enum eyetracker_return ireturn;

	while(state != TRACKING_DISABLED){

		/*
		gettimeofday(&t,0);
		unsigned long long lt = ((t.tv_sec - log_base_time.tv_sec)*1000000 + (t.tv_usec - log_base_time.tv_usec));
		lt = (unsigned long long)(lt/1000);
		*/
		unsigned long long lt = lg_get_time();
		g_mutex_lock(eyetracker->mutex_tracking_data);

		ireturn = (*(eyetracker->get_eyetracking_data))(eyetracker->tracking_data);
		count++;
		eyetracker->tracking_data_state = ireturn;
		g_mutex_unlock(eyetracker->mutex_tracking_data);

		g_mutex_lock(eyetracker->mutex_log_state);

		if(eyetracker->log_state == LOG_ENABLED && ireturn == LG_EYETRACKER_NEW_DATA){
		//if(eyetracker->log_state == LOG_ENABLED){

			g_mutex_lock(eyetracker->mutex_log_file);
			if(eyetracker->log_file != NULL){
				/*
				_DEBUG_("%s: T-X: %f\tT-Y: %f\tT-Z: %f\n",body->name,vicon_tracking_data.tx,vicon_tracking_data.ty,vicon_tracking_data.tz);
				_DEBUG_("%s: A-X: %f\tA-Y: %f\tA-Z: %f\n",body->name,vicon_tracking_data.qx,vicon_tracking_data.qy,vicon_tracking_data.qz);
				_DEBUG_("%s: ba-X: %f\tba-Y: %f\tba-Z: %f\n",body->name,vicon_tracking_data.bx,vicon_tracking_data.by,vicon_tracking_data.bz);

				*/
				//_DEBUG_("%lu:\t%lu\t%f,\t%f,\t%f,\t%f,\t%f,\t%f\n",(unsigned long )lt,(unsigned long )eyetracker->tracking_data->time,eyetracker->tracking_data->pupil_x[0],eyetracker->tracking_data->pupil_y[0],eyetracker->tracking_data->pupil_size[0],eyetracker->tracking_data->pupil_x[1],eyetracker->tracking_data->pupil_y[1],eyetracker->tracking_data->pupil_size[1]);

				fprintf(eyetracker->log_file,"E\t%lu\t%lu\t%d\t%f\t%f\t%f\t%f\t%f\t%f\n"
											,(unsigned long)lt,(unsigned long)eyetracker->tracking_data->time,eyetracker->tracking_data->eye_available,eyetracker->tracking_data->pupil_x[0],eyetracker->tracking_data->pupil_y[0],eyetracker->tracking_data->pupil_size[0],eyetracker->tracking_data->pupil_x[1],eyetracker->tracking_data->pupil_y[1],eyetracker->tracking_data->pupil_size[1]);


			}
			g_mutex_unlock(eyetracker->mutex_log_file);
			//printf("sec: %ld\tmsec: %ld\n",(t.tv_sec - log_base_time->tv_sec),(long)((t.tv_usec - log_base_time->tv_usec)/1000));




		}
		//sched_yield();
		g_mutex_unlock(eyetracker->mutex_log_state);


		g_mutex_lock( eyetracker->mutex_track_state);
		state = eyetracker->track_state;
		g_mutex_unlock(eyetracker->mutex_track_state);

		MILLISEC_SLEEP(timetosleep);
		//usleep(1000);

	}
	//free(eyetracker->tracking_data);
	_DEBUG_("count: %d\n",count);
}

enum eyetracker_return et_add_log_msg(EYE_TRACKER* eyetracker,char* msg){
	_DEBUG_("ht_add_log_msg: %s\n",msg);
	g_mutex_lock(eyetracker->mutex_log_file);
	if(eyetracker->log_file!=NULL){
		unsigned long long time = lg_get_time();
		fprintf(eyetracker->log_file,"M\t%llu\t%s\n",time,msg);
	}
	g_mutex_unlock(eyetracker->mutex_log_file);
	_DEBUG_("et_add_log_msg: end\n");

	return LG_EYETRACKER_OK;

}


enum eyetracker_return et_open_log_file(EYE_TRACKER* eyetracker, char* log_filename, char* path){
	_DEBUG_("et_open_log_file: %s %s\n",log_filename,path);
	g_mutex_lock( eyetracker->mutex_log_file);
	eyetracker->log_file = NULL;

	char edfa[256];
	sprintf(edfa,"%s%sa",path,log_filename);
	if (log_filename != NULL){
		eyetracker->log_file = fopen(edfa,"w");
	}

	if(eyetracker->log_file == NULL){

		g_mutex_unlock( eyetracker->mutex_log_file);
		return LG_EYETRACKER_ERROR;
	}
	g_mutex_unlock( eyetracker->mutex_log_file);

	return LG_EYETRACKER_OK;

}


enum eyetracker_return et_close_log_file(EYE_TRACKER* eyetracker){
	g_mutex_lock( eyetracker->mutex_log_file);

	if (eyetracker->log_file != NULL){
		fprintf(eyetracker->log_file,"\n");
		fclose(eyetracker->log_file);
		eyetracker->log_file = NULL;
	}
	g_mutex_unlock( eyetracker->mutex_log_file);

	return LG_EYETRACKER_OK;

}

enum eyetracker_return et_start_logging(EYE_TRACKER* eyetracker){
	_DEBUG_("start_logging\n");
	g_mutex_lock( eyetracker->mutex_log_state);
	eyetracker->log_state = LOG_ENABLED;
	g_mutex_unlock( eyetracker->mutex_log_state);
	//return	(*(eyetracker->start_logging))();
	return LG_EYETRACKER_OK;

}


enum eyetracker_return et_stop_logging(EYE_TRACKER* eyetracker){
		g_mutex_lock( eyetracker->mutex_log_state);
		eyetracker->log_state = LOG_DISABLED;
		g_mutex_unlock( eyetracker->mutex_log_state);
		//eturn	(*(eyetracker->stop_logging))();
		return LG_EYETRACKER_OK;


}



