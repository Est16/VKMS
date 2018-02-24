cdef extern from "libGaze/headtracker/libGaze_headtracker.h":

	ctypedef enum headtracker_modes:
		HEADTRACKER_MODE_NORMAL
		HEADTRACKER_MODE_DUMMY

	ctypedef enum headtracker_return:
		LG_HEADTRACKER_OK = 0,
		LG_HEADTRACKER_ERROR,
		LG_HEADTRACKER_COULD_NOT_FIND_MODULE,
		LG_HEADTRACKER_WRONG_MODULE,
		LG_HEADTRACKER_COULD_NOT_LOAD_MODULE,
		
		LG_HEADTRACKER_COULD_NOT_START_TRACKING,
		LG_HEADTRACKER_IS_NOT_TRACKING,		
	
		LG_HEADTRACKER_NEW_DATA,
		LG_HEADTRACKER_NO_NEW_DATA_AVAILABLE,
		LG_HEADTRACKER_NO_DATA_AVAILABLE,		
		LG_HEADTRACKER_REQUESTED_EYE_NOT_TRACKED,
	
		LG_HEADTRACKER_COULD_NOT_CONNECT,	
		LG_HEADTRACKER_NOT_CONNECTED,
		LG_HEADTRACKER_IS_CONNECTED,
	
		LG_HEADTRACKER_CFG_FILE_FORMAT_ERROR,		
		LG_HEADTRACKER_COULD_NOT_FIND_CFG_FILE,
		LG_HEADTRACKER_IS_NOT_CONFIGURED,
	
		LG_HEADTRACKER_NO_LOG_FILE_OPEN,					
		LG_HEADTRACKER_COULD_NOT_OPEN_LOG_FILE,
		LG_HEADTRACKER_COULD_NOT_CLOSE_LOG_FILE,
		LG_HEADTRACKER_COULD_NOT_ADD_LOG_MSG
		
		
	ctypedef struct t_headtracker	
	
	ctypedef struct t_raw_head_data:
		unsigned long time
		double position[3]
		double matrix[9]
		int button
		
		
	t_headtracker* ht_load_module(char* module,int debug, int error, int mode)

	int ht_unload_module(t_headtracker* ht)
	
	int ht_connect_to_head_tracker(t_headtracker* ht)
	
	int ht_disconnect_head_tracker(t_headtracker* ht)
	
	int ht_configure_head_tracker(t_headtracker* ht, char* cfg)
	
	int ht_start_head_tracking(t_headtracker* ht)
	
	int ht_stop_head_tracking(t_headtracker* ht)
	
	int ht_get_tracking_data(t_headtracker* ht, int id, t_raw_head_data* hdata)
	
	int ht_is_head_tracker_connected(t_headtracker* ht)
	
	int ht_open_log_file(t_headtracker* ht, char* file, char* path)
	
	int ht_close_log_file(t_headtracker* ht)
	
	int ht_start_logging(t_headtracker* ht)
	
	int ht_stop_logging(t_headtracker* ht)
	
	int ht_add_log_msg(t_headtracker* ht, char* msg)

		