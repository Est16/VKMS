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
#include <parse_conf.h>

#include <libGaze_defines.h>
#include <libGaze_types.h>
#include <libGaze_error.h>
#include <calibration/libGaze_calibration.h>
#include <gazetracker/libGaze_gazetracker.h>
#include <gazetracker/libGaze_gazetracker_types.h>


#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include "tools/libGaze_tools.h"

int gt_save_calibration_data(GAZE_TRACKER* gazetracker,char* filename){
	_DEBUG_("gt_save_calibration_data\n");
	if(gazetracker->calibdataset!=NULL){

		FILE* file;
		file = fopen(filename,"w");
		fprintf(file,"[gazetracker_CALIBRATION]\n");
		fprintf(file,"\tcalibrated_eye=%d\n",gazetracker->used_eye);
		fprintf(file,"\tcorrect_x=%f\n",gazetracker->hcorrection.correct_x);
		fprintf(file,"\tcorrect_y=%f\n",gazetracker->hcorrection.correct_y);
		fprintf(file,"\tcorrect_z=%f\n",gazetracker->hcorrection.correct_z);

		fprintf(file,"\tcorrect_matrix_0=%f\n",gazetracker->hcorrection.correct_matrix[0]);
		fprintf(file,"\tcorrect_matrix_1=%f\n",gazetracker->hcorrection.correct_matrix[1]);
		fprintf(file,"\tcorrect_matrix_2=%f\n",gazetracker->hcorrection.correct_matrix[2]);
		fprintf(file,"\tcorrect_matrix_3=%f\n",gazetracker->hcorrection.correct_matrix[3]);
		fprintf(file,"\tcorrect_matrix_4=%f\n",gazetracker->hcorrection.correct_matrix[4]);
		fprintf(file,"\tcorrect_matrix_5=%f\n",gazetracker->hcorrection.correct_matrix[5]);
		fprintf(file,"\tcorrect_matrix_6=%f\n",gazetracker->hcorrection.correct_matrix[6]);
		fprintf(file,"\tcorrect_matrix_7=%f\n",gazetracker->hcorrection.correct_matrix[7]);
		fprintf(file,"\tcorrect_matrix_8=%f\n",gazetracker->hcorrection.correct_matrix[8]);



		//printf("correct: %f\t%f\t%f\n",gazetracker->hcorrection.correct_x,gazetracker->hcorrection.correct_y,gazetracker->hcorrection.correct_z);
		//printf("matrix: %f\t%f\t%f\t\t%f\t%f\t%f\t\t%f\t%f\t%f\n",gazetracker->hcorrection.correct_matrix[0],gazetracker->hcorrection.correct_matrix[1],gazetracker->hcorrection.correct_matrix[2],gazetracker->hcorrection.correct_matrix[3],gazetracker->hcorrection.correct_matrix[4],gazetracker->hcorrection.correct_matrix[5],gazetracker->hcorrection.correct_matrix[6],gazetracker->hcorrection.correct_matrix[7],gazetracker->hcorrection.correct_matrix[8]);


		fprintf(file,"\teye_head_relation_x=%f\n",gazetracker->hcorrection.headtonasalbone[0]);
		fprintf(file,"\teye_head_relation_y=%f\n",gazetracker->hcorrection.headtonasalbone[1]);
		fprintf(file,"\teye_head_relation_z=%f\n",gazetracker->hcorrection.headtonasalbone[2]);

		fprintf(file,"\tnumcp=%i\n",gazetracker->calibdataset->num);

		int i;
		for(i=0;i<gazetracker->calibdataset->num;i++){
			t_calibration_data* tmp;
			tmp = &(gazetracker->calibdataset->cdata[i]);
			fprintf(file,"\t[CP%i]\n",i);

			fprintf(file,"\t\tx=%f\n",tmp->ctarget_pos[0]);
			fprintf(file,"\t\ty=%f\n",tmp->ctarget_pos[1]);
			fprintf(file,"\t\tz=%f\n",tmp->ctarget_pos[2]);

			fprintf(file,"\t\tcl_x=%f\n",tmp->eye.pupil_x[0]);
			fprintf(file,"\t\tcl_y=%f\n",tmp->eye.pupil_y[0]);
			fprintf(file,"\t\tcr_x=%f\n",tmp->eye.pupil_x[1]);
			fprintf(file,"\t\tcr_y=%f\n",tmp->eye.pupil_y[1]);

			fprintf(file,"\t\tpos_x=%f\n",tmp->head.position[0]);
			fprintf(file,"\t\tpos_y=%f\n",tmp->head.position[1]);
			fprintf(file,"\t\tpos_z=%f\n",tmp->head.position[2]);

			fprintf(file,"\t\tr_00=%f\n",tmp->head.matrix[0]);
			fprintf(file,"\t\tr_01=%f\n",tmp->head.matrix[1]);
			fprintf(file,"\t\tr_02=%f\n",tmp->head.matrix[2]);
			fprintf(file,"\t\tr_10=%f\n",tmp->head.matrix[3]);
			fprintf(file,"\t\tr_11=%f\n",tmp->head.matrix[4]);
			fprintf(file,"\t\tr_12=%f\n",tmp->head.matrix[5]);
			fprintf(file,"\t\tr_20=%f\n",tmp->head.matrix[6]);
			fprintf(file,"\t\tr_21=%f\n",tmp->head.matrix[7]);
			fprintf(file,"\t\tr_22=%f\n",tmp->head.matrix[8]);

		}

		fclose(file);
	}
	return 1;
}

