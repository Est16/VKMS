cimport pyGaze
cimport pyGazeGazeTracker

cdef extern from "libGaze/display/libGaze_display.h":
	
	ctypedef struct t_display_coord:
		int x
		int y
		double pos3D[3]
	
	ctypedef struct t_display_coord_set:
		int display_coords_available
		int avg_x
		int avg_y
		t_display_coord displayCoords[2]
	
	ctypedef struct t_display	
	
	
	
	t_display* d_load_module(char* dismod ,int debug, int error)
	int d_configure(t_display* display, char* discfg)
#	int d_unload_module(t_display* display)
	double* d_get3DPositionFromXY(t_display* display, int* xy)
	int* d_getXYPositionFrom3D(t_display* display, double* intersection)
	double* d_getIntersectionWithScreen(t_display* display, double* position, double* direction)
	int d_getDisplayCoordsFromGazeSet(t_display* display, pyGaze.t_gaze_data_set *gds, t_display_coord_set* dcoords)
	int d_getDisplayCoordFromRay(t_display* display, pyGazeGazeTracker.t_ray *ray, t_display_coord* dcoord)