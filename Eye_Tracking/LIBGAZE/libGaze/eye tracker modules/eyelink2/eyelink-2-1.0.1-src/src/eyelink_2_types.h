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
#ifndef EYE_LINK_2_TYPES_H_
#define EYE_LINK_2_TYPES_H_

#include <libGaze/libGaze_types.h>


typedef struct{
	short int left;
	short int right;
	short int fixation;
	short int fixupdate;
	short int saccade;
	short int blink;
	short int message;
	short int button;
	short int input;
	short int pupil;
}t_event_filter;

typedef struct{
	short int left;
	short int right;
	short int gaze;
	short int gazeres;
	short int href;
	short int area;
	//short int velocity;
	short int status;
	//short int fixavg;
	//short int nostart;
	short int button;
	short int input;
	short int pupil;
}t_sample_data;


typedef struct{
	short int gaze;
	short int gazeres;
	short int href;
	short int area;
	short int velocity;
	short int status;
	short int fixavg;
	short int nostart;
}t_event_data;


typedef struct{
	/*tracker inforamtion*/
	char *et_ip;
	char *node_name;
	int short is_eyelink2;
	/*display inforamtion*/
	int screen_x;
	int screen_y;
	int screen_w;
	int screen_h;

	/*tracker configuration*/
	char *calibration_type;		/*"H3","HV3","HV5","HV9"*/
	short int binocular_enabled;	/*YES or NO*/
	short int head_subsample_rate;	/*0 -> normal, 4 -> antirefelction on, -1 -> turn off makers*/
	short int heuristic_filter;		/*0 -> OFF, 1 -> moderate filtering (delay 1 sample rate),
										 2 extra level of filtering 2 sample rates delay*/

	int simulate_head_camera;		/*YES or NO*/
	int simulate_screen_distance;	/*distance in mm*/
	int parser_configuration;		/*just el2 0 standard, 1 high sensitivity*/

	//tracker frequency
	int frequency;

	// Fixation thresholds
	float fix_threshold;
	float fix_deviation;

	int edfEnabled;
	char *edfFileName;

	t_event_filter *file_event_filter;
	t_event_data *file_event_data;
	t_sample_data *file_sample_data;


	t_event_filter *link_event_filter;
	t_event_data *link_event_data;
	t_sample_data *link_sample_data;

}t_eye_tracker_config;

#endif /*EYE_LINK_2_TYPES_H_*/
