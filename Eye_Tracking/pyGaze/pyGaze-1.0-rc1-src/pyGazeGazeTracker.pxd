cimport pyGaze
cimport pyGazeHeadTracker
cimport pyGazeEyeTracker
cimport pyGazeDisplay
cimport pyGazeVisualisationHooks	

cdef extern from "libGaze/gazetracker/libGaze_gazetracker.h":
	

	ctypedef enum gt_modes:
		GT_MODE_BOTH=0,
		GT_MODE_EYE,
		GT_MODE_HEAD,
		GT_MODE_DUMMY

	ctypedef enum gt_return:
		LG_GT_OK = 0,
		LG_GT_ERROR,
		LG_GT_QUIT,
		LG_GT_ACCEPT,
		LG_GT_ABORT,
		#calibration returns
		LG_GT_COULD_NOT_COLLECT_ALL_CALIB_POINTS
	
	
	ctypedef struct t_validation_data:
		int flag
		#int x
		#int y
		#int cx[2]
		#int cy[2]
		double fov_alpha
		double fov_beta
		double alpha[2]
		double beta[2]
		double calpha[2]
		double cbeta[2]
		double cdrift[2]
		pyGaze.t_gaze_data_set gaze
		
	ctypedef struct t_validation_data_set:
		int num
		int bad
		int good
		double avg_drift[2]
		double max_drift[2]
		double avg_drift_alpha[2]
		double avg_drift_beta[2]
		t_validation_data *vdata	
	
	ctypedef struct t_ray:
		double origin[3]
		double direction[3]
	
	ctypedef struct t_gazetracker
	
	t_gazetracker* gt_create_eyeheadtracker(int debug, int error)


	pyGazeHeadTracker.t_headtracker* gt_get_headtracker(t_gazetracker* eht)
	pyGazeEyeTracker.t_eyetracker* gt_get_eyetracker(t_gazetracker* gt)
	int gt_load_modules(t_gazetracker* gt, char* eyemod ,char* headmod,  int mode)
	int gt_load_calibration_module(t_gazetracker* gt, char* cmod)
	int gt_configure(t_gazetracker* gt, char* eyecfg ,char* headcfg)
	int gt_connect(t_gazetracker* gt)
	int gt_disconnect(t_gazetracker* gt)
	int gt_start_tracking(t_gazetracker* gt)
	int gt_stop_tracking(t_gazetracker* gt)
	int gt_start_logging(t_gazetracker* gt)
	int gt_stop_logging(t_gazetracker* gt)
	int gt_open_log_file(t_gazetracker* gt, char* file, char* path, int flag)
	int gt_close_log_file(t_gazetracker* gt)
	int gt_add_log_msg(t_gazetracker* gt, char* msg)
	int gt_log_event(t_gazetracker* gt, char* event)
	int gt_save_calibration_data(t_gazetracker* gt, char* calibFile)
	int gt_set_visual_hooks(t_gazetracker* gt, pyGazeVisualisationHooks.t_visualisation_hooks *vhooks)
	int gt_load_calibration_data(t_gazetracker* gt, char* calibFile)
	int gt_collect_eyehead_relation(t_gazetracker* gt)
	int gt_correct_head_direction_vector(t_gazetracker* gt, double alpha, double beta)
	int gt_calibrate(t_gazetracker* gt, double alpha, double beta, int n, int m, double* center)
	int gt_validate_calibration(t_gazetracker* gt, t_validation_data_set* vdata, double alpha, double beta, int n, int m, double* center)
	int gt_display_last_validation(t_gazetracker* gt)
	int gt_drift_check(t_gazetracker* gt, t_validation_data_set* vdataset, double* center, double alpha, double beta, int n, int m, int count)
	int gt_do_head_fixation(t_gazetracker* gt, double* center, int fixdur, int showInstruction)
	int gt_show_instruction(t_gazetracker* gt, char* instruction, int keypress)
	int gt_get_current_gaze_position(t_gazetracker* gt, pyGaze.t_gaze_data_set *gazeDataSet)
	int gt_get_gaze_with_alpha_beta(t_gazetracker* gt, double alpha, double beta, pyGaze.t_gaze_data_set *gazeDataSet)
	void gt_get_ray_relative_to_head_orientation(t_gazetracker *gt,double alpha, double beta, t_ray *ray)