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
#include <glib.h>
#include <gmodule.h>
#include <math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>

#include <libGaze.h>
#include <libGaze_error.h>
#include <calibration/libGaze_calibration_types.h>
#include <tools/libGaze_tools.h>



CALIBRATION* c_load_module(char* module_path,int debug, int error){
	_DEBUG_("c_load_module: %s\t%d\t%d\n",module_path, debug, error);
	//void *mod;
	GModule *mod;
	mod = g_module_open(module_path,G_MODULE_BIND_LOCAL);
	if(mod==NULL){
		_ERROR_("%s\n",g_module_error());
		return NULL;
	}


	CALIBRATION *calib;
	calib = (CALIBRATION*)calloc(1,sizeof(CALIBRATION));
	if(calib == NULL){
		_ERROR_("could allocate memory for CALIBRATION\n");
		return NULL;
	}
	calib->module = mod;

	int merror =0;



	if(!g_module_symbol(mod,CALIBRATION_PROCESS_CALIBRATION_DATA,(gpointer*)&(calib->processCalibrationData))){

		_ERROR_("%s\n",g_module_error());
		merror ++;
	}

	if(!g_module_symbol(mod,CALIBRATION_PROCESS_EYE_DATA,(gpointer*)&(calib->procesEyeData))){

		_ERROR_("%s\n",g_module_error());
		merror ++;
	}

	if(merror>0){
		_ERROR_("calibration module does not implement all needed functions\n");
		return NULL;
	}


	int (*c_init)(int, int ,FILE*,FILE*,struct timeval*);
	_DEBUG_("init\n");
	if(g_module_symbol(mod,CALIBRATION_INIT_MODULE,(gpointer*)&(c_init))){

		 _DEBUG_("init: %p\tdebug: %d\terror:%d\n",c_init,debug,error);
		(*c_init)(debug,error,lg_debug,lg_error, log_base_time);
	}else{
		_ERROR_("%s\n",g_module_error());
		_ERROR_("ht_init = NULL\n");
		return NULL;
	}

	_DEBUG_("end_load_module\n");
	return calib;
}

int c_unload_module(CALIBRATION* calib){
	if(calib != NULL){
		if (calib->module != NULL){
			//g_module_close(head->module);
			int (*c_fini)(void);
			_DEBUG_("call fini_module\n");
			if(g_module_symbol(calib->module,CALIBRATION_FINI_MODULE,(gpointer*)&(c_fini)))
			{
//				_DEBUG_("finalize: %p\t\n",c_fini);
				(*c_fini)();
			}else{
				_ERROR_("%s\n",g_module_error());
				_ERROR_("c_fini = NULL\n");
				return -1;
			}
			free(calib);
			return 0;
		}else{
			_ERROR_("unload_module: module = NULL\n");
			free(calib);
		}

	}
	_ERROR_("unload_module: calib = NULL\n");
	return -1;
}

int c_processCalibrationData(CALIBRATION* calib,int eye,t_calibration_data_set* calibdataset){
	_DEBUG_("c_processCalibrationData\n");
	return (*(calib->processCalibrationData))(eye,calibdataset);
}

int c_processEyeData(CALIBRATION* calib,t_raw_eye_data* eyed,t_eye_data* gazed){
	return (*(calib->procesEyeData))(eyed,gazed);
}

