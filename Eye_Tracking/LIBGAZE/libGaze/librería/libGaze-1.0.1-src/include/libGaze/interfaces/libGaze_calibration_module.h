#ifndef LIBGAZE_CALIBRATION_MODULE_H_
#define LIBGAZE_CALIBRATION_MODULE_H_

#include "../libGaze_defines.h"
#include "../libGaze_types.h"
#include "../calibration/libGaze_calibration_types.h"


/**
 * Initializes the calibration module. This function is going to be called by libGaze when the calibration module is loaded.
 *
 * @param <idebug> int value defining if the module should print debug messages in the fdebug (0=no, 1=yes)
 * @param <ierror>  int value defining if the module should print error messages in the ferror (0=no, 1=yes)
 * @param <fdebug> a file descriptor to the debug output file
 * @param <ferror> a file discriptor to the error output file
 */
int calibration_init_module(int idebug, int ierror, FILE *fdebug, FILE *ferror);

/**
 *
 *
 */
int calibration_fini_module(void);

/**
 * Fits the mapping function used by the the calibration module to calculate eye related viewing angles from raw eye tracking data.
 *
 * @param <eye> which eye should be calibrated
 * @param <cdataset> a pointer to the calibration data set which should be used to calibrate the eye
 */
int calibration_processCalibrationData(enum lg_eyes eye,t_calibration_data_set* cdataset);

/**
 * Converts the raw eye data from an eye tracker to eye related viewing angles using the mapping function of the calibration module.
 *
 * @param <redata> pointer to the raw eye tracking data recieved from an eye tracker
 * @param <edata> pointer to the structure where the calculated eye data should be stored in
 */
int calibration_processEyeData(t_raw_eye_data* redata, t_eye_data* edata);

#endif /*LIBGAZE_CALIBRATION_MODULE_H_*/

