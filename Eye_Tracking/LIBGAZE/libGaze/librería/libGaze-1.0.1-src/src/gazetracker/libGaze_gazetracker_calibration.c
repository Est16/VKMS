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

#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>

#include <libGaze.h>
#include <libGaze_defines.h>
#include <libGaze_time.h>
#include <libGaze_error.h>
#include <gazetracker/libGaze_gazetracker.h>
#include <gazetracker/libGaze_gazetracker_types.h>
#include <headtracker/libGaze_headtracker.h>
#include <calibration/libGaze_calibration.h>
#include <tools/libGaze_tools.h>
//#include <display/libGaze_display.h>


int gt_collect_eyehead_relation(GAZE_TRACKER* gazetracker){
	//print "collectEyeHeadRelation"
	int ireturn;
	ireturn = gt_show_instruction(gazetracker,INSTRUCTION_3,KEY_PRESS);
	if (ireturn != LG_QUIT){
		if(gazetracker->mode == GT_MODE_BOTH || gazetracker->mode == GT_MODE_HEAD){
			// getting position of the head object
			t_raw_head_data hd;
			ht_get_tracking_data(gazetracker->headtracker,gazetracker->headtracker->ihead,&hd);

			// getting position of eye pointer object
			t_raw_head_data ehd;
			ht_get_tracking_data(gazetracker->headtracker,gazetracker->headtracker->ieye,&ehd);

			gsl_vector_view head_vector = gsl_vector_view_array(hd.position,3);
			gsl_vector_view eye_vector = gsl_vector_view_array(ehd.position,3);

			// calculating the vector, which points from the head object to the eye object
			gsl_vector* tmp_vector;
			tmp_vector = gsl_vector_calloc(3);
			gsl_vector_memcpy(tmp_vector,&eye_vector.vector);
			gsl_vector_sub(tmp_vector,&head_vector.vector);

			// rotating the head->eye vector to a local coordinate system
			// by multiplying it with the inverse rotation matrix
			gsl_matrix_view hm = gsl_matrix_view_array(hd.matrix,3,3);
			int j;
			gsl_permutation* p = gsl_permutation_alloc(3);
			gsl_linalg_LU_decomp(&hm.matrix,p,&j);
			gsl_matrix* hm_invert = gsl_matrix_alloc(3,3);
			gsl_linalg_LU_invert(&hm.matrix,p,hm_invert);

			gazetracker->hcorrection.headtonasalbone[0]=0.0;
			gazetracker->hcorrection.headtonasalbone[1]=0.0;
			gazetracker->hcorrection.headtonasalbone[2]=0.0;
			gsl_vector_view eyeheadrelation = gsl_vector_view_array(gazetracker->hcorrection.headtonasalbone,3);
			//gsl_vector_int_set_zero(&eyeheadrelation.vector);
			gsl_blas_dgemv(CblasNoTrans,1.0,hm_invert,tmp_vector,1.0,&eyeheadrelation.vector);

			gazetracker->hcorrection.headtoeye[0][0] = 0.0;
			gazetracker->hcorrection.headtoeye[0][1] = 0.0;
			gazetracker->hcorrection.headtoeye[0][2] = 0.0;

			gazetracker->hcorrection.headtoeye[1][0] = 0.0;
			gazetracker->hcorrection.headtoeye[1][1] = 0.0;
			gazetracker->hcorrection.headtoeye[1][2] = 0.0;
			gsl_vector_free(tmp_vector);
			gsl_matrix_free(hm_invert);
			gsl_permutation_free(p);
		}else{

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
		}
	}else{
		return LG_QUIT;
	}
	// waits until the eye object is removed
	ireturn = gt_show_instruction(gazetracker,INSTRUCTION_4,KEY_PRESS);
	if (ireturn == LG_QUIT){
		return LG_QUIT;
	}
	return LG_OK;
}

