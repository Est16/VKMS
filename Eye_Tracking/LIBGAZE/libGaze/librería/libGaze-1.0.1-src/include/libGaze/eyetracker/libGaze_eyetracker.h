#ifndef LIBGAZE_EYETRACKER_H_
#define LIBGAZE_EYETRACKER_H_

#include "../libGaze_types.h"

/*
 *
 */
EYE_TRACKER *et_load_module(char*,int ,int, enum eyetracker_modes);

/**
 * @fn enum eyetracker_return et_unload_module(EYE_TRACKER*)
 * @brief brief description
 * @return returns what ever
 * Used to initialize the libGaze, so that it is ready to be used.
 */
enum eyetracker_return et_unload_module(EYE_TRACKER*);

enum eyetracker_return et_connect_to_eye_tracker(EYE_TRACKER*);

enum eyetracker_return et_disconnect_eye_tracker(EYE_TRACKER*);

enum eyetracker_return et_configure_eye_tracker(EYE_TRACKER*,char*);

enum eyetracker_return et_get_tracker_frequency(EYE_TRACKER*,int*);

//int et_add_draw_cal_target(EYE_TRACKER*,void*,void (void*,int, int));

//int et_add_erase_cal_target(EYE_TRACKER*,void*,void (void*));

enum eyetracker_return et_get_eyetracking_data(EYE_TRACKER*,t_raw_eye_data*);

enum eyetracker_return et_start_eye_tracking(EYE_TRACKER*);

enum eyetracker_return et_stop_eye_tracking(EYE_TRACKER*);

//int et_start_eye_tracker_calibration(EYE_TRACKER*);

enum eyetracker_return et_open_log_file(EYE_TRACKER*,char*,char*);

enum eyetracker_return et_close_log_file(EYE_TRACKER*);

enum eyetracker_return et_start_logging(EYE_TRACKER*);

enum eyetracker_return et_stop_logging(EYE_TRACKER*);

enum eyetracker_return et_add_log_msg(EYE_TRACKER*,char*);

//int et_go_to_setup(EYE_TRACKER*);

enum eyetracker_return et_is_eye_tracker_connected(EYE_TRACKER*);

enum eyetracker_return et_eye_available(EYE_TRACKER*,enum lg_eyes*);

//int et_get_camera_image(EYE_TRACKER*, int ,t_image*);

//int et_enable_image_streaming_mode(EYE_TRACKER*);

//int et_disable_image_streaming_mode(EYE_TRACKER*);

//int et_send_input(EYE_TRACKER*,t_input);


#endif /*LIBGAZE_EYETRACKER_H_*/
