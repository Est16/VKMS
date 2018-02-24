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
#include <string.h>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>

#include <libGaze.h>
#include <libGaze_error.h>
#include <libGaze_defines.h>
#include <gazetracker/libGaze_gazetracker.h>
#include <gazetracker/libGaze_gazetracker_logging.h>
#include <gazetracker/libGaze_gazetracker_types.h>
#include <gazetracker/libGaze_gazetracker_defines.h>
#include <eyetracker/libGaze_eyetracker.h>
#include <headtracker/libGaze_headtracker.h>
#include <display/libGaze_display.h>
#include <tools/libGaze_tools.h>

void gt_update_vhooks(GAZE_TRACKER* eyeheadtracker, t_visualisation_hooks* hooks)
{
	eyeheadtracker->visualhooks=hooks;
}

/*
GAZE_TRACKER* gt_create_eyeheadtracker_pointer(void)
{
	GAZE_TRACKER* eyeheadtracker=(GAZE_TRACKER*)calloc(1,sizeof(GAZE_TRACKER));
	return eyeheadtracker;
}
*/

GAZE_TRACKER* gt_create_eyeheadtracker(int debug, int error){
	GAZE_TRACKER* gazetracker=(GAZE_TRACKER*)calloc(1,sizeof(GAZE_TRACKER));
	gazetracker->debug = debug;
	gazetracker->error = error;

	if (!g_thread_supported()) g_thread_init(NULL);

	gazetracker->mutex_log_state = g_mutex_new();
	gazetracker->mutex_log_file = g_mutex_new();
	gazetracker->mutex_tracking_data = g_mutex_new();
	gazetracker->mutex_log_thread = g_mutex_new();
	gazetracker->mutex_track_state = g_mutex_new();
	gazetracker->log_thread = NULL;

	gazetracker->log_state=LOG_DISABLED;
	gazetracker->track_state=TRACKING_DISABLED;
	gazetracker->log_file = NULL;
	gazetracker->tracking_data = NULL;

	return gazetracker;
}

int gt_load_modules(GAZE_TRACKER* gazetracker,char* eyemod, char* headmod,enum gt_modes mode){
	_DEBUG_("gt_load_modules\n");
	gazetracker->mode = mode;
	int lgreturn = LG_OK;

	//printf("%d\t%d\n",gazetracker->debug,gazetracker->error);

	if (mode == GT_MODE_BOTH || mode == GT_MODE_EYE){
		gazetracker->eyetracker = et_load_module(eyemod,gazetracker->debug,gazetracker->error,EYETRACKER_MODE_NORMAL);
		if(gazetracker->eyetracker == NULL){
			_ERROR_("gt_load_modules: could not load eyetracker\n");
			lgreturn = LG_ERROR;
		}
	}else if (mode == GT_MODE_DUMMY || mode == GT_MODE_HEAD){
		gazetracker->eyetracker = et_load_module(eyemod,gazetracker->debug,gazetracker->error,EYETRACKER_MODE_DUMMY);
		if(gazetracker->eyetracker == NULL){
			_ERROR_("gt_load_modules: could not load eyetracker\n");
			lgreturn =  LG_ERROR;
		}
	}
	if (mode == GT_MODE_BOTH || mode == GT_MODE_HEAD){
		gazetracker->headtracker = ht_load_module(headmod,gazetracker->debug,gazetracker->error,HEADTRACKER_MODE_NORMAL);
		if(gazetracker->headtracker == NULL){
			_ERROR_("gt_load_modules: could not load headtracker\n");
			lgreturn = LG_ERROR;
		}
	}else if(mode == GT_MODE_DUMMY || mode == GT_MODE_EYE){
		gazetracker->headtracker = ht_load_module(headmod,gazetracker->debug,gazetracker->error,HEADTRACKER_MODE_DUMMY);
		if(gazetracker->headtracker == NULL){
			_ERROR_("gt_load_modules: could not load headtracker\n");
			lgreturn = LG_ERROR;
		}
	}
/*
	gazetracker->display = d_load_module(dismod,gazetracker->debug,gazetracker->error);
	if(gazetracker->display == NULL){
		_ERROR_("gt_load_modules: could not load display\n");
		lgreturn = LG_ERROR;
	}
*/
	gazetracker->used_eye = LG_EYE_NONE;

	gazetracker->hcorrection.correct_x = 0.0;
	gazetracker->hcorrection.correct_y = 0.0;
	gazetracker->hcorrection.correct_z = 0.0;

	gazetracker->hcorrection.correct_matrix[0] = 1.0;
	gazetracker->hcorrection.correct_matrix[1] = 0.0;
	gazetracker->hcorrection.correct_matrix[2] = 0.0;

	gazetracker->hcorrection.correct_matrix[3] = 0.0;
	gazetracker->hcorrection.correct_matrix[4] = 1.0;
	gazetracker->hcorrection.correct_matrix[5] = 0.0;

	gazetracker->hcorrection.correct_matrix[6] = 0.0;
	gazetracker->hcorrection.correct_matrix[7] = 0.0;
	gazetracker->hcorrection.correct_matrix[8] = 1.0;

	gazetracker->hcorrection.headtonasalbone[0] = 0.0;
	gazetracker->hcorrection.headtonasalbone[1] = 0.0;
	gazetracker->hcorrection.headtonasalbone[2] = 0.0;

	gazetracker->hcorrection.headtoeye[0][0] = 0.0;
	gazetracker->hcorrection.headtoeye[0][1] = 0.0;
	gazetracker->hcorrection.headtoeye[0][2] = 0.0;

	gazetracker->hcorrection.headtoeye[1][0] = 0.0;
	gazetracker->hcorrection.headtoeye[1][1] = 0.0;
	gazetracker->hcorrection.headtoeye[1][2] = 0.0;

	return lgreturn;
}