int gt_correct_head_direction_vector(GAZE_TRACKER* gazetracker, double alpha,double beta){
	_DEBUG_("gt_correct_head_direction_vector: alpha= %f\tbeta=%f\n",alpha,beta);

	v_erase_screen(gazetracker->visualhooks);
	v_update_screen(gazetracker->visualhooks);

	// collecting the correction in degree for each axis
	double calpha = 0.0;
	double cbeta = 0.0;

	v_clear_input(gazetracker->visualhooks);

	// Allocate zeroed memory for input one time before the loop
	// this will increase performance and avoids an hang up during
	// the call of (*(vhooks->get_input))(input);
	t_input* input=(t_input*)calloc(1,sizeof(t_input));

	int keypressed = 0;
	while (keypressed==0){


		 //int x[4];
		 //int y[4];
		// generating a grid representing the field of view with
		// current correction values
		t_grid * grid;
		grid = gt_generate_grid(gazetracker,2,2,alpha,beta);
		int i;
		t_ray fov[4];
		//double fov_origin[4][3];
		//double fov_direction[4][3];
		for (i=0;i<4;i++){
			lg_get_fov_ray_from_angles(gazetracker,grid->alphas[i],grid->betas[i],fov[i].origin,fov[i].direction);

		}
		free(grid->alphas);
		free(grid->betas);
		free(grid);


		v_erase_screen(gazetracker->visualhooks);

		v_draw_fov(gazetracker->visualhooks,&fov[0],&fov[1],&fov[2],&fov[3]);
		// drawing a fixation target in the middle of the fov
		t_ray cfov;

		double pos[3];

		lg_get_fov_ray_from_angles(gazetracker,calpha,cbeta,cfov.origin,cfov.direction);

		v_draw_cal_target(gazetracker->visualhooks,&cfov,pos);
		v_update_screen(gazetracker->visualhooks);

		// checking for input data
		(*(gazetracker->visualhooks->get_input))(gazetracker->visualhooks->vptr,input);

		if (input->type == LG_INPUT_ACCEPT){
					keypressed = 1;
		}else if (input->type == LG_INPUT_QUIT){
					keypressed = 1;
					return LG_QUIT;
		}else if (input->type == LG_INPUT_KEY){
			//sleep(1);
			if (input->key == 'a'||input->key == 'A'){
				//_DEBUG_("KEY= a\n");
						 gazetracker->hcorrection.correct_x +=0.5;
			}else if (input->key == 'y'||input->key == 'Y'){
				//_DEBUG_("KEY= y\n");
				gazetracker->hcorrection.correct_x -=0.5;
			}else if (input->key == 's'||input->key == 'S'){
				//_DEBUG_("KEY= s\n");
				gazetracker->hcorrection.correct_y +=0.5;
			}else if (input->key == 'x'||input->key == 'X'){
				//_DEBUG_("KEY= x\n");
				gazetracker->hcorrection.correct_y -=0.5;
			}else if (input->key == 'd'||input->key == 'D'){
				//_DEBUG_("KEY= d\n");
				gazetracker->hcorrection.correct_z +=0.5;
			}else if (input->key == 'c'||input->key == 'C'){
				//_DEBUG_("KEY= c\n");
				gazetracker->hcorrection.correct_z -=0.5;
			}/*else if (input->key == 'f'||input->key == 'F'){
				//_DEBUG_("KEY= f\n");
				calpha +=0.5;
			}else if (input->key == 'v'||input->key == 'V'){
				//_DEBUG_("KEY= v\n");
				calpha -=0.5;
			}else if (input->key == 'g'||input->key == 'G'){
				//_DEBUG_("KEY= g\n");
				cbeta +=0.5;
			}else if (input->key == 'b'||input->key == 'B'){
				//_DEBUG_("KEY= b\n");
				cbeta -=0.5;
			}
			*/


			// generating the new correction matrix with the new correction values
			gsl_matrix* x_matrix = gsl_matrix_calloc(3,3);
			gsl_matrix* y_matrix = gsl_matrix_calloc(3,3);
			gsl_matrix* z_matrix = gsl_matrix_calloc(3,3);

			gsl_matrix* tmp_matrix = gsl_matrix_calloc(3,3);
			gsl_matrix* tmp_matrix2 = gsl_matrix_calloc(3,3);

			gsl_matrix_view cmatrix = gsl_matrix_view_array(gazetracker->hcorrection.correct_matrix,3,3);


			lg_tools_matrix_rotationXYZ(z_matrix,'z',lg_tools_radians(gazetracker->hcorrection.correct_z));
			lg_tools_matrix_rotationXYZ(x_matrix,'x',lg_tools_radians(gazetracker->hcorrection.correct_x));
			lg_tools_matrix_rotationXYZ(y_matrix,'y',lg_tools_radians(gazetracker->hcorrection.correct_y));
			//gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,x_matrix,y_matrix,1.0,tmp_matrix);
			//gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,tmp_matrix,z_matrix,1.0,tmp_matrix2);
			gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,z_matrix,y_matrix,1.0,tmp_matrix);
			gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,tmp_matrix,x_matrix,1.0,tmp_matrix2);
			gsl_matrix_memcpy(&cmatrix.matrix,tmp_matrix2);
			gsl_matrix_free(x_matrix);
			gsl_matrix_free(y_matrix);
			gsl_matrix_free(z_matrix);
			gsl_matrix_free(tmp_matrix);
			gsl_matrix_free(tmp_matrix2);

			MILLISEC_SLEEP(10);
		}
	}

	v_erase_screen(gazetracker->visualhooks);
	v_update_screen(gazetracker->visualhooks);
	return LG_OK;
}

