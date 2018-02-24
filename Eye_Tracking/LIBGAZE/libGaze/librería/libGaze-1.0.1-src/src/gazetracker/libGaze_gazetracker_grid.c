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

#include <libGaze_defines.h>
#include <libGaze_types.h>
#include <libGaze_error.h>
#include <libGaze_fixations.h>
#include <libGaze_time.h>

#include <gazetracker/libGaze_gazetracker.h>
#include <gazetracker/libGaze_gazetracker_types.h>
//#include <display/libGaze_display.h>
#include <headtracker/libGaze_headtracker.h>
#include <eyetracker/libGaze_eyetracker.h>
#include <eyetracker/libGaze_eyetracker_fixation.h>
#include <tools/libGaze_tools.h>
#include <visualisation/libGaze_visualisation.h>


t_grid *gt_generate_grid(GAZE_TRACKER* gazetracker, int n,int m ,double alpha ,double beta){
		//_DEBUG_("gt_generate_grid: n= %d\tm= %d\talpha= %f\tbeta= %f\n",n,m,alpha,beta);
		// generates a grid of points (x y display coordinates) with the size
				// of n times m and the visual field of ax times ay (in degree)

				int i = 0;
				int j = 0;

				t_grid *grid;
				grid = (t_grid*)malloc(sizeof(t_grid));

				grid->range_alpha = alpha;
				grid->range_beta = beta;

			grid->alphas = (double*)calloc(n*m,sizeof(double));
			grid->betas = (double*)calloc(n*m,sizeof(double));

			grid->n=n;
			grid->m=m;
			grid->num = n*m;

				grid->center_alpha = 0.0;
				grid->center_beta = 0.0;

				// calculate the size of each steps in degree
				double step_x = alpha/((double)(n-1));
				double step_y = beta/((double)(m-1));
	//			_DEBUG_("gt_generate_grid: step_x= %f\tstep_y= %f\n",step_x,step_y);
		//_DEBUG_("gt_generate_grid: start generating the grid\n");
				i=0;
				// start generating the grid

				for (i=0;i<n;i++){
						for(j=0;j<m;j++){
								// calculating the vertical and horicontal anfle of the grid point
								double x = ((double)i)*step_x;
								double y = ((double)j)*step_y;
								grid->alphas[(i*m)+j]=x-(alpha/2.0);
								grid->betas[(i*m)+j]=y-(beta/2.0);
								//_DEBUG_("gt_generate_grid: x= %f\ty= %f\n",(x-(alpha/2.0)),(y-(beta/2.0)));

						}
				}


				// return the center of the fov and the grid of display coordinates
				return grid;

}

int gt_select_n_from_grid(t_grid* grid, int n,t_grid *ngrid){

	if(n<=(grid->n*grid->m)){
		ngrid->alphas = (double*)calloc(n,sizeof(double));
		ngrid->betas = (double*)calloc(n,sizeof(double));
		ngrid->num = n;
		ngrid->center_alpha = grid->center_alpha;
		ngrid->center_beta = grid->center_beta;
		ngrid->range_alpha = grid->range_alpha;
		ngrid->range_beta = grid->range_beta;

		//int done[grid->num]; //Initialze dynamic sized arrays not allowed in MSVC
		int* done=(int*)calloc(grid->num,sizeof(int));

		int i;
		int count = 0;
		while (count<n){
			i = (rand()%(grid->num));
			if(done[i]==0){
				ngrid->alphas[count] = grid->alphas[i];
				ngrid->betas[count] = grid->betas[i];
				done[i]=1;
				count++;
			}
		}
		free(done);

		return LG_OK;
	}else{
		return LG_ERROR;
	}

}

int gt_show_grid(GAZE_TRACKER* gazetracker,int n,int m ,double alpha ,double beta, double* pos, t_calibration_data_set *cdataset){
	// fixates the orientation of the head to the direction of the
	// point describes by center
	int ireturn = LG_OK;
	if (pos != NULL){
		ireturn=	gt_do_head_fixation(gazetracker,pos,2000,0);
	}
	if(ireturn != LG_QUIT){

		// generates the grid of display points from the curren position and
		// orientation considering the given angles for the field of view
		t_grid* grid;
		grid = gt_generate_grid(gazetracker,n,m,alpha,beta);


		ireturn = gt_collect_data_for_grid(gazetracker,grid, cdataset, 0);
		free(grid->alphas);
		free(grid->betas);
		free(grid);
		return ireturn;
	}else{
		_DEBUG_("gt_show_grid: LG_QUIT\n");
		return LG_QUIT;
	}
}


