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
#ifndef EYE_LINK_2_LOGGING_H_
#define EYE_LINK_2_LOGGING_H_

#include <sys/time.h>
extern char *log_file_name;
extern char *log_file_path;

extern struct timeval *log_base_time;

extern int open_log;
extern int enable_logging;

#endif /*EYE_LINK_2_LOGGING_H_*/
