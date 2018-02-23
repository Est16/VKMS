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
#include <stdlib.h>
#include "libGaze_defines.h"
#include "libGaze_fixations.h"
#include "libGaze_error.h"
#include "libGaze_time.h"
#include "eyetracker/libGaze_eyetracker.h"

int wait_for_anti_eye_fixation(EYE_TRACKER* eyetracker, t_visualisation_hooks* vhooks,enum lg_eyes eye,t_eye_fix_data* target,float threshold,int timeout){
	if(eyetracker->mode == EYETRACKER_MODE_NORMAL){
		//checks if eyetracker is even connected
		if(et_is_eye_tracker_connected(eyetracker)==LG_EYETRACKER_IS_CONNECTED){

			enum lg_eyes aveye;
			if(((*(eyetracker->eye_available))(&aveye))==LG_EYETRACKER_NOT_CONNECTED){
				return LG_EYETRACKER_NOT_CONNECTED;
			}

			//checks if the eyes available by the eyetracker math with the eyes which should do a fixation
			if (eye== LG_EYE_BOTH && aveye != LG_EYE_BOTH){
				_ERROR_("wait_for_fixation: EYE_BOTH is not available\n");
				return -1;
			}

			if (eye== LG_EYE_LEFT && (aveye != LG_EYE_BOTH && aveye!=LG_EYE_LEFT)){
				_ERROR_("wait_for_fixation: EYE_LEFT is not available\n");
				return -1;
			}
			if (eye== LG_EYE_RIGHT && (aveye != LG_EYE_BOTH && aveye!=LG_EYE_RIGHT)){
				_ERROR_("wait_for_fixation: EYE_RIGHT is not available\n");
				return -1;
			}

			//flags if a fixation for the left [0] or right [1] eye was accepted
			int accept[] ={0,0};
			if(aveye == LG_EYE_LEFT){
				accept[1] = 1;
			}else if(aveye == LG_EYE_RIGHT){
				accept[0] = 1;
			}

			t_eye_fix_data gaze_pos;

			double dist[] = {0.0,0.0};

			// variable for the starting time (used to check for a timeout)
			struct timeval t;
			gettimeofday(&t,0);
			unsigned long long stime = ((t.tv_sec)*1000 + ((t.tv_usec)/1000));

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
					_DEBUG_("wait_for_anti_eye_fixation: timeout\n");
					free(input);
					return -1;
				}else{ //if no timeout
					t_raw_eye_data eyedata;
					int tmp;
					MILLISEC_SLEEP(1);
					tmp = et_get_eyetracking_data(eyetracker, &eyedata);

					//if the eyetracker returned data (tmp)
					if (tmp==LG_EYETRACKER_NEW_DATA){
						if (eye == LG_EYE_LEFT){

							gaze_pos.x[0] = eyedata.pupil_x[0];
							gaze_pos.y[0] = eyedata.pupil_y[0];
							gaze_pos.time[0] = (int)ctime;
							//_DEBUG_("eyedata[0]: %f\t%f\n",gaze_pos.x[0],gaze_pos.y[0]);


						}else if(eye == LG_EYE_RIGHT){
							gaze_pos.x[1] = eyedata.pupil_x[1];
							gaze_pos.y[1] = eyedata.pupil_y[1];
							gaze_pos.time[1] = (int)ctime;
							//_DEBUG_("gaze_pos[1]: %f\t%f\n",gaze_pos.x[1],gaze_pos.y[1]);
						}else if(eye == LG_EYE_BOTH){
							gaze_pos.x[0] = eyedata.pupil_x[0];
							gaze_pos.y[0] = eyedata.pupil_y[0];
							gaze_pos.time[0] = (int)ctime;
							//_DEBUG_("gaze_pos[0]: %f\t%f\n",gaze_pos.x[0],gaze_pos.y[0]);

							gaze_pos.x[1] = eyedata.pupil_x[1];
							gaze_pos.y[1] = eyedata.pupil_y[1];
							gaze_pos.time[1] = (int)ctime;
							//_DEBUG_("eyedata[1]: %f\t%f\n",gaze_pos.x[1],gaze_pos.y[1]);
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
						if (accept[i]==1){
							i++;
							continue;
						}else{

							dist[i] = sqrt(pow((gaze_pos.x[i]-target->x[i]),2)+pow((gaze_pos.y[i]-target->y[i]),2));
							if(dist[i]>threshold){
								_DEBUG_("wait_for_anti_eye_fixation: accept antifixation: %f\n",dist[i]);
								accept[i]=1;
								i++;
								continue;
							}
							i++;
						}

					}

					if (accept[0]==1 && accept[1]==1){
						_DEBUG_("wait_for_anti_eye_fixation: real antifixation\n",i);
						free(input);
						return 1;
					}

					//:TODO implement visualisation hooks for acept and abort

					// checking for input data
					input->type = LG_INPUT_NONE;
					(*(vhooks->get_input))(vhooks->vptr,input);

					if (input->type == LG_INPUT_ACCEPT){
						_DEBUG_("wait_for_anti_eye_fixation: accepted antifixation\n");
						free(input);
						return LG_OK;
					}else if (input->type == LG_INPUT_ABORT){
						_DEBUG_("wait_for_anti_eye_fixation: abortet antifixation\n");
						free(input);
						return LG_ABORT;
					}else if (input->type == LG_INPUT_QUIT){
						_DEBUG_("wait_for_anti_eye_fixation: QUIT\n");
						free(input);
						return LG_QUIT;
					}


				}

			}

		}else{
			_ERROR_("wait_for_anti_eye_fixation: eyetracker is not connected: %d\n",et_is_eye_tracker_connected(eyetracker));
		}
	}else{
		return LG_OK;
	}
	return LG_QUIT;
}



