#ifndef LIBGAZE_EYE_MODULE_H_
#define LIBGAZE_EYE_MODULE_H_

#include "../libGaze_defines.h"
#include "../eyetracker/libGaze_eyetracker_defines.h"

enum eyetracker_return init_eye_tracker_module(int, int ,FILE*,FILE*);

enum eyetracker_return fini_eye_tracker_module(void);

/**
 * Enables a connection to the eye tracker.
*/
/*
 * returns:
 *		LG_EYETRACKER_OK if it could establish a connection and
 *		LG_EYETRACKER_IS_NOT_CONFIGURED if the eyetracker was not configured before
 * 		LG_EYETRACKER_COULD_NOT_CONNECT if it could not connect to the eye tracker.
 */
enum eyetracker_return connect_to_eye_tracker(void);

/**
 *  Closes the connection to the eye tracker.
 *
 *//*
 * @return
 *		LG_EYETRACKER_OK if it could disable the connection and
 *		LG_EYETRACKER_ERROR if a problem occurred.
 */
enum eyetracker_return disconnect_eye_tracker(void);

/**
 * Checks if the eye tracker is connected or not.
 *
 * @return
 * <ul>
*		\li
 * 		\li LG_EYETRACKER_IS_CONNECTED if the eyetracker is connected
 * 		\li LG_EYETRACKER_NOT_CONNECTED if the eyetracker is not connected
 * 	</ul>
 */
enum eyetracker_return is_eye_tracker_connected(void);

/**
 * Configures the eye tracker using a configuration file.

 @param <cfg> a char* pointer to the name of the configuration file (path + filename)
 */
 /* returns:
 *		LG_EYETRACKER_OK if it could configure the eyetracker and
 *		LG_EYETRACKER_CFG_FILE_FORMAT_ERROR if the format of the config file was wrong.
 * 		LG_EYETRACKER_COULD_NOT_FIND_CFG_FILE if the config file could not be found.
 */
enum eyetracker_return configure_eye_tracker(void* cfg);

/**
 * Gets the current tracking-data from the eye tracker and
 * saves it in the variable described by the t_gaze_data* pointer.
 *
 *@param <redata> a (t_raw_eye_data*) pointer to the structure in which the the new eye tracking data should be stored
 * @return <ul>
 * \li
 * 	\li	LG_EYETRACKER_NO_DATA_AVAILABLE if no data at all was availible
 * 	\li	LG_EYETRACKER_NO_NEW_DATA_AVAILABLE if no new data was availible.
 * 	\li	LG_EYETRACKER_NEW_DATA  if a new data was availible and stored under t_gaze_data*.
 * 	\li	LG_EYETRACKER_ERROR  if an error occurred.
 * </ul>
 */
enum eyetracker_return get_eyetracking_data(t_raw_eye_data* redata);

/**
 * Starts the eye-tracking.
 *
 * @retrun
 * 		LG_EYETRACKER_OK if tracking could be started.
 * 		LG_EYETRACKER_COULD_NOT_START_TRACKING if the tracking could not be started.
 */
enum eyetracker_return start_eye_tracking();


/**
 * Stops the eye-tracking.
 *
 * retruns:
 * 		LG_EYETRACKER_OK if the tracking was successfully stopped.
 *		LG_EYETRACKER_ERROR if an error ocurred
 */
enum eyetracker_return stop_eye_tracking(void);

/**
 * Tells the user which eye can be tracked. The result will be saved under "int *eye" it can be: LG_EYE_BOTH, LG_EYE_LEFT, LG_EYE_RIGHT or LG_EYE_NONE
 *
 * @param <eye> a pointer to a lg_eyes variable where the id of the available eye will be stored.
 *			<ul>
 *			\li
 *			\li LG_EYE_BOTH
 *			\li LG_EYE_LEFT
 *			\li LG_EYE_RIGHT
 *			\li LG_EYE_NONE
 *			</ul>
 *
 *	@return
 *	<ul>
 *		\li
 *		\li LG_EYETRACKER_OK
 * 		\li LG_EYETRACKER_ERROR
 * 	</ul>
 */
enum eyetracker_return eye_available(enum lg_eyes* eye);

/**
 * Gets the frequency of the eye tracker in samples per second.
 *
 * @param <frequency> a point the an int variable where the frequency should be stored.
 *	@return
 *	<ul>
 *		\li
 *		\li LG_EYETRACKER_OK
 * 		\li LG_EYETRACKER_ERROR
 * 	</ul>
 */
enum eyetracker_return get_tracker_frequency(int* frequency);

/**
 * Gets the threshold values for the eye fixation algorithm from the eye tracker. These thresholds are in eye tracker camera image pixel units.
 * These values can differ between different eye tracker model.
 *
 *  @param <threshold> a pointer to a float variable where the fixation threshold should be stored.
 *  @param <threshold> a pointer to a float variable where the fixation deviation should be stored.
 */
enum eyetracker_return get_fixation_thresholds(float* threshold, float* deviation);

/*
enum eyetracker_return add_log_msg(char*);

enum eyetracker_return open_log_file(char*,char*);

enum eyetracker_return close_log_file();

enum eyetracker_return start_logging();
enum eyetracker_return stop_logging();

int add_draw_cal_target(void*,void (void*,int, int));

int add_erase_cal_target(void*, void (void*));

int start_eye_tracker_calibration(void);

int get_camera_image(int,t_image*);

int enable_image_streaming_mode(void);

int disable_image_streaming_mode(void);

int send_input(t_input);

*/

#endif /*LIBGAZE_EYE_MODULE_H_*/
