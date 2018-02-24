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

#include "libGaze_error.h"

FILE *lg_debug = NULL;
FILE *lg_error = NULL;

int lg_debug_level = 0;
int lg_error_level = 0;

void _ERROR_(const char *format, ...){
	va_list ap;
	
	if(lg_error_level>0){
		fprintf(lg_error,"ERR LibGaze:\t");
		
		va_start(ap,format);
		vfprintf(lg_error,format,ap);
		va_end(ap);
		fflush(lg_error);
	}
}

void _DEBUG_(const char *format, ...){
	va_list ap;
	if(lg_debug_level>0){
		fprintf(lg_debug,"MSG LibGaze:\t");
	
		va_start(ap,format);
		vfprintf(lg_debug,format,ap);
		va_end(ap);
		fflush(lg_debug);
	}
}


