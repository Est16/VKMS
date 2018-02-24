#ifndef LIBGAZE_GAZETRACKER_DEFINES_H_
#define LIBGAZE_GAZETRACKER_DEFINES_H_

#define INSTRUCTION_0 "Calibration: please fixate the targets without moving your head."
#define INSTRUCTION_1 "Validation: please fixate the targets without moving your head."
#define INSTRUCTION_2 "Head fixation: please rotate your head, so that the small circle is inside the big circle."
#define INSTRUCTION_3 "Eye head relation: please position the eye pointer."
#define INSTRUCTION_4 "Eye head relation: you can remove the eye pointer now."
#define INSTRUCTION_5 "Driftcorrection"
#define INSTRUCTION_6 "Could not collect Calibration points. CHECK CAMERAS"

#define KEY_PRESS 1
#define NO_KEY_PRESS 0

enum gt_modes{
	GT_MODE_BOTH=0,
	GT_MODE_EYE,
	GT_MODE_HEAD,
	GT_MODE_DUMMY
};

enum gt_return{
	LG_GT_OK = 0,
	LG_GT_ERROR,
	LG_GT_QUIT,
	LG_GT_ACCEPT,
	LG_GT_ABORT,
	//calibration returns
	LG_GT_COULD_NOT_COLLECT_ALL_CALIB_POINTS,
	LG_GT_NOT_CALIBRATED,
	LG_GT_NEW_GAZE_DATA,
	LG_GT_NO_NEW_GAZE_DATA
};

#endif /*LIBGAZE_GAZETRACKER_DEFINES_H_*/