int gt_print_calibration_data(GAZE_TRACKER* gazetracker, char* tag, t_calibration_data_set* cds){
	_DEBUG_("gt_print_calibration_data\n");
	if (gazetracker->log_mode & CALIB_LOG_FILE && gazetracker->calibration_log != NULL){

		unsigned long long time = lg_get_time();
		fprintf(gazetracker->calibration_log,"%s %llu %d\n",tag,time,cds->num);
		fprintf(gazetracker->calibration_log,"EHR %f %f %f %f %f %f\n",gazetracker->hcorrection.headtonasalbone[0],gazetracker->hcorrection.headtonasalbone[1],gazetracker->hcorrection.headtonasalbone[2],gazetracker->hcorrection.correct_x,gazetracker->hcorrection.correct_y,gazetracker->hcorrection.correct_z);
		int i;
		for(i=0;i<cds->num;i++){
			t_calibration_data *tmp = &(cds->cdata[i]);
			fprintf(gazetracker->calibration_log,"CP %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",tmp->ctarget_pos[0], tmp->ctarget_pos[1], tmp->ctarget_pos[2], tmp->fov_alpha, tmp->fov_beta, tmp->eye.pupil_x[0],tmp->eye.pupil_y[0],tmp->eye.pupil_x[1],tmp->eye.pupil_y[1],tmp->head.position[0],tmp->head.position[1],tmp->head.position[2],tmp->head.matrix[0],tmp->head.matrix[1],tmp->head.matrix[2],tmp->head.matrix[3],tmp->head.matrix[4],tmp->head.matrix[5],tmp->head.matrix[6],tmp->head.matrix[7],tmp->head.matrix[8]);
		}

	}

	return 1;
}

int gt_calibrate(GAZE_TRACKER* gazetracker, double alpha,double beta,int n, int m, double* center){
	_DEBUG_("gt_calibrate\n");
	if(gazetracker->mode == GT_MODE_DUMMY || gazetracker->mode == GT_MODE_HEAD){
		gazetracker->used_eye = LG_EYE_BOTH;
	}else{
		et_eye_available(gazetracker->eyetracker,&gazetracker->used_eye);
	}
	int ireturn =0;
	ireturn = gt_show_instruction(gazetracker, INSTRUCTION_0,KEY_PRESS);
	if (ireturn == LG_QUIT){
		return LG_QUIT;
	}
	/*
	int cx;
	int cy;
	if (center == NULL){
		gt_get_gaze_with_alpha_beta(gazetracker,0.0,0.0,&cx,&cy);

	}else{
		cx = center[0];
		cy = center[1];
	}

	_DEBUG_("gt_calibrate: cx: %d\ty: %d\n",cx,cy);
	*/
	t_calibration_data_set* cdataset;
	cdataset = (t_calibration_data_set*)calloc(1,sizeof(t_calibration_data_set));

	ireturn = gt_show_grid(gazetracker,n,m,alpha,beta,center,cdataset);

	if (ireturn == LG_OK){
		_DEBUG_("gt_calibrate: ireturn = LG_OK\n");
		if (cdataset->bad == 0){
			_DEBUG_("Number of calibration points in cdataset: %d\n",cdataset->num);
			gt_print_calibration_data(gazetracker,"CALIBRATION",cdataset);
			int i;
			for (i=0;i<cdataset->num;i++){
				_DEBUG_("i: %d\n",i);
				lg_tools_correct_head_data(&(cdataset->cdata[i].head),gazetracker->hcorrection.headtonasalbone,
					gazetracker->hcorrection.correct_matrix);
				_DEBUG_("Done i: %d\n",i);
			}
			//TODO: add calculation of left and right eye
			c_processCalibrationData(gazetracker->calibration,gazetracker->used_eye, cdataset);
			gazetracker->calibdataset = cdataset;
			gazetracker->is_calibrated = 1;
			return LG_OK;
		}else{
			gt_show_instruction(gazetracker, INSTRUCTION_6,KEY_PRESS);
			return LG_ERROR;
		}
	}else{
		return ireturn;
	}

}

