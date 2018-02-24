/*
 * libGaze_eyetracker_fixation.c
 *
 *  Created on: Nov 6, 2008
 *      Author: racoon
 */

#include <eyetracker/libGaze_eyetracker_fixation.h>
//#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <libGaze_time.h>
#include <libGaze_error.h>
#include <libGaze.h>

typedef struct{
	int time;
	double x;
	double y;
}t_eye_tmp_data;

void calculateDistance(GSList* list, double *avgdist, double *maxdist);
void calculateAVG(GSList* list, t_eye_tmp_data* fix_data);
enum eyetracker_return et_get_fixation_thresholds(EYE_TRACKER* eyetracker, float* threshold, float* deviation){
	if (eyetracker->mode == EYETRACKER_MODE_NORMAL){
		return ((*eyetracker->get_fixation_thresholds))(threshold,deviation);
	}else if (eyetracker->mode == EYETRACKER_MODE_DUMMY){
		*threshold = 200.0;
		*deviation = 50.0;
		return LG_EYETRACKER_OK;
	}
	return LG_EYETRACKER_ERROR;

}

//int et_wait_for_fixation(EYE_TRACKER* eyetracker, t_visualisation_hooks* vhooks, enum lg_eyes eye,t_eye_fix_data *fix_data,int fixdur,int timeout){
int et_wait_for_fixation(EYE_TRACKER* eyetracker, enum lg_eyes eye,t_eye_fix_data *fix_data,int fixdur,int timeout){

	float threshold;
	float deviation;

	et_get_fixation_thresholds(eyetracker,&threshold,&deviation);

	if(eyetracker->mode == EYETRACKER_MODE_NORMAL){
		//checks if eyetracker is even connected
		//_DEBUG_("eyetracker_ptr: %p\n",eyetracker);
		if(et_is_eye_tracker_connected(eyetracker)==LG_EYETRACKER_IS_CONNECTED){

			// variable for the starting time (used to check for a timeout)
			struct timeval t;
			gettimeofday(&t,0);
			unsigned long long stime = ((t.tv_sec)*1000 + ((t.tv_usec)/1000));


			GSList *samples[] = {NULL,NULL};

			//flags if a fixation for the left [0] or right [1] eye was accepted
			int accept[] ={0,0};

			int fixtime[] = {0,0};
			double avgdist[] = {0.0,0.0};
			double maxdist[] = {0.0,0.0};



			enum lg_eyes  aveye;
			if(((*(eyetracker->eye_available))(&aveye))==LG_EYETRACKER_NOT_CONNECTED){
				return LG_EYETRACKER_NOT_CONNECTED;
			}

			//checks if the eyes available by the eyetracker math with the eyes which should do a fixation
			if (eye== LG_EYE_BOTH && aveye != LG_EYE_BOTH){
				_ERROR_("wait_for_fixation: EYE_BOTH is not available\n");
				return -1;
			}else if	(eye== LG_EYE_BOTH && aveye == LG_EYE_BOTH){
				_DEBUG_("wait_for_fixation: EYE_BOTH\n");
				accept[0] = 0;
				accept[1] = 0;
				fix_data->type = LG_EYE_BOTH;
			}

			if (eye== LG_EYE_LEFT && (aveye != LG_EYE_BOTH && aveye!=LG_EYE_LEFT)){
				_ERROR_("wait_for_fixation: EYE_LEFT is not available\n");
				return -1;
			}else if (eye== LG_EYE_LEFT && (aveye == LG_EYE_BOTH || aveye==LG_EYE_LEFT)){
				_DEBUG_("wait_for_fixation: EYE_LEFT\n");
				//accepts an empty fixation for the right eye
				fix_data->type = LG_EYE_LEFT;
				accept[0] = 0;
				accept[1] = 1;
			}
			if (eye== LG_EYE_RIGHT && (aveye != LG_EYE_BOTH && aveye!=LG_EYE_RIGHT)){
				_ERROR_("wait_for_fixation: EYE_RIGHT is not available\n");
				return -1;
			}else if	(eye== LG_EYE_RIGHT && (aveye == LG_EYE_BOTH || aveye ==LG_EYE_RIGHT)){
				_DEBUG_("wait_for_fixation: EYE_RIGHT\n");
				//accepts an empty fixation for the left eye
				fix_data->type = LG_EYE_RIGHT;
				accept[0] = 1;
				accept[1] = 0;
			}

			// Allocate zeroed memory for input one time before the loop
			// this will increase performance and avoids an hang up during
			// the call of (*(vhooks->get_input))(input);
			t_input* input=(t_input*)calloc(1,sizeof(t_input));

			//current time
			unsigned long long ctime;
			while(1){
				gettimeofday(&t,0);
				ctime = ((t.tv_sec)*1000 + ((t.tv_usec)/1000));

				//checks for timeout
				if ((ctime - stime) > timeout){
					_DEBUG_("wait_for_eye_fixation: timeout\n");
					free(input);
					return -1;
				}else{ //if no timeout
					t_raw_eye_data eyedata;
					int tmp;
					MILLISEC_SLEEP(1);
					tmp = et_get_eyetracking_data(eyetracker, &eyedata);
					//_DEBUG_("call et_get_eyetracking_data\n");

					//if the eyetracker returned data (tmp)
					if (tmp==LG_EYETRACKER_NEW_DATA){
						//_DEBUG_("et_get_eyetracking_data: LG_EYETRACKER_NEW_DATA received\n");
						if ((eye == LG_EYE_LEFT) && (eyedata.eye_available == LG_EYE_LEFT)){
							t_eye_tmp_data *efd_left;
							efd_left = (t_eye_tmp_data*)malloc(sizeof(t_eye_tmp_data));

							efd_left->x = eyedata.pupil_x[0];
							efd_left->y = eyedata.pupil_y[0];
							efd_left->time = (int)ctime;
							//_DEBUG_("efd_left: %f\t%f\n",efd_left->x,efd_left->y);

							samples[0] = g_slist_append(samples[0],efd_left);

						}else if((eye == LG_EYE_RIGHT)&& (eyedata.eye_available == LG_EYE_RIGHT)){
							t_eye_tmp_data *efd_right;
							efd_right = (t_eye_tmp_data*)malloc(sizeof(t_eye_tmp_data));
							efd_right->x = eyedata.pupil_x[1];
							efd_right->y = eyedata.pupil_y[1];
							efd_right->time = (int)ctime;
							//_DEBUG_("efd_right: %f\t%f\n",efd_right->x,efd_right->y);
							samples[1] = g_slist_append(samples[1],efd_right);
						}else if(eye == LG_EYE_BOTH){
							if((eyedata.eye_available == LG_EYE_BOTH)|| (eyedata.eye_available == LG_EYE_LEFT)){
								t_eye_tmp_data *efd_left;
								efd_left = (t_eye_tmp_data*)malloc(sizeof(t_eye_tmp_data));

								efd_left->x = eyedata.pupil_x[0];
								efd_left->y = eyedata.pupil_y[0];
								efd_left->time = (int)ctime;
								samples[0] = g_slist_append(samples[0],efd_left);
							}
							if((eyedata.eye_available == LG_EYE_BOTH)|| (eyedata.eye_available == LG_EYE_RIGHT)){
								t_eye_tmp_data *efd_right;
								efd_right = (t_eye_tmp_data*)malloc(sizeof(t_eye_tmp_data));

								efd_right->x = eyedata.pupil_x[1];
								efd_right->y = eyedata.pupil_y[1];
								efd_right->time = (int)ctime;
								samples[1]= g_slist_append(samples[1],efd_right);
							}
						}
					}else{ //If there was no data from the eyetracker
						if (tmp ==0){
							//_DEBUG_("wait_for_eye_fixation: eyedata is old\n");
						}
						//TODO: Delete the samples lists and start from the beginning

					}

					//check the sample lists for fixations
					int i = 0;
					while (i<2){
						//if the sample list for eye i is empty then continue with the next eye
						if (samples[i]==NULL){
							i++;
							continue;
						}


						if(g_slist_length(samples[i]) >1 && accept[i] == 0){

							calculateDistance(samples[i],&avgdist[i], &maxdist[i]);
							if(maxdist[i]>threshold){
								//_DEBUG_("wait_for_eye_fixation: too large dist: %f\n",maxdist[i]);
								void * first = g_slist_nth_data(samples[i],0);
								samples[i] = g_slist_remove(samples[i],first);
								i++;
								continue;
							}

							t_eye_tmp_data* first = (t_eye_tmp_data*)g_slist_nth_data(samples[i],0);
							if (fixdur < (ctime - first->time)){
								_DEBUG_("wait_for_eye_fixation: accept fixation for eye[%d]\n",i);
								t_eye_tmp_data tmp;
								calculateAVG(samples[i],&tmp);
								fix_data->x[i] = tmp.x;
								fix_data->y[i] = tmp.y;
								fix_data->time[i] = tmp.time;
								_DEBUG_("wait_for_eye_fixation: x: %f\ty: %f\n",tmp.x,tmp.y);
								accept[i]=1;
							}

						}
						i++;
					}

					if (accept[0]==1 && accept[1]==1){
						_DEBUG_("wait_for_eye_fixation: real fixation\n",i);
						_DEBUG_("wait_for_eye_fixation: left: %f\t%f\tright: %f\t%f\n",fix_data->x[0],fix_data->y[0],fix_data->x[1],fix_data->y[1]);
						g_slist_free(samples[0]);
						g_slist_free(samples[1]);
						free(input);
						return LG_OK;
					}

					//:TODO implement visualisation hooks for acept and abort
					// checking for input data
					input->type = LG_INPUT_NONE;
//					(*(vhooks->get_input))(vhooks->vptr,input);
/*
					if (input->type == LG_INPUT_ACCEPT){
						_DEBUG_("wait_for_eye_fixation: accepted fixation %d\n",i);
						t_eye_tmp_data tmp;
						calculateAVG(samples[0],&tmp);
						fix_data->x[0] = tmp.x;
						fix_data->y[0] = tmp.y;
						fix_data->time[0] = tmp.time;
						calculateAVG(samples[1],&tmp);
						fix_data->x[1] = tmp.x;
						fix_data->y[1] = tmp.y;
						fix_data->time[1] = tmp.time;
						g_slist_free(samples[0]);
						g_slist_free(samples[1]);
						free(input);
						return LG_OK;
					}else if (input->type == LG_INPUT_ABORT){
							g_slist_free(samples[0]);
							g_slist_free(samples[1]);
							free(input);
							return LG_ABORT;
					}else if (input->type == LG_INPUT_QUIT){
						g_slist_free(samples[0]);
						g_slist_free(samples[1]);
						free(input);
						return LG_QUIT;
					}
*/
				}
			}
		}else{
			_ERROR_("wait_for_fixation: eyetracker is not connected: %d\n",et_is_eye_tracker_connected(eyetracker));
		}
	}else{
		MILLISEC_SLEEP(fixdur);
		return LG_OK;
	}
	return LG_QUIT;
}