int gt_load_calibration_data(GAZE_TRACKER* gazetracker,char* filename){
	_DEBUG_("gt_load_calibration_data: %s\n",filename);
	pcnf_t *pcnf = pcnf_alloc();
	pcnf_read(pcnf,filename);
	gazetracker->used_eye = (enum lg_eyes)(pcnf_iget(pcnf,"gazetracker_CALIBRATION","calibrated_eye"));
	gazetracker->hcorrection.correct_x = pcnf_fget(pcnf,"gazetracker_CALIBRATION","correct_x");
	gazetracker->hcorrection.correct_y = pcnf_fget(pcnf,"gazetracker_CALIBRATION","correct_y");
	gazetracker->hcorrection.correct_z = pcnf_fget(pcnf,"gazetracker_CALIBRATION","correct_z");


	//printf("correct: %f\t%f\t%f\n",gazetracker->hcorrection.correct_x,gazetracker->hcorrection.correct_y,gazetracker->hcorrection.correct_z);

	gazetracker->hcorrection.correct_matrix[0] = pcnf_fget(pcnf,"gazetracker_CALIBRATION","correct_matrix_0");
	gazetracker->hcorrection.correct_matrix[1] = pcnf_fget(pcnf,"gazetracker_CALIBRATION","correct_matrix_1");
	gazetracker->hcorrection.correct_matrix[2] = pcnf_fget(pcnf,"gazetracker_CALIBRATION","correct_matrix_2");
	gazetracker->hcorrection.correct_matrix[3] = pcnf_fget(pcnf,"gazetracker_CALIBRATION","correct_matrix_3");
	gazetracker->hcorrection.correct_matrix[4] = pcnf_fget(pcnf,"gazetracker_CALIBRATION","correct_matrix_4");
	gazetracker->hcorrection.correct_matrix[5] = pcnf_fget(pcnf,"gazetracker_CALIBRATION","correct_matrix_5");
	gazetracker->hcorrection.correct_matrix[6] = pcnf_fget(pcnf,"gazetracker_CALIBRATION","correct_matrix_6");
	gazetracker->hcorrection.correct_matrix[7] = pcnf_fget(pcnf,"gazetracker_CALIBRATION","correct_matrix_7");
	gazetracker->hcorrection.correct_matrix[8] = pcnf_fget(pcnf,"gazetracker_CALIBRATION","correct_matrix_8");


	//printf("matrix: %f\t%f\t%f\t\t%f\t%f\t%f\t\t%f\t%f\t%f\n",gazetracker->hcorrection.correct_matrix[0],gazetracker->hcorrection.correct_matrix[1],gazetracker->hcorrection.correct_matrix[2],gazetracker->hcorrection.correct_matrix[3],gazetracker->hcorrection.correct_matrix[4],gazetracker->hcorrection.correct_matrix[5],gazetracker->hcorrection.correct_matrix[6],gazetracker->hcorrection.correct_matrix[7],gazetracker->hcorrection.correct_matrix[8]);

/*
	gsl_matrix* x_matrix = gsl_matrix_calloc(3,3);
	gsl_matrix* y_matrix = gsl_matrix_calloc(3,3);
	gsl_matrix* z_matrix = gsl_matrix_calloc(3,3);

	gsl_matrix* tmp_matrix = gsl_matrix_calloc(3,3);
	gsl_matrix* tmp_matrix2 = gsl_matrix_calloc(3,3);

	gsl_matrix_view cmatrix = gsl_matrix_view_array(gazetracker->hcorrection.correct_matrix,3,3);


	matrix_rotationXYZ(z_matrix,'z',radians(gazetracker->hcorrection.correct_z));
	matrix_rotationXYZ(x_matrix,'x',radians(gazetracker->hcorrection.correct_x));
	matrix_rotationXYZ(y_matrix,'y',radians(gazetracker->hcorrection.correct_y));
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

	printf("matrix: %f\t%f\t%f\t\t%f\t%f\t%f\t\t%f\t%f\t%f\n",gazetracker->hcorrection.correct_matrix[0],gazetracker->hcorrection.correct_matrix[1],gazetracker->hcorrection.correct_matrix[2],gazetracker->hcorrection.correct_matrix[3],gazetracker->hcorrection.correct_matrix[4],gazetracker->hcorrection.correct_matrix[5],gazetracker->hcorrection.correct_matrix[6],gazetracker->hcorrection.correct_matrix[7],gazetracker->hcorrection.correct_matrix[8]);
*/
	gazetracker->hcorrection.headtonasalbone[0] = pcnf_fget(pcnf,"gazetracker_CALIBRATION","eye_head_relation_x");
	gazetracker->hcorrection.headtonasalbone[1] = pcnf_fget(pcnf,"gazetracker_CALIBRATION","eye_head_relation_y");
	gazetracker->hcorrection.headtonasalbone[2] = pcnf_fget(pcnf,"gazetracker_CALIBRATION","eye_head_relation_z");

	gazetracker->hcorrection.headtoeye[0][0] = 0.0;
	gazetracker->hcorrection.headtoeye[0][1] = 0.0;
	gazetracker->hcorrection.headtoeye[0][2] = 0.0;

	gazetracker->hcorrection.headtoeye[1][0] = 0.0;
	gazetracker->hcorrection.headtoeye[1][1] = 0.0;
	gazetracker->hcorrection.headtoeye[1][2] = 0.0;

	_DEBUG_("eye_head_relation_x: %f\n",gazetracker->hcorrection.headtonasalbone[0]);
	_DEBUG_("eye_head_relation_y: %f\n",gazetracker->hcorrection.headtonasalbone[1]);
	_DEBUG_("eye_head_relation_z: %f\n",gazetracker->hcorrection.headtonasalbone[2]);

	gazetracker->calibdataset = (t_calibration_data_set*)malloc(sizeof(t_calibration_data_set));
	gazetracker->calibdataset->num = 	pcnf_iget(pcnf,"gazetracker_CALIBRATION","numcp");

	_DEBUG_("num: %d\n",gazetracker->calibdataset->num);

	gazetracker->calibdataset->cdata = (t_calibration_data*)malloc(gazetracker->calibdataset->num*sizeof(t_calibration_data));

	int i;
	for(i=0;i<gazetracker->calibdataset->num;i++){

		char cp[128];

		//printf("%s\n",cp);
		sprintf(cp,"CP%d",i);
		//printf("%s\n",cp);

		gazetracker->calibdataset->cdata[i].ctarget_pos[0] = pcnf_fget(pcnf,cp,"x");
		gazetracker->calibdataset->cdata[i].ctarget_pos[1] = pcnf_fget(pcnf,cp,"y");
		gazetracker->calibdataset->cdata[i].ctarget_pos[2] = pcnf_fget(pcnf,cp,"z");
		//int xy[]={gazetracker->calibdataset->cdata[i].dis_x,gazetracker->calibdataset->cdata[i].dis_y};
		//gazetracker->calibdataset->cdata[i].dis_vec = (*(gazetracker->display->get3DPositionFromXY))(xy);
		//_DEBUG_("dis[%d]= %f\t%f\t%f\n",i,gazetracker->calibdataset->cdata[i].dis_vec[0],gazetracker->calibdataset->cdata[i].dis_vec[1],gazetracker->calibdataset->cdata[i].dis_vec[2]);

		//gazetracker->calibdataset->cdata[i].eye = (t_eye_data*)malloc(sizeof(t_eye_data));

		gazetracker->calibdataset->cdata[i].eye.eye_available = LG_EYE_BOTH;
		gazetracker->calibdataset->cdata[i].eye.pupil_x[0] = pcnf_fget(pcnf,cp,"cl_x");
		gazetracker->calibdataset->cdata[i].eye.pupil_y[0] = pcnf_fget(pcnf,cp,"cl_y");
		gazetracker->calibdataset->cdata[i].eye.pupil_x[1] = pcnf_fget(pcnf,cp,"cr_x");
		gazetracker->calibdataset->cdata[i].eye.pupil_y[1] = pcnf_fget(pcnf,cp,"cr_y");

		//gazetracker->calibdataset->cdata[i].head = (t_head_data*)malloc(sizeof(t_head_data));
		gazetracker->calibdataset->cdata[i].head.position[0] = pcnf_fget(pcnf,cp,"pos_x");
		gazetracker->calibdataset->cdata[i].head.position[1] = pcnf_fget(pcnf,cp,"pos_y");
		gazetracker->calibdataset->cdata[i].head.position[2] = pcnf_fget(pcnf,cp,"pos_z");

		gazetracker->calibdataset->cdata[i].head.matrix[0] = pcnf_fget(pcnf,cp,"r_00");
		gazetracker->calibdataset->cdata[i].head.matrix[1] = pcnf_fget(pcnf,cp,"r_01");
		gazetracker->calibdataset->cdata[i].head.matrix[2] = pcnf_fget(pcnf,cp,"r_02");
		gazetracker->calibdataset->cdata[i].head.matrix[3] = pcnf_fget(pcnf,cp,"r_10");
		gazetracker->calibdataset->cdata[i].head.matrix[4] = pcnf_fget(pcnf,cp,"r_11");
		gazetracker->calibdataset->cdata[i].head.matrix[5] = pcnf_fget(pcnf,cp,"r_12");
		gazetracker->calibdataset->cdata[i].head.matrix[6] = pcnf_fget(pcnf,cp,"r_20");
		gazetracker->calibdataset->cdata[i].head.matrix[7] = pcnf_fget(pcnf,cp,"r_21");
		gazetracker->calibdataset->cdata[i].head.matrix[8] = pcnf_fget(pcnf,cp,"r_22");

	}

	pcnf_free(pcnf);
	_DEBUG_("gt_load_modules: finished loading calibration data\n");
	_DEBUG_("gt_load_modules: start processing calibration data\n");
	c_processCalibrationData(gazetracker->calibration,
		 gazetracker->used_eye,
		 gazetracker->calibdataset);


	gazetracker->is_calibrated = 1;
	return 1;

}