int gt_validate_calibration(GAZE_TRACKER* gazetracker, t_validation_data_set *vdataset,double alpha, double beta, int n, int m, double* center){
	_DEBUG_("gt_validate_calibration\n");
	//if(gazetracker->mode == GT_MODE_BOTH){
	int ireturn =0;
	ireturn = gt_show_instruction(gazetracker, INSTRUCTION_1,KEY_PRESS);
	if(ireturn==LG_QUIT){
		return LG_QUIT;
	}
	/*
	int cx;
	int cy;
	if (center == NULL){
		gt_get_gaze_with_alpha_beta(gazetracker,0.0,0.0,&cx,&cy);

	}else{
		cx = center[0];
		cy = center[1];
	}
	_DEBUG_("gt_validate_calibration: cx: %d\ty: %d\n",cx,cy);
	*/
	t_calibration_data_set* cdataset;
	cdataset = (t_calibration_data_set*)calloc(1,sizeof(t_calibration_data_set));

	ireturn = gt_show_grid(gazetracker,n,m,alpha,beta,center,cdataset);

	if (ireturn == LG_OK){

		gt_print_calibration_data(gazetracker,"VALIDATION",cdataset);

		gazetracker->validdataset = NULL;
		c_validate_calibration(gazetracker,cdataset,vdataset);
		gazetracker->validdataset = cdataset;
		if(GAZE_LOG_FILE & gazetracker->log_mode){
			g_mutex_lock(gazetracker->mutex_log_file);
			if(gazetracker->log_file!=NULL){
				unsigned long long time = lg_get_time();
				fprintf(gazetracker->log_file,"V\t%llu\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",time,vdataset->avg_drift[0],vdataset->avg_drift[1],vdataset->max_drift[0],vdataset->max_drift[1],vdataset->avg_drift_alpha[0],vdataset->avg_drift_alpha[1],vdataset->avg_drift_beta[0],vdataset->avg_drift_beta[1]);
			}
			g_mutex_unlock(gazetracker->mutex_log_file);
		}
	}
	return ireturn;
}

