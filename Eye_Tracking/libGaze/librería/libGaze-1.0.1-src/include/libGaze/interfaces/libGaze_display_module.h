#ifndef LIBGAZE_DISPLAY_MODULE_H_
#define LIBGAZE_DISPLAY_MODULE_H_

/**
 * Initializes the display module. This function is going to be called by libGaze when the display module is loaded.
 *
 * @param <idebug> int value defining if the module should print debug messages in the fdebug (0=no, 1=yes)
 * @param <ierror>  int value defining if the module should print error messages in the ferror (0=no, 1=yes)
 * @param <fdebug> a file descriptor to the debug output file
 * @param <ferror> a file discriptor to the error output file
 */
int display_init_module(int, int ,FILE*,FILE*);

int display_fini_module(void);


/**
 * Configures the display module using a configuration file.
 *
 * @param <cfgfile> the path to the configuration file used to configure the display
 * @param <width> a pointer to an int, where the horizontal resolution of the display in pixel should be stored
 * @param <height> a pointer to an int, where the vertical resolution of the display in pixel should be stored
 */
int display_configure(char* cfgfile,int* width,int* height);

/**
 * Calculates the 3D position of a given x,y display coordinate.
 *
 * @param <xy> pointer to an int array containing the x,y display coordinates of the display in pixel
 * @return the 3D position in the world coordinate system of the given x,y display coordinate
 */
double* display_get3DPositionFromXY(int*xy);

/**
 *  Returns the x,y display coordinates for a given 3D intersection point with the display in the world coordinate system.
 *
 * @param <pos> a 3D position describing a point on the display plane
 * @return a pointer to an int array containing the x,y display coordinates of the display
 */
int* display_getXYPositionFrom3D( double* pos);

/**
 * Calculates an intersection point with the display and a given 3D ray represented through an origin 3D position and a 3D direction vector in the world coordinate system.
 *
 * @param <origin> pointer to a double array containing the x y z values of the origin of the ray
 * @param <direction> pointer to a double array containing the x y z values of the direction vector of the ray
 * @return a pointer to a double array containing the x y z values for the 3D intersection point
 */
double* display_getIntersectionWithScreen(double* origin ,double* direction);


#endif /*LIBGAZE_DISPLAY_MODULE_H_*/
