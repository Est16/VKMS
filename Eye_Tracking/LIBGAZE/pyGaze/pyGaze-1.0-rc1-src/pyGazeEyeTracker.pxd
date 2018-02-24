cdef extern from "libGaze/eyetracker/libGaze_eyetracker.h":


	ctypedef enum eyetracker_modes:
		EYETRACKER_MODE_NORMAL
		EYETRACKER_MODE_DUMMY


	ctypedef enum eyetracker_return:
		LG_EYETRACKER_OK,
		LG_EYETRACKER_ERROR,
		LG_EYETRACKER_COULD_NOT_FIND_MODULE,
		LG_EYETRACKER_WRONG_MODULE,
		LG_EYETRACKER_COULD_NOT_LOAD_MODULE,
		LG_EYETRACKER_COULD_NOT_START_TRACKING,
		LG_EYETRACKER_IS_NOT_TRACKING,		
		LG_EYETRACKER_NEW_DATA,
		LG_EYETRACKER_NO_NEW_DATA_AVAILABLE,
		LG_EYETRACKER_NO_DATA_AVAILABLE,		
		LG_EYETRACKER_REQUESTED_EYE_NOT_TRACKED,
		LG_EYETRACKER_COULD_NOT_CONNECT,	
		LG_EYETRACKER_NOT_CONNECTED,
		LG_EYETRACKER_IS_CONNECTED,
		LG_EYETRACKER_CFG_FILE_FORMAT_ERROR,		
		LG_EYETRACKER_COULD_NOT_FIND_CFG_FILE,
		LG_EYETRACKER_IS_NOT_CONFIGURED,
		LG_EYETRACKER_NO_LOG_FILE_OPEN,					
		LG_EYETRACKER_COULD_NOT_OPEN_LOG_FILE,
		LG_EYETRACKER_COULD_NOT_CLOSE_LOG_FILE,
		LG_EYETRACKER_COULD_NOT_ADD_LOG_MSG
		
		
	ctypedef struct t_eyetracker	
		
	ctypedef struct t_raw_eye_data:
		unsigned long long time
		int eye_available
		double pupil_x[2]
		double pupil_y[2]
		double pupil_size[2]
		
cdef extern from "libGaze/eyetracker/libGaze_eyetracker_fixation.h":
	ctypedef struct t_eye_fix_data:
		int time[2]
		int type
		double x[2]
		double y[2]

	
	ctypedef struct t_eyetracker	
	
	t_eyetracker* et_load_module(char* module,int debug, int error, int mode)

	int et_unload_module(t_eyetracker* et)
	
	int et_connect_to_eye_tracker(t_eyetracker* et)
	
	int et_disconnect_eye_tracker(t_eyetracker* et)
	
	int et_configure_eye_tracker(t_eyetracker* et, char* cfg)
	
	int et_get_tracker_frequency(t_eyetracker* et, int* frequency)
	
	int et_start_eye_tracking(t_eyetracker* et)
	
	int et_stop_eye_tracking(t_eyetracker* et)
	
	int et_get_eyetracking_data(t_eyetracker* et, t_raw_eye_data* edata)
	
	int et_is_eye_tracker_connected(t_eyetracker* et)
	
	int et_open_log_file(t_eyetracker* et, char* file, char* path)
	
	int et_close_log_file(t_eyetracker* et)
	
	int et_start_logging(t_eyetracker* et)
	
	int et_stop_logging(t_eyetracker* et)
	
	int et_add_log_msg(t_eyetracker* et, char* msg)
	
	int et_wait_for_fixation(t_eyetracker* et,  int eye,t_eye_fix_data *fix_data,int fixdur,int timeout)
	
	int et_is_eye_tracker_connected(t_eyetracker* et)
	
	int et_eye_available(t_eyetracker* et, int *eye)
	