int gt_load_calibration_module(GAZE_TRACKER* gazetracker, char* cmod){
 	_DEBUG_("gt_load_calibration_module\n");
	gazetracker->calibration = c_load_module(cmod,gazetracker->debug,gazetracker->error);
	if(gazetracker->calibration == NULL){
		_ERROR_("gt_load_calibration_module: could not load calibration\n");
		return LG_ERROR;
	}
	return LG_OK;
}

int gt_configure(GAZE_TRACKER* gazetracker, char* eyeconf,char* headconf){
	_DEBUG_("gt_configure\n");
	int tmp;
	int lgreturn = LG_OK;
	//if (gazetracker->mode == GT_MODE_BOTH || gazetracker->mode == GT_MODE_EYE || gazetracker->mode == GT_MODE_DUMMY){
		tmp = et_configure_eye_tracker(gazetracker->eyetracker,eyeconf);
		if(tmp != LG_EYETRACKER_OK){
			_ERROR_("gt_configure: could not configure eyetracker\n");
			lgreturn =  tmp;
		}
	//}
	//if (gazetracker->mode == GT_MODE_BOTH || gazetracker->mode == GT_MODE_HEAD || gazetracker->mode == GT_MODE_DUMMY){
		tmp = ht_configure_head_tracker(gazetracker->headtracker,headconf);
		if(tmp != LG_HEADTRACKER_OK){
			_ERROR_("gt_configure: could not configure headtracker\n");
			lgreturn = tmp;
		}
	//}
/*
	tmp = d_configure(gazetracker->display,disconf);
	if(tmp != 0){
		_ERROR_("gt_configure: could not configure display\n");
		lgreturn =  tmp;
	}
*/
	return lgreturn;
}

int gt_connect(GAZE_TRACKER* gazetracker){
	_DEBUG_("gt_connect\n");
	int tmp;
	int lgreturn = LG_OK;
	//if (gazetracker->mode == GT_MODE_BOTH || gazetracker->mode == GT_MODE_EYE){
		tmp = et_connect_to_eye_tracker(gazetracker->eyetracker);
		if(tmp != LG_EYETRACKER_OK){
			_ERROR_("gt_connect: could not connect to eyetracker\n");
			lgreturn =  tmp;
		}
	//}
	//if (gazetracker->mode == GT_MODE_BOTH || gazetracker->mode == GT_MODE_HEAD){
		tmp = ht_connect_to_head_tracker(gazetracker->headtracker);
		if(tmp != LG_HEADTRACKER_OK){
			_ERROR_("gt_connect: could not connect to headtracker\n");
			lgreturn =  tmp;
		}
	//}
	return lgreturn;
}

