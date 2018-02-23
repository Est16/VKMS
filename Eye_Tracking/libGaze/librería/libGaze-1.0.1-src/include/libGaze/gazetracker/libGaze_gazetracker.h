#ifndef LIBGAZE_GAZETRACKER_H_
#define LIBGAZE_GAZETRACKER_H_

#include "../libGaze_api.h"
#include "libGaze_gazetracker_types.h"
#include "libGaze_gazetracker_defines.h"
#include "libGaze_gazetracker_logging.h"
//extern char **eht_instructions;

LIBGAZE_API void gt_update_vhooks(GAZE_TRACKER* eyeheadtracker, t_visualisation_hooks* hooks);

LIBGAZE_API GAZE_TRACKER* gt_create_eyeheadtracker(int debug, int error);

HEAD_TRACKER* gt_get_headtracker(GAZE_TRACKER*);

EYE_TRACKER* gt_get_eyetracker(GAZE_TRACKER*);

int gt_get_tracked_eye(GAZE_TRACKER*);

LIBGAZE_API int gt_load_modules(GAZE_TRACKER*,char*,char*,enum gt_modes);

LIBGAZE_API int gt_load_calibration_module(GAZE_TRACKER*, char*);

LIBGAZE_API int gt_configure(GAZE_TRACKER*, char*,char*);

LIBGAZE_API int gt_connect(GAZE_TRACKER*);

LIBGAZE_API int gt_disconnect(GAZE_TRACKER*);

LIBGAZE_API int gt_start_tracking(GAZE_TRACKER*);

LIBGAZE_API int gt_stop_tracking(GAZE_TRACKER*);

LIBGAZE_API int gt_start_logging(GAZE_TRACKER*);

LIBGAZE_API int gt_stop_logging(GAZE_TRACKER*);

LIBGAZE_API int gt_open_log_file(GAZE_TRACKER*,char*,char*, int);

LIBGAZE_API int gt_close_log_file(GAZE_TRACKER*);

LIBGAZE_API int gt_add_log_msg(GAZE_TRACKER*,char*);

LIBGAZE_API int gt_save_calibration_data(GAZE_TRACKER*,char*);

LIBGAZE_API int gt_set_visual_hooks(GAZE_TRACKER*,t_visualisation_hooks*);

int gt_calculate_head_relative_position(double* headpos, double* headorient, double* position_correction, double* orientation_correction, double *eye_position, double *eye_orientation);

LIBGAZE_API int gt_load_calibration_data(GAZE_TRACKER*,char*);

LIBGAZE_API int gt_collect_eyehead_relation(GAZE_TRACKER*);

LIBGAZE_API int gt_correct_head_direction_vector(GAZE_TRACKER*,double,double);

LIBGAZE_API int gt_calibrate(GAZE_TRACKER*, double,double,int,int,double*);

t_grid *gt_generate_grid(GAZE_TRACKER*,int,int,double,double);

int gt_select_n_from_grid(t_grid*, int ,t_grid *);

int gt_show_grid(GAZE_TRACKER* ,int,int,double,double,double*, t_calibration_data_set*);

LIBGAZE_API int gt_do_head_fixation(GAZE_TRACKER*, double*, int, int);

int gt_show_instruction(GAZE_TRACKER*, char*,int);

int gt_collect_data_for_grid(GAZE_TRACKER*, t_grid*, t_calibration_data_set*,int);

LIBGAZE_API int gt_get_current_gaze_position(GAZE_TRACKER*,t_gaze_data_set*);

LIBGAZE_API int gt_get_current_gaze_position_from_raw_eye_head_data(GAZE_TRACKER* eyeheadtracker, t_raw_eye_data *red, t_raw_head_data *rhd,t_gaze_data_set* gds);

LIBGAZE_API int gt_get_current_gaze_position_from_eye_head_data(GAZE_TRACKER* eyeheadtracker, t_eye_data *ed, t_head_data *hd,t_gaze_data_set* gds);

LIBGAZE_API int gt_validate_calibration(GAZE_TRACKER* ,t_validation_data_set*, double,double,int,int,double*);

LIBGAZE_API int gt_get_gaze_with_alpha_beta(GAZE_TRACKER* gazetracker,double alpha, double beta, t_gaze_data_set *gds);

LIBGAZE_API int gt_drift_check(GAZE_TRACKER*,t_validation_data_set*,double*,double,double,int,int,int);

LIBGAZE_API int gt_display_last_validation(GAZE_TRACKER*);

LIBGAZE_API void gt_get_ray_relative_to_head_orientation(GAZE_TRACKER *gazetracker,double alpha, double beta, t_ray* ray);

int gt_wait_for_fixation(void);

int gt_write_grid_data_to_file(void);


#endif /*LIBGAZE_GAZETRACKER_H_*/
