cimport pyGazeHeadTracker
cimport pyGazeEyeTracker

cdef extern from "libGaze/libGaze.h":
	ctypedef struct t_gaze_data:
		unsigned long time
		double origin[3]
		double direction[3]	
		double dangles[2]
		double matrix[9]
	
	ctypedef struct t_data:
		int eye_available
		int head_available
		int dis_x[2]
		int dis_y[2]
		double alpha[2]
		double beta[2]
		double gpos[2][3]
		pyGazeEyeTracker.t_raw_eye_data eye
		pyGazeHeadTracker.t_raw_head_data head
		t_gaze_data gaze[2]
	
	ctypedef struct t_eye_data:
		unsigned long long time
		int eye_available
		double alpha[2]
		double beta[2]
		double size[2]
	
	ctypedef struct t_head_data:
		unsigned long long time
		double position[3]
		double orientation[9]
	
	ctypedef struct t_gaze_data_set:
		unsigned long long time
		int gaze_available
		t_head_data head
		t_eye_data eye
		t_gaze_data gaze[2]