void calculateAVG(GSList* list, t_eye_tmp_data* fix_data){
	GSList *tmp;
	tmp = list;

	int count =0;
	double x = 0.0;
	double y=0.0;

	while (tmp != NULL){
		t_eye_tmp_data *ptr = (t_eye_tmp_data*)tmp->data;
		x += ptr->x;
		y += ptr->y;
		count++;
		tmp = g_slist_next(tmp);
	}
	if (count >0){
		x = x/ (double)count;
		y = y / (double)count;
	}
	fix_data->x = x;
	fix_data->y = y;

}
void calculateDistance(GSList* list, double *avgdist, double *maxdist){
	GSList *tmp;
	tmp = list;

	int count =0;
	*avgdist=0,0;
	*maxdist = 0.0;
	double x = 0.0;
	double y = 0.0;

	if (tmp!= NULL){
		x = ((t_eye_tmp_data*)tmp->data)->x;
		y = ((t_eye_tmp_data*)tmp->data)->y;
	}

	while (tmp != NULL){
		t_eye_tmp_data *ptr = (t_eye_tmp_data*)tmp->data;

		double dist = sqrt(pow((ptr->x-x),2)+pow((ptr->y-y),2));
		*avgdist += dist;
		if (dist> *maxdist){
			*maxdist = dist;
		}
		count++;
		tmp = g_slist_next(tmp);
	}
	if (count >0){
		*avgdist = *avgdist / (double)count;
	}


}
