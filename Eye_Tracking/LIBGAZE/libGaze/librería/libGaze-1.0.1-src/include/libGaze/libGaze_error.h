#ifndef LIBGAZE_ERROR_H_
#define LIBGAZE_ERROR_H_

/*
#define _LG_OK											 0
#define _LG_NOT_CONNECTED				-1
#define _LG_ALLREADY_CONNECTED		-2
#define _LG_MODULE_NOT_FOUND			-3
#define _LG_INCOMPATIBLE_MODULE		-4

#define _LG_MODULE_ERROR					-23
*/

#include <stdio.h>


extern FILE *lg_debug;
extern FILE *lg_error;

extern int lg_debug_level;
extern int lg_error_level;

void _ERROR_(const char *format, ...) 
#if defined(__GNUC__)
		__attribute__((format(printf,1,2)))
#endif
;

void _DEBUG_(const char *format, ...) 
#if defined(__GNUC__)
		__attribute__((format(printf,1,2)))
#endif
;


#endif /*LIBGAZE_ERROR_H_*/