int gt_disconnect(GAZE_TRACKER* gazetracker){
	_DEBUG_("gt_disconnect\n");
	int tmp;
	int lgreturn = LG_OK;
	if (gazetracker!=NULL)
	{
		//if (gazetracker->mode == GT_MODE_BOTH || gazetracker->mode == GT_MODE_EYE){
			if (gazetracker->eyetracker!=NULL)
			{
				tmp = et_disconnect_eye_tracker(gazetracker->eyetracker);
				if(tmp != LG_EYETRACKER_OK){
					_ERROR_("gt_disconnect: could not disconnect eyetracker\n");
					lgreturn =  tmp;
				}
			}
		//}
		//if (gazetracker->mode == GT_MODE_BOTH || gazetracker->mode == GT_MODE_HEAD){
			if (gazetracker->headtracker!=NULL)
			{
				tmp = ht_disconnect_head_tracker(gazetracker->headtracker);
				if(tmp != LG_HEADTRACKER_OK){
					_ERROR_("gt_disconnect: could not disconnect headtracker\n");
					lgreturn =  tmp;
				}
			}
		//}
	}
	return lgreturn;
}

int gt_start_tracking(GAZE_TRACKER* gazetracker){
	_DEBUG_("gt_start_tracking\n");
	int tmp;
	int lgreturn = LG_OK;
	//if (gazetracker->mode == GT_MODE_BOTH || gazetracker->mode == GT_MODE_EYE){
		tmp = et_start_eye_tracking(gazetracker->eyetracker);
		if(tmp != LG_EYETRACKER_OK){
			_ERROR_("gt_start_tracking: could not start eyetracking\n");
			lgreturn =  tmp;
		}
	//}
	//if (gazetracker->mode == GT_MODE_BOTH || gazetracker->mode == GT_MODE_HEAD){
		tmp = ht_start_head_tracking(gazetracker->headtracker);
		if(tmp != LG_HEADTRACKER_OK){
			_ERROR_("gt_start_tracking: could not start headtracking\n");
			lgreturn =  tmp;
		}
	//}
	g_mutex_lock( gazetracker->mutex_track_state);
	gazetracker->track_state = TRACKING_ENABLED;
	g_mutex_unlock( gazetracker->mutex_track_state);
	free(gazetracker->tracking_data);
	gazetracker->tracking_data = (t_gaze_data_set*)calloc(1,sizeof(t_gaze_data_set));
	gazetracker->log_thread =g_thread_create((GThreadFunc)gazetracker_logging_main,(gpointer)gazetracker,1,(GError**)&gazetracker->thread_error);
	g_thread_set_priority(gazetracker->log_thread,G_THREAD_PRIORITY_NORMAL);

	return lgreturn;
}

int gt_stop_tracking(GAZE_TRACKER* gazetracker){
	_DEBUG_("gt_stop_tracking\n");
	int tmp;
	int lgreturn = LG_OK;

	g_mutex_lock( gazetracker->mutex_track_state);
	gazetracker->track_state = TRACKING_DISABLED;
	g_mutex_unlock( gazetracker->mutex_track_state);
	g_thread_join(gazetracker->log_thread);


	//if (gazetracker->mode == GT_MODE_BOTH || gazetracker->mode == GT_MODE_EYE){
		tmp = et_stop_eye_tracking(gazetracker->eyetracker);
		if(tmp != LG_EYETRACKER_OK){
			_ERROR_("gt_stop_tracking: could not stop eyetracking\n");
			lgreturn =  tmp;
		}
	//}
	//if (gazetracker->mode == GT_MODE_BOTH || gazetracker->mode == GT_MODE_HEAD){
	tmp = ht_stop_head_tracking(gazetracker->headtracker);
		if(tmp != LG_HEADTRACKER_OK){
			_ERROR_("gt_stop_tracking: could not stop headtracking\n");
			lgreturn =  tmp;
		}
	//}
	return lgreturn;
}

int gt_start_logging(GAZE_TRACKER* gazetracker){
	int tmp;
	int lgreturn = LG_OK;

	if(gazetracker->log_mode & GAZE_LOG_FILE){

		g_mutex_lock( gazetracker->mutex_log_state);
		gazetracker->log_state = LOG_ENABLED;
		g_mutex_unlock( gazetracker->mutex_log_state);

	}
	if(gazetracker->log_mode & EYE_LOG_FILE){
		tmp = et_start_logging(gazetracker->eyetracker);
		if(tmp != LG_EYETRACKER_OK){
			_ERROR_("gt_start_logging: could not start logging on eyetracker\n");
			lgreturn =  tmp;
		}
	}
	if(gazetracker->log_mode & HEAD_LOG_FILE){
		tmp = ht_start_logging(gazetracker->headtracker);
		if(tmp != LG_HEADTRACKER_OK){
			_ERROR_("gt_start_logging: could not start logging on headtracker\n");
			lgreturn =  tmp;
		}
	}
	return lgreturn;
}

