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
#include <stdio.h>


#include "libGaze.h"

struct timeval *log_base_time = NULL;
short int __is_init = 0;

int init_LibGaze(int dlevel, int elevel){

	//lg_debug = fopen("debug.txt","w");
	//lg_error = lg_debug;

	lg_debug = stdout;
	lg_error = stderr;

	lg_debug_level =dlevel ;
	lg_error_level = elevel;
	_DEBUG_("init_LibGaze\n");
	log_base_time = (struct timeval*)malloc(sizeof(struct timeval));
	gettimeofday(log_base_time,0);

	return 1;
}
/**
 * This function is used to unload libGaze.
 */
int fin_LibGaze(GAZE_TRACKER* gazetracker){
	_DEBUG_("fin_LibGaze\n");

	if (gazetracker!=NULL)
	{
		if (gazetracker->eyetracker!=NULL)
		{
			_DEBUG_("et_unload_module\n");
			et_unload_module(gazetracker->eyetracker);
		}
		if (gazetracker->headtracker!=NULL)
		{
			_DEBUG_("ht_unload_module\n");
			ht_unload_module(gazetracker->headtracker);
		}
	}

	//fclose(lg_debug);
	//fclose(lg_error);
//	free(lg_display);
	return 1;
}

unsigned long long lg_get_time(){
	struct timeval t;
	gettimeofday(&t,0);

	unsigned long long lot = ((t.tv_sec - log_base_time->tv_sec)*1000 + (t.tv_usec - log_base_time->tv_usec)/1000);
	return lot;
}