int gt_collect_data_for_grid(GAZE_TRACKER* gazetracker ,t_grid* grid, t_calibration_data_set *cdataset, int center){
	// collects eye and head data for a given grid of display coordinates
	// the points will be presented in random order. If data for one point could not
	// be collected it retries the procedure 'tries' times. The parameter offset
	// discribes the display offset wich can be presented (if a point lies)
	// outside this area no data will be collected.

	int tries = 3;
		//_DEBUG_("gt_collect_data_for_grid\n");

		_DEBUG_("gt_collect_data_for_grid: grid->num: %d\n",grid->num);
		_DEBUG_("gt_collect_data_for_grid: cdataset->num: %d\n",cdataset->num);
		// init the done list with zeros (needed to find out if data for a specific
		// point was allready collected (0 = False, 1 = True))
		_DEBUG_("n: %d\n",grid->num);
		cdataset->num = grid->num+(2*center);
		int *done;
		done = (int*)calloc(cdataset->num,sizeof(int));

		//checks if a fixation point at the center of the fov should be added at the
		//begining and end of collecing process
		if (center==1){
			done[cdataset->num-1] = 1;
			done[cdataset->num-2] = 1;
		}

		// counts for how many points data has been collected
		int count = 0;

		// init list for the collected data sets
		_DEBUG_("num: %d\n",cdataset->num);
		cdataset->cdata = (t_calibration_data*)malloc((cdataset->num)*sizeof(t_calibration_data));
		t_calibration_data* cdataptr = cdataset->cdata;
		cdataset->bad = 0;
		cdataset->outside = 0;
		// resets the the old fixation value (needed for the antifixation)
		t_eye_fix_data oldfix;
		oldfix.x[0] = 0.0;
		oldfix.y[0] = 0.0;
		oldfix.x[1] = 0.0;
		oldfix.y[1] = 0.0;

		// the index for the next point
		int i = 0;

		// initialize the randomizer
		srand(randtime(NULL));
		int random = 1;

		// collects data for the grid points until data is present for each point
		//while (count < (grid->n +(2*center))){
		while (count < cdataset->num){
		//_DEBUG_("n: %d\n",grid->n);
				// searches for the next point which should be presented

			if (center==1 && count ==0){
					i = grid->num;
				}else if(center==1 && count == grid->num+1){
					i = grid->num+1;
				}else if (random==1){
						int found = 0;
						//_DEBUG_("searching for next point\n");

						while (found ==0){
							//generating a randoized number from 0 to n-1
								i = (rand()%(grid->num));
								//_DEBUG_("done[%d]: %d\n",i,done[i]);
								if (done[i]!=1){
										//_DEBUG_("next point: %d\n",i);
										found = 1;
								}
								else{
										//_DEBUG_("no candidate: %d\n",i);
								}
						}
				// itterative aproch if random is disabled
				}else{
						i = count;
				}

				_DEBUG_("Calibration point i: %d\n",i);
				//int x;
				//int y;
				double alpha;
				double beta;
				if (center == 1 && (i == grid->num || i == (grid->num+1))){
					alpha = grid->center_alpha;
					beta = grid->center_beta;
				}else{
					alpha = grid->alphas[i];
					beta = grid->betas[i];
				}

				double ctarget_pos[3];
				t_ray fov_ray;
				gt_get_ray_relative_to_head_orientation(gazetracker,alpha,beta,&fov_ray);
				// checks if the display coordinates of the point are in the range discribed by the osset values
				// TODO think about implementing offset
				v_erase_screen(gazetracker->visualhooks);
				int dreturn =v_draw_cal_target(gazetracker->visualhooks,&fov_ray,ctarget_pos);
				v_update_screen(gazetracker->visualhooks);
				//if((x>0 && x<gazetracker->display->width)&& (y>0 && y<gazetracker->display->height)){
				if(dreturn==0){
						//_DEBUG_("selected point is inside display area: x: %d\ty: %d\n",x,y);
						// presents the point on the display
						int fix;
						// waits for an	fixation (if the was no fixation before it waits afwait seconds)
						_DEBUG_(" waits for an	fixation");
						if(gazetracker->mode != GT_MODE_DUMMY){
							if (count > 0){
							fix = wait_for_anti_eye_fixation(gazetracker->eyetracker,
								gazetracker->visualhooks,
								gazetracker->used_eye,	&oldfix,200,5000);
								MILLISEC_SLEEP(400);
								if (fix==LG_ABORT){
										return LG_ABORT;
								}else if (fix==LG_QUIT){
										return LG_QUIT;
								}
							}else{
								SEC_SLEEP(1);
							}
						}
						// wait for an eye fixation

						t_eye_fix_data fixdata;
						if(gazetracker->eyetracker->mode == EYETRACKER_MODE_NORMAL){
							fix =et_wait_for_fixation(gazetracker->eyetracker,
															gazetracker->used_eye,
															&fixdata,500,5000);


						}else if(gazetracker->eyetracker->mode == EYETRACKER_MODE_DUMMY){
							fixdata.x[0]=alpha;
							fixdata.y[0]=beta;
							fixdata.x[1]=alpha;
							fixdata.y[1]=beta;
							fixdata.type = LG_EYE_BOTH;
							MILLISEC_SLEEP(200);
							fix = LG_OK;
						}


						// checks if the fixation procedure was canceled
						// a cancelation of the fixation procedure causes a canelation of the
						// data collection
						if (fix==LG_ABORT){
								return LG_ABORT;
						}else if (fix==LG_QUIT){
								return LG_QUIT;
						}
						// if an eye fixation was detected the fixation data and current head position
						// will be saved in the datalist
						if (fix == LG_OK){
							_DEBUG_("fix %d: True\n",i);
								// sets the flag in the done list to one, so that the system knows that data was collected for the point 'i'
								done[i] = 1;
								// increases the counter
								count ++;

								cdataptr[i].fov_alpha = alpha;
								cdataptr[i].fov_beta = beta;

								//TODO
								cdataptr[i].ctarget_pos[0]  = ctarget_pos[0];
								cdataptr[i].ctarget_pos[1]  = ctarget_pos[1];
								cdataptr[i].ctarget_pos[2]  = ctarget_pos[2];

								int ireturn;
								ireturn = ht_get_tracking_data(gazetracker->headtracker,gazetracker->headtracker->ihead,&(cdataptr[i].head));

								cdataptr[i].eye.eye_available = (enum lg_eyes)fixdata.type;
								if (fixdata.type == LG_EYE_BOTH || fixdata.type == LG_EYE_LEFT){
									cdataptr[i].eye.pupil_x[0] = fixdata.x[0];
									cdataptr[i].eye.pupil_y[0] = fixdata.y[0];

									oldfix.x[0] = fixdata.x[0];
									oldfix.y[0] = fixdata.y[0];
								}

								if (fixdata.type == LG_EYE_BOTH || fixdata.type == LG_EYE_RIGHT){
									cdataptr[i].eye.pupil_x[1] = fixdata.x[1];
									cdataptr[i].eye.pupil_y[1] = fixdata.y[1];
									oldfix.x[1] = fixdata.x[1];
									oldfix.y[1] = fixdata.y[1];
								}
								cdataptr[i].flag = LG_CDATA_GOOD;

						}else{
						// if no fixation is detected
								// checks if there are any tries left
								// if no more tries are left
								if (done[i]==1-tries){
										// sets the flag in the done list to one, so that the system knows that data was collected for the point 'i'
										done[i] = 1;
										// increases the counter
										count +=1;
										//_DEBUG_("fix 0: False\n");

										cdataptr[i].fov_alpha = alpha;
										cdataptr[i].fov_beta = beta;
										cdataset->bad++;
										cdataptr[i].flag = LG_CDATA_BAD;
								}else{
										done[i]--;
								}
						}

				// if the point is outside the area discribed by the offset values the
				// collected data will be none

				}else{
					 //_DEBUG_("selected point is outside display area: x: %d\ty: %d\n",x,y);
					done[i] = 1;
					// increases the counter
					count +=1;
					cdataptr[i].flag = LG_CDATA_OUT_OF_DISPLAY;
					cdataset->outside++;
				}

				//Checks if collection was aborted
				t_input input;
				v_get_input(gazetracker->visualhooks,&input);
				if(input.type == LG_INPUT_ABORT){
					return LG_ABORT;
				}else if (input.type == LG_INPUT_QUIT){
					return LG_QUIT;
				}


		}

		free(done);
		return LG_OK;
}