int gt_stop_logging(GAZE_TRACKER* gazetracker){
	int tmp;
	int lgreturn = LG_OK;
	if(gazetracker->log_mode & GAZE_LOG_FILE){
		g_mutex_lock( gazetracker->mutex_log_state);
		gazetracker->log_state = LOG_DISABLED;
		g_mutex_unlock( gazetracker->mutex_log_state);
	}
	if(gazetracker->log_mode & EYE_LOG_FILE){
		tmp = et_stop_logging(gazetracker->eyetracker);
		if(tmp != LG_EYETRACKER_OK){
			_ERROR_("gt_stop_logging: could not stop logging on eyetracker\n");
			lgreturn =  tmp;
		}
	}
	if(gazetracker->log_mode & HEAD_LOG_FILE){
		tmp = ht_stop_logging(gazetracker->headtracker);
		if(tmp != LG_HEADTRACKER_OK){
			_ERROR_("gt_stop_logging: could not stop logging on headtracker\n");
			lgreturn =  tmp;
		}
	}
	return LG_OK;
}

int gt_open_log_file(GAZE_TRACKER* gazetracker,char* filetag, char* path, int flag){
	int tmp;
	int lgreturn = LG_OK;

	gazetracker->log_mode = flag;

	if (flag & GAZE_LOG_FILE){
		g_mutex_lock( gazetracker->mutex_log_file);
		gazetracker->log_file = NULL;

		char gdfa[256];
		sprintf(gdfa,"%s%s.gdf",path,filetag);
		if (gdfa != NULL){
			gazetracker->log_file = fopen(gdfa,"w");
		}
		g_mutex_unlock( gazetracker->mutex_log_file);

		if(gazetracker->log_file == NULL){
			return LG_ERROR;
		}
	}
	if(EYE_LOG_FILE& flag){
		char edf[256];
		sprintf(edf,"%s.edf",filetag);
		tmp = et_open_log_file(gazetracker->eyetracker,edf,path);
		if(tmp != LG_EYETRACKER_OK){
			_ERROR_("gt_open_log_file: could not open log file on eyetracker\n");
			lgreturn = tmp;
		}
	}
	if(HEAD_LOG_FILE&flag){
		char hdf[256];
		sprintf(hdf,"%s.hdf",filetag);
		tmp = ht_open_log_file(gazetracker->headtracker,hdf,path);
		if(tmp != LG_HEADTRACKER_OK){
			_ERROR_("gt_open_log_file: could not open log file on headtracker\n");
			lgreturn =  tmp;
		}
	}

	if(CALIB_LOG_FILE & flag){
		char calib[256];
		sprintf(calib,"%s%s.calib",path,filetag);
		gazetracker->calibration_log = fopen(calib,"w");
		if (gazetracker->calibration_log ==NULL){
			_ERROR_("gt_open_log_file: could not open calibration log file\n");
			//return LG_ERROR;
		}
	}
	return lgreturn;

}

int gt_close_log_file(GAZE_TRACKER* gazetracker){
	int tmp;
	int lgreturn = LG_OK;
	if(GAZE_LOG_FILE & gazetracker->log_mode){
		g_mutex_lock( gazetracker->mutex_log_file);

		if (gazetracker->log_file != NULL){
			fprintf(gazetracker->log_file,"\n");
			fclose(gazetracker->log_file);
			gazetracker->log_file = NULL;
		}
		g_mutex_unlock( gazetracker->mutex_log_file);
	}

	if(EYE_LOG_FILE & gazetracker->log_mode){
		tmp = et_close_log_file(gazetracker->eyetracker);
		if(tmp != LG_EYETRACKER_OK){
			_ERROR_("gt_close_log_file: could not close logfile on eyetracker\n");
			lgreturn =  tmp;
		}
	}
	if(HEAD_LOG_FILE & gazetracker->log_mode){
		tmp = ht_close_log_file(gazetracker->headtracker);
		if(tmp != LG_HEADTRACKER_OK){
			_ERROR_("gt_close_log_file: could not close logfile on headtracker\n");
			lgreturn = tmp;
		}
	}
	if(EVENT_LOG_FILE & gazetracker->log_mode){
		fclose(gazetracker->event_log);
	}
	if(CALIB_LOG_FILE & gazetracker->log_mode){
		fclose(gazetracker->calibration_log);
	}
	return lgreturn;
}