int gt_display_last_validation(GAZE_TRACKER* gazetracker){
	if (gazetracker->validdataset!=NULL){
		t_validation_data_set vdataset;

		c_validate_calibration(gazetracker,gazetracker->validdataset,&vdataset);
		v_erase_screen(gazetracker->visualhooks);
		int j=0;
		for (j=0; j< gazetracker->validdataset->num;j++){
			//int *xy;
			//double vtdirection[3];
			t_ray tmp_ray;
			tmp_ray.origin[0] = vdataset.vdata[j].gaze.head.position[0];
			tmp_ray.origin[1] = vdataset.vdata[j].gaze.head.position[1];
			tmp_ray.origin[2] = vdataset.vdata[j].gaze.head.position[2];
			tmp_ray.direction[0] = vdataset.vdata[j].vtarget_pos[0] -vdataset.vdata[j].gaze.head.position[0];
			tmp_ray.direction[1] = vdataset.vdata[j].vtarget_pos[1] -vdataset.vdata[j].gaze.head.position[1];
			tmp_ray.direction[2] = vdataset.vdata[j].vtarget_pos[2] -vdataset.vdata[j].gaze.head.position[2];

			double pos3d[3];
			v_draw_validation(gazetracker->visualhooks,&tmp_ray,pos3d,0);

			if (vdataset.vdata[j].flag == LG_CDATA_GOOD){
				if(gazetracker->used_eye ==LG_EYE_BOTH || gazetracker->used_eye ==LG_EYE_LEFT){
					t_ray l_ray;
					l_ray.origin[0] = vdataset.vdata[j].gaze.gaze[0].origin[0];
					l_ray.origin[1] = vdataset.vdata[j].gaze.gaze[0].origin[1];
					l_ray.origin[2] = vdataset.vdata[j].gaze.gaze[0].origin[2];
					l_ray.direction[0] = vdataset.vdata[j].gaze.gaze[0].direction[0];
					l_ray.direction[1] = vdataset.vdata[j].gaze.gaze[0].direction[1];
					l_ray.direction[2] = vdataset.vdata[j].gaze.gaze[0].direction[2];

					v_draw_validation(gazetracker->visualhooks,&l_ray,pos3d,1);

				}
				if(gazetracker->used_eye ==LG_EYE_BOTH || gazetracker->used_eye ==LG_EYE_RIGHT){
					t_ray r_ray;
					r_ray.origin[0] = vdataset.vdata[j].gaze.gaze[1].origin[0];
					r_ray.origin[1] = vdataset.vdata[j].gaze.gaze[1].origin[1];
					r_ray.origin[2] = vdataset.vdata[j].gaze.gaze[1].origin[2];
					r_ray.direction[0] = vdataset.vdata[j].gaze.gaze[1].direction[0];
					r_ray.direction[1] = vdataset.vdata[j].gaze.gaze[1].direction[1];
					r_ray.direction[2] = vdataset.vdata[j].gaze.gaze[1].direction[2];
					v_draw_validation(gazetracker->visualhooks,&r_ray,pos3d,2);

				}
			}
		}
		int ireturn;
		int end = 0;

		// Using clear input with update screen before (see above) gives me an excpetion during
		// call to get input ... (Adrian)
		v_clear_input(gazetracker->visualhooks);
		v_update_screen(gazetracker->visualhooks);

		t_input input;
		while (end==0){
			v_get_input(gazetracker->visualhooks,&input);

				if (input.type == LG_INPUT_ACCEPT){
					end = 1;
					ireturn = LG_ACCEPT;
				}else if (input.type == LG_INPUT_ABORT){
					end = 1;
					ireturn = LG_ABORT;
				}
				MILLISEC_SLEEP(5);
		}
		return ireturn;
	}else{
		return LG_ERROR;
	}
}

int gt_drift_check(GAZE_TRACKER* gazetracker, t_validation_data_set *vdataset,double* pos,double alpha, double beta, int n, int m, int count){

	int ireturn = LG_OK;
	if(pos!=NULL){
		_DEBUG_("gt_drift_correction: pos: %f\t%f\t%f\t alpha: %f beta: %f n: %d m: %d count: %d\n",pos[0],pos[1],pos[2],alpha,beta,n,m,count);
		ireturn = gt_do_head_fixation(gazetracker,pos,2000,0);
	}
	if (ireturn !=LG_QUIT){
		t_grid* grid;
		grid = gt_generate_grid(gazetracker,n, m, alpha, beta);
		t_grid ngrid;
		gt_select_n_from_grid(grid,count,&ngrid);
		t_calibration_data_set* dcdataset = (t_calibration_data_set*)calloc(1,sizeof(t_calibration_data_set));
		int ireturn = gt_collect_data_for_grid(gazetracker,&ngrid,dcdataset, 1);
		//free(grid->alphas);
		//free(grid->betas);
		//free(grid);
		if (ireturn == LG_OK){
			gt_print_calibration_data(gazetracker,"DRIFT_CORRECTION",dcdataset);

			c_validate_calibration(gazetracker,dcdataset,vdataset);
			//free(ddataset->cdata);
			//free(ddataset);

			if(GAZE_LOG_FILE & gazetracker->log_mode){
				g_mutex_lock(gazetracker->mutex_log_file);
				if(gazetracker->log_file!=NULL){
					unsigned long long time = lg_get_time();
					fprintf(gazetracker->log_file,"D\t%llu\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",time,vdataset->avg_drift[0],vdataset->avg_drift[1],vdataset->max_drift[0],vdataset->max_drift[1],vdataset->avg_drift_alpha[0],vdataset->avg_drift_alpha[1],vdataset->avg_drift_beta[0],vdataset->avg_drift_beta[1]);
				}
				g_mutex_unlock(gazetracker->mutex_log_file);
			}

			return LG_OK;
		}else{
			return ireturn;
		}

	}else{
		return LG_QUIT;
	}

	return 0;
}



