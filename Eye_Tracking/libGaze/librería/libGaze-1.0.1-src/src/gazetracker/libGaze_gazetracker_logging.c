#include <stdio.h>
#include <stdlib.h>
#include <glib/gthread.h>

#include <libGaze.h>
#include <libGaze_types.h>
#include <libGaze_error.h>
#include <libGaze_time.h>
#include <gazetracker/libGaze_gazetracker_logging.h>

void gazetracker_logging_main(GAZE_TRACKER *gazetracker){

	int count =0;

	EYE_TRACKER * eyetracker = gt_get_eyetracker(gazetracker);
	HEAD_TRACKER * headtracker = gt_get_headtracker(gazetracker);
	int timetosleep=1;

	if	(headtracker==NULL || eyetracker==NULL)
	{
		return;
	}

	int hfrequency=0;
	ht_get_tracker_frequency(headtracker,&hfrequency);

	int efrequency=0;
	et_get_tracker_frequency(eyetracker,&efrequency);

	if (efrequency>hfrequency){
		timetosleep = (int)(1000.0/(1.5*(double)efrequency));
	}else{
		timetosleep = (int)(1000.0/(1.5*(double)(hfrequency)));
	}

	_DEBUG_("timetosleep: %d\n",timetosleep);
	int state;
	g_mutex_lock(gazetracker->mutex_log_state);

	if(gazetracker->log_state == LOG_ENABLED){
		//fprintf(log_file,"MSG: let the logging begin\n");
		fprintf(gazetracker->log_file,"M\tlocaltime(milisec)\ttrackertime\tavailableeye\tlefteye_x\tlefteye_y\tlefteye_size\trighteye_x\trighteye_y\trighteye_size\n");
	}
	g_mutex_unlock(gazetracker->mutex_log_state);

	g_mutex_lock( gazetracker->mutex_track_state);
	state = gazetracker->track_state;
	g_mutex_unlock( gazetracker->mutex_track_state);

	unsigned long long lot = lg_get_time();
	_DEBUG_("time: %llu\n",lot);

	//enum eyetracker_return ieyereturn;
	t_raw_eye_data edata;
	t_raw_head_data hdata;

	int iehreturn;
	while(state != TRACKING_DISABLED){
		unsigned long long lt = lg_get_time();
		//getting eye tracker data

		et_get_eyetracking_data(eyetracker,&edata);
		ht_get_tracking_data(headtracker,headtracker->ihead,&hdata);

		g_mutex_lock(gazetracker->mutex_tracking_data);

		iehreturn = gt_get_current_gaze_position_from_raw_eye_head_data(gazetracker,&edata,&hdata,gazetracker->tracking_data);

		//iehreturn = LG_gt_NEW_GAZE_DATA;
		gazetracker->tracking_data_state = iehreturn;
		g_mutex_unlock(gazetracker->mutex_tracking_data);

		g_mutex_lock(gazetracker->mutex_log_state);

		if(gazetracker->log_state == LOG_ENABLED && iehreturn == LG_GT_NEW_GAZE_DATA){
		//if(eyetracker->log_state == LOG_ENABLED){

			g_mutex_lock(gazetracker->mutex_log_file);
			if(gazetracker->log_file != NULL){
				/*
				_DEBUG_("%s: T-X: %f\tT-Y: %f\tT-Z: %f\n",body->name,vicon_tracking_data.tx,vicon_tracking_data.ty,vicon_tracking_data.tz);
				_DEBUG_("%s: A-X: %f\tA-Y: %f\tA-Z: %f\n",body->name,vicon_tracking_data.qx,vicon_tracking_data.qy,vicon_tracking_data.qz);
				_DEBUG_("%s: ba-X: %f\tba-Y: %f\tba-Z: %f\n",body->name,vicon_tracking_data.bx,vicon_tracking_data.by,vicon_tracking_data.bz);

				*/
				//_DEBUG_("%lu:\t%lu\t%f,\t%f,\t%f,\t%f,\t%f,\t%f\n",(unsigned long )lt,(unsigned long )eyetracker->tracking_data->time,eyetracker->tracking_data->pupil_x[0],eyetracker->tracking_data->pupil_y[0],eyetracker->tracking_data->pupil_size[0],eyetracker->tracking_data->pupil_x[1],eyetracker->tracking_data->pupil_y[1],eyetracker->tracking_data->pupil_size[1]);

				t_gaze_data_set* gds;
				gds = gazetracker->tracking_data;
				fprintf(gazetracker->log_file,"G\t%lu\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%lu\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%lu\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n"
											,(unsigned long)gds->time,gds->gaze_available,
											gds->gaze[0].origin[0],gds->gaze[0].origin[1],gds->gaze[0].origin[2],gds->gaze[0].direction[0],gds->gaze[0].direction[1],gds->gaze[0].direction[2],
											gds->gaze[1].origin[0],gds->gaze[1].origin[1],gds->gaze[1].origin[2],gds->gaze[1].direction[0],gds->gaze[1].direction[1],gds->gaze[1].direction[2],
											(unsigned long)gds->eye.time,gds->eye.eye_available,gds->eye.alpha[0],gds->eye.beta[0],gds->eye.size[0],gds->eye.alpha[1],gds->eye.beta[1],gds->eye.size[1],
											(unsigned long)gds->head.time,gds->head.position[0],gds->head.position[1],gds->head.position[2],gds->head.orientation[0],gds->head.orientation[1],gds->head.orientation[2],gds->head.orientation[3],gds->head.orientation[4],gds->head.orientation[5],gds->head.orientation[6],gds->head.orientation[7],gds->head.orientation[8]);


			}
			g_mutex_unlock(gazetracker->mutex_log_file);
			//printf("sec: %ld\tmsec: %ld\n",(t.tv_sec - log_base_time->tv_sec),(long)((t.tv_usec - log_base_time->tv_usec)/1000));

		}

		g_mutex_unlock(gazetracker->mutex_log_state);

		g_mutex_lock( gazetracker->mutex_track_state);
		state = gazetracker->track_state;
		g_mutex_unlock(gazetracker->mutex_track_state);
		MILLISEC_SLEEP(timetosleep);
	}
}