int gt_add_log_msg(GAZE_TRACKER* gazetracker,char* msg){
	int tmp;
	int lgreturn = LG_OK;
	_DEBUG_("gt_add_log_msg: %s\n",msg);
	if(GAZE_LOG_FILE & gazetracker->log_mode){
		g_mutex_lock(gazetracker->mutex_log_file);
		if(gazetracker->log_file!=NULL){
			unsigned long long time = lg_get_time();
			fprintf(gazetracker->log_file,"MSG\t%llu\t%s\n",time,msg);
		}
		g_mutex_unlock(gazetracker->mutex_log_file);
	}
	if(EYE_LOG_FILE & gazetracker->log_mode){
		tmp = et_add_log_msg(gazetracker->eyetracker,msg);
		if(tmp != LG_EYETRACKER_OK){
			_ERROR_("gt_add_log_msg: could not add log msg on eyetracker\n");
			lgreturn = tmp;
		}
	}
	if(HEAD_LOG_FILE & gazetracker->log_mode){
		tmp = ht_add_log_msg(gazetracker->headtracker,msg);
		if(tmp != LG_HEADTRACKER_OK){
			_ERROR_("gt_add_log_msg: could not add log msg on headtracker\n");
			lgreturn = tmp;
		}
	}
	return lgreturn;
}

int gt_set_visual_hooks(GAZE_TRACKER* gazetracker,t_visualisation_hooks* vhooks){
	if (gazetracker->visualhooks != NULL){
		//free(gazetracker->visualhooks);
	}
	gazetracker->visualhooks = vhooks;
	return LG_OK;
}

int gt_calculate_head_relative_position(double* headpos, double* headorient, double* position_correction, double* orientation_correction, double *eye_position, double *eye_orientation){
	_DEBUG_("correct_head_data\n");
	gsl_vector_view hpos = gsl_vector_view_array(headpos,3);
	gsl_matrix_view hmatrix = gsl_matrix_view_array(headorient,3,3);

	gsl_vector_view epos = gsl_vector_view_array(eye_position,3);
	gsl_matrix_view ematrix = gsl_matrix_view_array(eye_orientation,3,3);

	gsl_vector_memcpy(&epos.vector,&hpos.vector);

	_DEBUG_("correct_head_data: correcting the head position\n");
	// correcting the head position
	// calculating the vector which need to be added to the current head position
	gsl_vector_view ehrelation = gsl_vector_view_array(position_correction,3);
	gsl_vector *tmp_vector = gsl_vector_calloc(3);
	gsl_matrix *tmp_matrix = gsl_matrix_calloc(3,3);
	gsl_matrix_memcpy(tmp_matrix,&hmatrix.matrix);

	gsl_blas_dgemv(CblasNoTrans,1.0,tmp_matrix,&ehrelation.vector,1.0,tmp_vector);
	_DEBUG_("correct_head_data: adding the correction vector to the current head position\n");
	// adding the correction vector to the current head position

	gsl_vector_add(&epos.vector,tmp_vector);
	gsl_matrix_view cmatrix = gsl_matrix_view_array(orientation_correction,3,3);
	gsl_matrix* tmp_matrix2 = gsl_matrix_calloc(3,3);
	gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,tmp_matrix,&cmatrix.matrix,1.0,tmp_matrix2);


	gsl_matrix_memcpy(&ematrix.matrix,tmp_matrix2);

	gsl_vector_free(tmp_vector);
	gsl_matrix_free(tmp_matrix);
	gsl_matrix_free(tmp_matrix2);
	return 1;
}

int gt_get_current_gaze_position(GAZE_TRACKER* gazetracker,t_gaze_data_set* gds){
	int ireturn;
	g_mutex_lock(gazetracker->mutex_tracking_data);
	ireturn = gazetracker->tracking_data_state;
	memcpy(gds,gazetracker->tracking_data, sizeof(t_gaze_data_set));
	g_mutex_unlock(gazetracker->mutex_tracking_data);
	return ireturn;

}


