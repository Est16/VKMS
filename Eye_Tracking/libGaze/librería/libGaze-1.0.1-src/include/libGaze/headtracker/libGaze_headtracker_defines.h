#ifndef LIBGAZE_HEADTRACKER_DEFINES_H_
#define LIBGAZE_HEADTRACKER_DEFINES_H_


enum headtracker_return{
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
};


#endif /*LIBGAZE_HEADTRACKER_DEFINES_H_*/
