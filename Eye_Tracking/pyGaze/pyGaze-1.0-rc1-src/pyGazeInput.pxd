cdef extern from "libGaze/libGaze_input_types.h":
	ctypedef enum lg_input_types:
		LG_INPUT_NONE,
		LG_INPUT_QUIT,
		LG_INPUT_ACCEPT,
		LG_INPUT_ABORT	,	
		LG_INPUT_KEY,	
		LG_INPUT_MESSAGE 	
	
	ctypedef struct t_input:
		int type
		int key
		int mod
		char *msg
		
	
