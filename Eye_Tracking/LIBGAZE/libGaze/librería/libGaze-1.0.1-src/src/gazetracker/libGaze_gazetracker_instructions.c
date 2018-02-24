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


#include <libGaze_defines.h>
#include <libGaze_time.h>
#include <libGaze_error.h>
#include <visualisation/libGaze_visualisation.h>
#include <gazetracker/libGaze_gazetracker.h>


int gt_show_instruction(GAZE_TRACKER* gazetracker, char* instruction,int key){
	_DEBUG_("gt_show_instruction: %s\n",instruction);
	int timeout=3;

	v_clear_input(gazetracker->visualhooks);

	v_erase_screen(gazetracker->visualhooks);
	v_show_message(gazetracker->visualhooks,instruction);
	v_update_screen(gazetracker->visualhooks);

	int quit=0;
	if (key==KEY_PRESS){
		int keypressed = 0;
		t_input input;
		while (keypressed == 0){

			v_get_input(gazetracker->visualhooks, &input);
			//_DEBUG_("gt_show_instruction: type: %d\n",input.type);
					if (input.type == LG_INPUT_ABORT){
				quit = LG_ABORT;
							keypressed = 1;
					}else if(input.type == LG_INPUT_ACCEPT){
						quit =LG_ACCEPT;
							keypressed = 1;
					}else if(input.type == LG_INPUT_QUIT){
						quit =LG_QUIT;
							keypressed = 1;
					}
					MILLISEC_SLEEP(5);
		}
		}else{
			SEC_SLEEP(timeout);
	}
	v_erase_screen(gazetracker->visualhooks);
	v_update_screen(gazetracker->visualhooks);

	return quit;
}


