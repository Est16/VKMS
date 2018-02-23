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

#include <math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include "libGaze_error.h"
#include "tools/libGaze_tools.h"
#include "headtracker/libGaze_headtracker.h"
#include "gazetracker/libGaze_gazetracker.h"

#ifndef M_PI
#define M_PI 3.1415927
#endif

int lg_tools_correct_head_data(t_raw_head_data* hdata,double* position_correction, double* orientation_correction){
	//_DEBUG_("correct_head_data\n");
	gsl_vector_view hpos = gsl_vector_view_array(hdata->position,3);
	gsl_matrix_view hmatrix = gsl_matrix_view_array(hdata->matrix,3,3);
	//_DEBUG_("correct_head_data: correcting the head position\n");
	// correcting the head position
	// calculating the vector which need to be added to the current head position
	gsl_vector_view ehrelation = gsl_vector_view_array(position_correction,3);
	gsl_vector *tmp_vector = gsl_vector_calloc(3);
	gsl_matrix *tmp_matrix = gsl_matrix_calloc(3,3);
	gsl_matrix_memcpy(tmp_matrix,&hmatrix.matrix);

	gsl_blas_dgemv(CblasNoTrans,1.0,tmp_matrix,&ehrelation.vector,1.0,tmp_vector);
	//_DEBUG_("correct_head_data: adding the correction vector to the current head position\n");
	// adding the correction vector to the current head position
	gsl_vector_add(&hpos.vector,tmp_vector);

	// correction the orientation matrix by multiplying it with the correction matrix
	gsl_matrix_view cmatrix = gsl_matrix_view_array(orientation_correction,3,3);
	gsl_matrix* tmp_matrix2 = gsl_matrix_calloc(3,3);
	gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,tmp_matrix,&cmatrix.matrix,1.0,tmp_matrix2);
	//gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,&cmatrix.matrix,tmp_matrix,1.0,tmp_matrix2);

	gsl_matrix_memcpy(&hmatrix.matrix,tmp_matrix2);

	gsl_vector_free(tmp_vector);
	gsl_matrix_free(tmp_matrix);
	gsl_matrix_free(tmp_matrix2);

	return 1;
}


void lg_get_fov_ray_from_angles(GAZE_TRACKER *gazetracker,double alpha, double beta, double *origin, double *direction){
	//printf("lg_get_fov_ray_from_angles\n");
	t_raw_head_data rhd;
	//printf("ht_get_tracking_data\n");
	ht_get_tracking_data(gazetracker->headtracker,gazetracker->headtracker->ihead,&rhd);
	//printf("gt_calculate_head_relative_position\n");
	t_head_data hd;
	gt_calculate_head_relative_position(rhd.position,rhd.matrix,gazetracker->hcorrection.headtonasalbone,gazetracker->hcorrection.correct_matrix,hd.position,hd.orientation);
	//printf("hm\n");
	origin[0] = hd.position[0];
	origin[1] = hd.position[1];
	origin[2] = hd.position[2];

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
	direction[0] = gsl_vector_get(gaze_direction,0);
	direction[1] = gsl_vector_get(gaze_direction,1);
	direction[2] = gsl_vector_get(gaze_direction,2);
}




