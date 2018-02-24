#ifndef LIBGAZE_API_H_
#define LIBGAZE_API_H_

//The __declspec directives are omitted, because under MSVC we now use 
//an export definition file libgaze/lib/libgaze.def, because the
//API is nearly complete (so we have to change this file not often)
//and when compiling with /CLR flag, __declspec directives are not allowed
//under MinGW and Linux __declspec is not needed because all functions are
//defined extern by default.
//
//#if defined __WIN32__ || defined WIN32
//	#ifdef LIBGAZE_EXPORTS
//		#define LIBGAZE_API __declspec(dllexport)
//	#else
//		#define LIBGAZE_API __declspec(dllimport)
//	#endif
//#else
	#define LIBGAZE_API 
//#endif

#endif /*LIBGAZE_API_H_*/
