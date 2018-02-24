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
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

#include "libGaze.h"
#include "libGaze_defines.h"
#include "libGaze_fixations.h"
#include "libGaze_error.h"
#include "libGaze_time.h"
#include "headtracker/libGaze_headtracker.h"
//#include "display/libGaze_display.h"
#include "tools/libGaze_tools.h"


void calculateHeadDistance(GSList* list, double *avgdist, double *maxdist);
void calculateHeadAVG(GSList* list, t_head_fix_data* fix_data);

int wait_for_head_fixation(GAZE_TRACKER* gazetracker,
													 t_visualisation_hooks* vhooks, double* target ,
													 double* position_correction,
													 double* orientation_correction,
													 int fixdur,
													 float deviation,
													 float threshold,
													 int timeout){
	//printf("wait_for_head_fixation: %f\t%f\%f\n",target[0],target[1],target[2]);
	if(gazetracker->mode == GT_MODE_HEAD||gazetracker->mode == GT_MODE_BOTH||gazetracker->mode == GT_MODE_DUMMY){
		//checks if headtracker is even connected
		if(ht_is_head_tracker_connected(gazetracker->headtracker)==LG_HEADTRACKER_IS_CONNECTED){

			// variable for the starting time (used to check for a timeout)
			unsigned long long stime = lg_get_time();

			GSList *samples = NULL;

			double avgdist = 0.0;
			double maxdist = 0.0;
			int accept = 0;
			t_head_fix_data fix_data;
			//t_head_fix_data *fix_data = (t_head_fix_data*)malloc(sizeof(t_eye_fix_data));

			// Allocate zeroed memory for input one time before the loop
			// this will increase performance and avoids an hang up during
			// the call of (*(vhooks->get_input))(input);
			t_input* input=(t_input*)calloc(1,sizeof(t_input));
			int ht_freq;
			ht_get_tracker_frequency(gazetracker->headtracker,&ht_freq);
			//current time
			//printf("ht_freq: %d\n",ht_freq);
			unsigned long long ctime;
			while(1){

				ctime = lg_get_time();

				//checks for timeout
				if ((ctime - stime) > timeout){
					_DEBUG_("wait_for_head_fixation: timeout\n");
					free(input);
					return LG_TIMEOUT;
				}else{ //if no timeout
					t_head_fix_data* hfd;
					hfd = (t_head_fix_data*)calloc(1,sizeof(t_head_fix_data));
					//t_raw_head_data headdata;
					hfd->time = ctime;
					//int tmp;

					MILLISEC_SLEEP(1000/(2*ht_freq));
					gt_get_ray_relative_to_head_orientation(gazetracker,0.0,0.0,&hfd->ray);

					double target_direction[3];
					target_direction[0] = target[0] - hfd->ray.origin[0];
					target_direction[1] = target[1] - hfd->ray.origin[1];
					target_direction[2] = target[2] - hfd->ray.origin[2];

					double len = sqrt(pow(target_direction[0],2)+pow(target_direction[1],2)+pow(target_direction[2],2));
					target_direction[0] /=len;
					target_direction[1] /=len;
					target_direction[2] /=len;

					double skalar = hfd->ray.direction[0]*target_direction[0] +hfd->ray.direction[1]*target_direction[1]+hfd->ray.direction[2]*target_direction[2];
					double len2 = sqrt(pow(target_direction[0],2)+pow(target_direction[1],2)+pow(target_direction[2],2));
					double len3 = sqrt(pow(hfd->ray.direction[0],2)+pow(hfd->ray.direction[1],2)+pow(hfd->ray.direction[2],2));

					hfd->angle = lg_tools_degrees(acos(skalar/(len2*len3)));
					t_ray target_ray;
					target_ray.origin[0] = hfd->ray.origin[0];
					target_ray.origin[1] = hfd->ray.origin[1];
					target_ray.origin[2] = hfd->ray.origin[2];
					target_ray.direction[0] = target_direction[0];
					target_ray.direction[1] = target_direction[1];
					target_ray.direction[2] = target_direction[2];
					//printf("hfd->angle: %f\n",hfd->angle);
					samples= g_slist_append(samples,hfd);
					v_erase_screen(vhooks);
					v_draw_head_orientation_target(vhooks,&target_ray,(int)(2*deviation));
					v_draw_head_orientation(vhooks,&hfd->ray,(int)(deviation));
					v_update_screen(vhooks);
				}
				//check the sample list for fixations
				//if the sample list is empty the n continue

				if(g_slist_length(samples) >1 && accept == 0){

					calculateHeadDistance(samples,&avgdist, &maxdist);
					//_DEBUG_("maxdist: %f\tthreshold:%f\n",maxdist,threshold);
					//printf("avgdist: %f\tmaxdist: %f\n",avgdist,maxdist);
					if(maxdist>threshold){
						_DEBUG_("wait_for_head_fixation: too large dist: %f\n",maxdist);
						void * first = g_slist_nth_data(samples,0);
						samples = g_slist_remove(samples,first);
						continue;
					}

					t_head_fix_data* first = (t_head_fix_data*)g_slist_nth_data(samples,0);
					if (fixdur < (ctime - first->time)){
						_DEBUG_("wait_for_head_fixation: accept fixation for head\n");
						//calculateAVG(samples,fixdata);
						calculateHeadAVG(samples,&fix_data);
						accept=1;
					}
				}

				if (accept==1 ){
					_DEBUG_("wait_for_head_fixation: real fixation\n");
					_DEBUG_("head: %f\n",fix_data.angle);
					g_slist_free(samples);
					free(input);
					return LG_OK;
				}

				//:TODO implement visualisation hooks for acept and abort
				input->type = LG_INPUT_NONE;
				//(*(vhooks->get_input))(vhooks->vptr,input);
				v_get_input(vhooks,input);
				if (input->type == LG_INPUT_ACCEPT){
					_DEBUG_("wait_for_head_fixation: accepted fixation\n");
					calculateHeadAVG(samples,&fix_data);
					free(input);
					g_slist_free(samples);
					return LG_OK;
				}else if (input->type == LG_INPUT_ABORT){
					_DEBUG_("wait_for_head_fixation: abortet fixation\n");
					g_slist_free(samples);
					free(input);
					return LG_ABORT;
				}else if (input->type == LG_INPUT_QUIT){
					_DEBUG_("wait_for_head_fixation: QUIT\n");
					g_slist_free(samples);
					free(input);
					return LG_QUIT;
				}
			}

		}else{
			_ERROR_("wait_for_head_fixation: headtracker is not connected: %d\n",ht_is_head_tracker_connected(gazetracker->headtracker));
		}
	}else{
		v_erase_screen(vhooks);
		t_ray ray;

		gt_get_ray_relative_to_head_orientation(gazetracker,0.0,0.0,&ray);

		ray.direction[0] = target[0]-ray.origin[0];
		ray.direction[1] = target[1]-ray.origin[1];
		ray.direction[2] = target[2]-ray.origin[2];

		v_draw_head_orientation_target(vhooks,&ray,(int)(2*deviation));
		v_draw_head_orientation(vhooks,&ray,(int)(deviation));
		v_update_screen(vhooks);
		SEC_SLEEP(2);
	}

return LG_OK;
}

void calculateHeadAVG(GSList* list, t_head_fix_data* fix_data){
	GSList *tmp;
	tmp = list;

	int count =0;
	double angle = 0.0;


	while (tmp != NULL){
		t_head_fix_data *ptr = (t_head_fix_data*)tmp->data;
		angle += ptr->angle;
		count++;
		tmp = g_slist_next(tmp);
	}
	if (count >0){
		angle /=(double)count;
	}
	fix_data->angle = angle;

}
void calculateHeadDistance(GSList* list, double *avgdist, double *maxdist){
	GSList *tmp;
	tmp = list;

	int count =0;
	*avgdist=0.0;
	*maxdist = 0.0;


	while (tmp != NULL){
		t_head_fix_data *ptr = (t_head_fix_data*)tmp->data;

		//double dist = sqrt(pow((double)(ptr->x-x),2)+pow((double)(ptr->y-y),2));
		*avgdist += ptr->angle;
		if (ptr->angle> *maxdist){
			*maxdist = ptr->angle;
		}
		count++;
		tmp = g_slist_next(tmp);
	}
	if (count >0){
		*avgdist = *avgdist / (double)count;
	}

}


