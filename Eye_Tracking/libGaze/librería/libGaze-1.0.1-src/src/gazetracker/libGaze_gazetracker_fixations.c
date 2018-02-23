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


#include <libGaze_error.h>
#include <libGaze_fixations.h>
#include <gazetracker/libGaze_gazetracker.h>


int gt_do_head_fixation(GAZE_TRACKER* gazetracker, double* pos, int fixdur, int showInstructions){
	_DEBUG_("gt_do_head_fixation: %f\t%f\t%f\t%d\n",pos[0],pos[1],pos[2],fixdur);
	if (showInstructions == 1){
		gt_show_instruction(gazetracker,INSTRUCTION_2,NO_KEY_PRESS);
	}
	//self.showInstruction(2, key = False)
	// writing tag to log files
	//gt_add_log_msg(gazetracker,"Start_HeadFixation: ");
	//self.add_log_msg("Start_HeadFixation: ")
	//int x,y;
	//t_head_fix_data hfd;
	//hfd.x = x;
	//hfd.y = y;
	//t_head_fix_data fd;
	int ireturn = wait_for_head_fixation(gazetracker,
		gazetracker->visualhooks,pos,
		gazetracker->hcorrection.headtonasalbone,
		gazetracker->hcorrection.correct_matrix,fixdur,15.0,50, 10000 );
	//pyGazeFix.head_fixation2(self.display, self.calibobj,self.headtracker,target=cp,hdcorrection =[self.eyeheadrelation,self.correct_matrix],fixdur=fixdur)
	//gt_add_log_msg(gazetracker,"End_HeadFixation: ");
	//self.add_log_msg("End_HeadFixation: ")
	return ireturn;
}