int gt_get_current_gaze_position_from_raw_eye_head_data(GAZE_TRACKER* gazetracker, t_raw_eye_data *red, t_raw_head_data *rhd,t_gaze_data_set* gds)
{
	int ireturn = LG_ERROR;
	if (gazetracker->is_calibrated == 1){

		t_head_data hd;
		_DEBUG_("caluclating the position of the nasal bone relative to the raw head tracking data\n");
		//caluclating the position of the nasal bone relative to the raw head tracking data
		gt_calculate_head_relative_position(rhd->position,rhd->matrix,gazetracker->hcorrection.headtonasalbone,gazetracker->hcorrection.correct_matrix,hd.position,hd.orientation);

		_DEBUG_("c_procesEyeData\n");
		t_eye_data ed;
		c_processEyeData(gazetracker->calibration,red,&ed);
		ireturn = gt_get_current_gaze_position_from_eye_head_data(gazetracker,&ed,&hd,gds);
		return ireturn;
	}else{
		return LG_GT_NOT_CALIBRATED;
	}
}

int gt_get_current_gaze_position_from_eye_head_data(GAZE_TRACKER* gazetracker, t_eye_data *ed, t_head_data *hd,t_gaze_data_set* gds)
{

	int ireturn = LG_ERROR;
	if (gazetracker->is_calibrated == 1){

		gds->time = lg_get_time();
		memcpy(&gds->head,hd,sizeof(t_head_data));
		memcpy(&gds->eye,ed,sizeof(t_eye_data));

		gds->gaze_available = ed->eye_available;

		gt_calculate_head_relative_position(hd->position,hd->orientation,gazetracker->hcorrection.headtoeye[0],gazetracker->hcorrection.correct_matrix,gds->gaze[0].origin,gds->gaze[0].matrix);

		gsl_vector* gaze_direction_left = NULL;
		gaze_direction_left = gsl_vector_calloc(3);
		gsl_vector* viewing_direction_left = lg_tools_get3DVectorFromAngles(lg_tools_radians(gds->eye.alpha[0]),lg_tools_radians(gds->eye.beta[0]));
		_DEBUG_("viewing_direction_left: %f\t%f\t%f\n",gsl_vector_get(viewing_direction_left,0),gsl_vector_get(viewing_direction_left,1),gsl_vector_get(viewing_direction_left,2));
		gsl_matrix_view left_hm_tmp =gsl_matrix_view_array(gds->head.orientation,3,3);
		gsl_matrix *left_hm = gsl_matrix_calloc(3,3);
		gsl_matrix_memcpy(left_hm,&left_hm_tmp.matrix);
		gsl_blas_dgemv(CblasNoTrans,1.0,left_hm,viewing_direction_left,1.0,gaze_direction_left);
		gds->gaze[0].direction[0] = gsl_vector_get(gaze_direction_left,0);
		gds->gaze[0].direction[1] = gsl_vector_get(gaze_direction_left,1);
		gds->gaze[0].direction[2] = gsl_vector_get(gaze_direction_left,2);

		_DEBUG_("gds->gaze[0].direction: %f\t%f\t%f\n",gds->gaze[0].direction[0],gds->gaze[0].direction[1],gds->gaze[0].direction[2]);

		gt_calculate_head_relative_position(hd->position,hd->orientation,gazetracker->hcorrection.headtoeye[1],gazetracker->hcorrection.correct_matrix,gds->gaze[1].origin,gds->gaze[1].matrix);

		gsl_vector* gaze_direction_right = NULL;
		gaze_direction_right = gsl_vector_calloc(3);
		gsl_vector* viewing_direction_right = lg_tools_get3DVectorFromAngles(lg_tools_radians(gds->eye.alpha[1]),lg_tools_radians(gds->eye.beta[1]));
		_DEBUG_("viewing_direction_right: %f\t%f\t%f\n",gsl_vector_get(viewing_direction_right,0),gsl_vector_get(viewing_direction_right,1),gsl_vector_get(viewing_direction_right,2));

		gsl_matrix_view right_hm_tmp =gsl_matrix_view_array(gds->head.orientation,3,3);
		gsl_matrix *right_hm = gsl_matrix_calloc(3,3);
		gsl_matrix_memcpy(right_hm,&right_hm_tmp.matrix);
		gsl_blas_dgemv(CblasNoTrans,1.0,right_hm,viewing_direction_right,1.0,gaze_direction_right);
		gds->gaze[1].direction[0] = gsl_vector_get(gaze_direction_right,0);
		gds->gaze[1].direction[1] = gsl_vector_get(gaze_direction_right,1);
		gds->gaze[1].direction[2] = gsl_vector_get(gaze_direction_right,2);


		gsl_vector_free(gaze_direction_left);
		gsl_vector_free(viewing_direction_left);
		gsl_matrix_free(left_hm);
		gsl_vector_free(gaze_direction_right);
		gsl_vector_free(viewing_direction_right);
		gsl_matrix_free(right_hm);

		ireturn = LG_GT_NEW_GAZE_DATA;
		return ireturn;
	}else{
		return LG_GT_NOT_CALIBRATED;
	}
}