int c_validate_calibration(GAZE_TRACKER *gazetracker, t_calibration_data_set *cdataset, t_validation_data_set *vdataset){
	//printf("c_validate_calibration: %d\n",cdataset->num);
	int i;
	vdataset->num = cdataset->num;
	vdataset->vdata = (t_validation_data*)calloc(cdataset->num,sizeof(t_validation_data));
	t_validation_data *vd = vdataset->vdata;
	t_calibration_data *cd= cdataset->cdata;

	vdataset->good = 0;
	vdataset->bad = 0;
	vdataset->max_drift[0] = DBL_MIN;
	vdataset->max_drift[1] = DBL_MIN;
	vdataset->avg_drift[0] = 0.0;
	vdataset->avg_drift[1] = 0.0;
	vdataset->avg_drift_alpha[0] = 0.0;
	vdataset->avg_drift_alpha[1] = 0.0;
	vdataset->avg_drift_beta[0] = 0.0;
	vdataset->avg_drift_beta[1] = 0.0;

	for (i=0;i<cdataset->num;i++){

		if (cd[i].flag == LG_CDATA_GOOD){
			//printf("i: %d\t GOOD\n",i);
			vd[i].flag = LG_CDATA_GOOD;

			vd[i].vtarget_pos[0] = cd[i].ctarget_pos[0];
			vd[i].vtarget_pos[1] = cd[i].ctarget_pos[1];
			vd[i].vtarget_pos[2] = cd[i].ctarget_pos[2];

			vd[i].fov_alpha = cd[i].fov_alpha;
			vd[i].fov_beta = cd[i].fov_beta;

			gt_get_current_gaze_position_from_raw_eye_head_data(gazetracker,&(cd[i].eye),&(cd[i].head),&(vd[i].gaze));

			int j;
			for(j=0;j<2;j++){
				double valid_vector[3];
				valid_vector[0]=vd[i].vtarget_pos[0]-vd[i].gaze.gaze[j].origin[0];
				valid_vector[1]=vd[i].vtarget_pos[1]-vd[i].gaze.gaze[j].origin[1];
				valid_vector[2]=vd[i].vtarget_pos[2]-vd[i].gaze.gaze[j].origin[2];

				gsl_vector_view tmp_valid_vector = gsl_vector_view_array(valid_vector,3);

				gsl_matrix_view hm_tmp = gsl_matrix_view_array(vd[i].gaze.head.orientation,3,3);
				gsl_matrix *hm = gsl_matrix_calloc(3,3);
				gsl_matrix_memcpy(hm,&hm_tmp.matrix);
				int j;
				gsl_permutation* p = gsl_permutation_calloc(3);
				gsl_linalg_LU_decomp(hm,p,&j);
				gsl_matrix* hm_inv = gsl_matrix_calloc(3,3);
				gsl_linalg_LU_invert(hm,p,hm_inv);

				gsl_vector *tmpvector2 = gsl_vector_calloc(3);
				gsl_blas_dgemv(CblasNoTrans,1.0,hm_inv,&tmp_valid_vector.vector,1.0,tmpvector2);

				double* angles = lg_tools_getYZAnglesFrom3DVector(tmpvector2);
				//printf("alpha: %f\tbeta: %f\n",angles[0],angles[1]);
				//printf("vd[i].gaze.eye.alpha: %f\tvd[i].gaze.eye.alpha:%f\nvd[i].gaze.eye.alpha",vd[i].gaze.eye.alpha[j],vd[i].gaze.eye.beta[j]);

				vd[i].alpha[j] = angles[0];
				vd[i].beta[j] = angles[1];

				vd[i].calpha[j] = vd[i].gaze.eye.alpha[j];
				vd[i].cbeta[j] = vd[i].gaze.eye.beta[j];
				vd[i].cdrift[j] = sqrt(pow((vd[i].calpha[j]-vd[i].alpha[j]),2)+pow((vd[i].cbeta[j]-vd[i].beta[j]),2));
				vdataset->avg_drift[j] += vd[i].cdrift[j];
				vdataset->avg_drift_alpha[j] += vd[i].calpha[j]-vd[i].alpha[j];
				vdataset->avg_drift_beta[j] += vd[i].beta[j]-vd[i].beta[j];
			}


			vdataset->good++;
			if(vd[i].cdrift[0]>vdataset->max_drift[0]){
				vdataset->max_drift[0] =vd[i].cdrift[0];
			}
			if(vd[i].cdrift[1]>vdataset->max_drift[1]){
				vdataset->max_drift[1] =vd[i].cdrift[1];
			}

		}else{
			//printf("i: %d\t BAD\n",i);
			vd[i].flag = cd[i].flag;
			vd[i].fov_alpha = cd[i].fov_alpha;
			vd[i].fov_beta = cd[i].fov_beta;
			vd[i].vtarget_pos[0] = cd[i].ctarget_pos[0];
			vd[i].vtarget_pos[1] = cd[i].ctarget_pos[1];
			vd[i].vtarget_pos[2] = cd[i].ctarget_pos[2];
		}
	}
	vdataset->avg_drift[0]/=vdataset->good;
	vdataset->avg_drift[1]/=vdataset->good;
	vdataset->avg_drift_alpha[0]/=vdataset->good;
	vdataset->avg_drift_alpha[1]/=vdataset->good;
	vdataset->avg_drift_beta[0]/=vdataset->good;
	vdataset->avg_drift_beta[1]/=vdataset->good;
	_DEBUG_("avg_drift: %f\t%f\n",vdataset->avg_drift[0],vdataset->avg_drift[1]);

	return 1;
}



