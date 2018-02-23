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
#include <stdio.h>
#include <stdarg.h>

#include "eyelink_2_error.h"

FILE *el2_debug = NULL;
FILE *el2_error = NULL;

int eyedebug = 0;
int eyeerror = 0;


void _ERROR_(const char *format, ...){
	
	if (eyeerror){
		if(el2_error!=NULL){
			va_list ap;
			
			fprintf(el2_error,"ERROR_Eye_Link_2:\t");
			
			va_start(ap,format);
			vfprintf(el2_error,format,ap);
			va_end(ap);
		}
	}
}

void _DEBUG_(const char *format, ...){
	if (eyedebug){
		if(el2_debug != NULL){
			va_list ap;
		
			fprintf(el2_debug,"Eye_Link_2:\t");
		
			va_start(ap,format);
			vfprintf(el2_debug,format,ap);
			va_end(ap);	
		}
	}
}

