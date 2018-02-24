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
#ifndef EYE_LINK_2_ERROR_H_
#define EYE_LINK_2_ERROR_H_



#include <stdio.h>
extern FILE *el2_debug;
extern FILE *el2_error;

extern int eyedebug;
extern int eyeerror;

extern void _ERROR_(const char *format, ...) __attribute__((format(printf,1,2)));


extern void _DEBUG_(const char *format, ...) __attribute__((format(printf,1,2)));

#endif /*EYE_LINK_2_ERROR_H_*/