int gt_get_gaze_with_alpha_beta(GAZE_TRACKER* gazetracker,double alpha, double beta, t_gaze_data_set *gds){
				//_DEBUG_("gt_get_gaze_with_alpha_beta: alpha= %f\tbeta= %f\n",alpha,beta);
				t_raw_head_data rhd;
				ht_get_tracking_data(gazetracker->headtracker,gazetracker->headtracker->ihead,&rhd);

				t_head_data hd;
				gt_calculate_head_relative_position(rhd.position,rhd.matrix,gazetracker->hcorrection.headtonasalbone,gazetracker->hcorrection.correct_matrix,hd.position,hd.orientation);


				t_eye_data ed;
				ed.alpha[0] = alpha;
				ed.beta[0] = beta;
				ed.size[0] = 0;

				ed.alpha[1] = alpha;
				ed.beta[1] = beta;
				ed.size[1] = 0;

				ed.eye_available = LG_EYE_BOTH;

				return gt_get_current_gaze_position_from_eye_head_data(gazetracker,&ed,&hd,gds);
}


HEAD_TRACKER* gt_get_headtracker(GAZE_TRACKER* gazetracker){
	if(gazetracker != NULL){
		return gazetracker->headtracker;
	}else{
		return NULL;
	}
}

EYE_TRACKER* gt_get_eyetracker(GAZE_TRACKER* gazetracker){
	if(gazetracker != NULL){
		return gazetracker->eyetracker;
	}else{
		return NULL;
	}
}


void gt_get_ray_relative_to_head_orientation(GAZE_TRACKER *gazetracker,double alpha, double beta, t_ray *ray){
	//printf("lg_get_fov_ray_from_angles\n");
	t_raw_head_data rhd;
	//printf("ht_get_tracking_data\n");
	ht_get_tracking_data(gazetracker->headtracker,gazetracker->headtracker->ihead,&rhd);
	//printf("gt_calculate_head_relative_position\n");
	t_head_data hd;
	gt_calculate_head_relative_position(rhd.position,rhd.matrix,gazetracker->hcorrection.headtonasalbone,gazetracker->hcorrection.correct_matrix,hd.position,hd.orientation);
	//printf("hm\n");
	ray->origin[0] = hd.position[0];
	ray->origin[1] = hd.position[1];
	ray->origin[2] = hd.position[2];

	gsl_vector* gaze_direction = NULL;
	gaze_direction= gsl_vector_calloc(3);
	//gsl_vector_view gaze_direction = gsl_vector_view_array(direction,3);
	gsl_vector* viewing_direction = lg_tools_get3DVectorFromAngles(lg_tools_radians(alpha),lg_tools_radians(beta));
	_DEBUG_("viewing_direction: %f\t%f\t%f\n",gsl_vector_get(viewing_direction,0),gsl_vector_get(viewing_direction,1),gsl_vector_get(viewing_direction,2));
	//printf("viewing_direction: %f\t%f\t%f\n",gsl_vector_get(viewing_direction,0),gsl_vector_get(viewing_direction,1),gsl_vector_get(viewing_direction,2));

	gsl_matrix_view left_hm_tmp =gsl_matrix_view_array(hd.orientation,3,3);
	gsl_matrix *left_hm = gsl_matrix_calloc(3,3);
	gsl_matrix_memcpy(left_hm,&left_hm_tmp.matrix);
	gsl_blas_dgemv(CblasNoTrans,1.0,left_hm,viewing_direction,1.0,gaze_direction);
	ray->direction[0] = gsl_vector_get(gaze_direction,0);
	ray->direction[1] = gsl_vector_get(gaze_direction,1);
	ray->direction[2] = gsl_vector_get(gaze_direction,2);
}
