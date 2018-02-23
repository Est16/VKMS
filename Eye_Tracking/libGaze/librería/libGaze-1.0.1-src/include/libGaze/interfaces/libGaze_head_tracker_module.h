#ifndef LIBGAZE_HEAD_TRACKER_MODULE_H_
#define LIBGAZE_HEAD_TRACKER_MODULE_H_
#include "../headtracker/libGaze_headtracker_defines.h"

/*
 * returns:
 * 	 0
 * 	-1
 */
enum headtracker_return init_head_tracker_module(int idebug, int ierror, FILE *fdebug, FILE *ferror);
/*
 * returns:
 * 	 0
 * 	-1
 */
enum headtracker_return fini_head_tracker_module(void);


enum headtracker_return get_tracker_frequency(int* frequency);
/*
 * Enables a connection to the headtracker.
 * returns:
 * 	 0 if it could establish a connection and
 * 	-1 if a problem occurred.
 */
enum headtracker_return connect_to_head_tracker(void);
/*
 *  Disconnects the headtracker.
 *
 * returns:
 * 	 0 if it could disable the connection and
 *	-1 if a problem occurred.
 */
enum headtracker_return disconnect_head_tracker(void);
/*
 * Checks if the headtracker is connected or not.
 *
 * retruns:
 * 	1 if the headtracker is connected
 * 	0 if the headtracker is not connected
 */
enum headtracker_return is_head_tracker_connected(void);

/*
 * Configures the headtracker. the first void* pointer
 * shows to the configuration data (for example a file name)
 * and the second parameter describes the display wich will be used.
 *
 * returns:
 * 	 0 if it could configure the headtracker and
 * 	-1 if a problem occurred.
 */
enum headtracker_return configure_head_tracker(char *filename, int *ihead, int *ieye,int* num);
/*
 * Gets the current tracking-data from the headtracker and
 * saves it in the variable described by the t_gaze_data* pointer.
 *
 * retruns:
 * 	 0 if no new dataset is availible
 * 	 1 if a new dataset was availible and stored under t_gaze_data*
 * 	-1 if an error occurred.
 */
enum headtracker_return get_head_tracking_data(t_raw_head_data* hd);
/*
 * Starts the head-tracking. If logging is enabled and a log-file is open
 * all tracking data will be stored in this logfile.
 *
 * retruns:
 * 	 0 if tracking could be enabled
 * 	-1 if an error ocurred
 */
enum headtracker_return start_head_tracking();
/*
 * Pauses the head-tracking. From this time on
 * no more data will be written into the log-file
 * until the tracking will be continued.
 *
 * retruns:
 * 	 0 if the tracking was paused successfully
 * 	-1 if an error ocurred
 */

enum headtracker_return stop_head_tracking(void);


#endif /*LIBGAZE_HEAD_TRACKER_MODULE_H_*/