//int gt_get_gaze_with_alpha_beta(GAZE_TRACKER* gazetracker,double alpha, double beta, int* x, int* y){
//				//_DEBUG_("gt_get_gaze_with_alpha_beta: alpha= %f\tbeta= %f\n",alpha,beta);
//				t_raw_head_data hd;
//				ht_get_tracking_data(gazetracker->headtracker,gazetracker->headtracker->ihead,&hd);
//
//				//_DEBUG_("hd: %f\t%f\t%f\n%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",hd.position[0],hd.position[1],hd.position[2],hd.matrix[0],hd.matrix[1],hd.matrix[2],hd.matrix[3],hd.matrix[4],hd.matrix[5],hd.matrix[6],hd.matrix[7],hd.matrix[8]);
//
//				lg_tools_correct_head_data(&hd,gazetracker->hcorrection.headtonasalbone,gazetracker->hcorrection.correct_matrix);
//
//				gsl_vector_view	hpos = gsl_vector_view_array(hd.position,3);
//
//				gsl_matrix_view hmatrix = gsl_matrix_view_array(hd.matrix,3,3);
//
//			 	gsl_vector *cdirect;
//				cdirect = lg_tools_get3DVectorFromAngles(lg_tools_radians(alpha), lg_tools_radians(beta));
//
//				//printf("cdirect (%f , %f)= %f\t %f\t %f\n",alpha,beta,cdirect->data[0],cdirect->data[1],cdirect->data[2]);
//				double *alphabeta = lg_tools_getYZAnglesFrom3DVector(cdirect);
//				//printf("alpha: %f\tbeta: %f\n",alphabeta[0],alphabeta[1]);
//				//_DEBUG_("cdirect= %f\t %f\t %f\n",cdirect->data[0],cdirect->data[1],cdirect->data[2]);
//
//				// correcting the direction of the tracked object because it is not on the x axis
//				// TODO: set direction on the x axis
//				/*
//				gsl_matrix *crmatrix;
//				crmatrix = gsl_matrix_calloc(3,3);
//				matrix_rotationXYZ(crmatrix,'y',radians(90));
//				gsl_vector *cdirect2;
//				cdirect2 = gsl_vector_calloc(3);
//				gsl_blas_dgemv(CblasNoTrans,1.0,crmatrix,cdirect,1.0,cdirect2);
//				*/
//				gsl_vector *cdirect2 = cdirect;
//
//				//_DEBUG_("cdirect2= %f\t %f\t %f\n",cdirect2->data[0],cdirect2->data[1],cdirect2->data[2]);
//
//				gsl_vector_view ehr_vector;
//				ehr_vector = gsl_vector_view_array(gazetracker->hcorrection.headtonasalbone,3);
//
//		gsl_vector* tmp_vector = gsl_vector_calloc(3);
//				gsl_matrix* tmp_matrix = gsl_matrix_calloc(3,3);
//				gsl_matrix_memcpy(tmp_matrix,&hmatrix.matrix);
//
//		gsl_vector *cdirect3;
//				cdirect3 = gsl_vector_calloc(3);
//		gsl_blas_dgemv(CblasNoTrans,1.0,tmp_matrix,cdirect2,1.0,cdirect3);
//
//		double * cipos;
////				cipos = d_getIntersectionWithScreen(gazetracker->display,tmp_vector->data,cdirect3->data);
//		//printf("cdirect3= %f\t %f\t %f\n",cdirect3->data[0],cdirect3->data[1],cdirect3->data[2]);
//		cipos = d_getIntersectionWithScreen(gazetracker->display,hd.position,cdirect3->data);
//				//_DEBUG_("cipos: %f\t%f\t%f\n",cipos[0],cipos[1],cipos[2]);
//				int *cxy;
//				cxy= d_getXYPositionFrom3D(gazetracker->display,cipos);
//
//				*x = cxy[0];
//
//				*y = cxy[1];
//				//_DEBUG_("x: %d\ty: %d\n",*x,*y);
//			 	free(cipos);
//			 	free(cxy);
//
//			 	gsl_vector_free(cdirect);
//			 	//gsl_vector_free(cdirect2);
//			 	gsl_vector_free(cdirect3);
//			 	gsl_vector_free(tmp_vector);
//			 	//gsl_matrix_free(crmatrix);
//			 	gsl_matrix_free(tmp_matrix);
//				return 1;
//}


