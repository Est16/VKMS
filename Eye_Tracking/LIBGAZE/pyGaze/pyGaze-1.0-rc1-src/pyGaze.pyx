cimport pyGaze
cimport pyGazeHeadTracker
cimport pyGazeEyeTracker
cimport pyGazeDisplay
cimport pyGazeGazeTracker
cimport pyGazeVisualisationHooks
cimport pyGazeInput

cdef extern from "libGaze/libGaze.h":
	ctypedef enum lg_eyes:
		LG_EYE_BOTH 	= 1,
		LG_EYE_LEFT,
		LG_EYE_RIGHT,
		LG_EYE_NONE 
	
	int init_LibGaze(int dlevel, int elevel)
	
	unsigned long long lg_get_time()
	
	lg_tools_decompose_rotation_matix(double *matrix, double *eulerX, double *eulerY, double *eulerZ)

def decomposeRotationMatix(matrix):
	"""
	Decomposes a rotation matrix into euler angles for yaw pitch and roll.
			
			@param matrix:  a single array of 9 elements presenting a 3x3 rotation matrix.
			
			@return:  a list with 3 elements containing the euler angles for x y z rotations.
	"""
	 
	cdef double cmatrix[9]
	cdef double eulerX
	cdef double eulerY
	cdef double eulerZ
	
	cmatrix[0] = matrix[0]
	cmatrix[1] = matrix[1]
	cmatrix[2] = matrix[2]
	cmatrix[3] = matrix[3]
	cmatrix[4] = matrix[4]
	cmatrix[5] = matrix[5]
	cmatrix[6] = matrix[6]
	cmatrix[7] = matrix[7]
	cmatrix[8] = matrix[8]
	lg_tools_decompose_rotation_matix(cmatrix, &eulerX, &eulerY, &eulerZ)
	
	return [eulerX,eulerY,eulerZ]

class pyGaze:
	"""
	The base class of the pyGaze module. This class has to be initialized before any other pyGaze
	class is used.
	"""
		
	def __init__(self, debug = 0, error =1):
		"""
		Initializes the pyGaze framework. During the initialisation the internal timer will be reset.
	
			@param debug:  if the framework should print out debug messages
			@param error:  if the framework should print out error messages 
		"""
		init_LibGaze(debug,error)

	
	def getTime(self):
		"""
		Gets the current time from the pyGaze timer.
				@return:  long value current time in milliseconds 
		"""
		return lg_get_time()
		


	
PG_KEY_PRESS = 1
PG_NO_KEY_PRESS = 0

PG_ERROR = 5
PG_OK = 6
PG_ACCEPT = 7
PG_ABORT = 8
PG_QUIT = 9
PG_TIMEOUT = 10


PG_EYE_BOTH = LG_EYE_BOTH
PG_EYE_LEFT = LG_EYE_LEFT
PG_EYE_RIGHT = LG_EYE_RIGHT
PG_EYE_NONE = LG_EYE_NONE

PG_GT_MODE_BOTH = pyGazeGazeTracker.GT_MODE_BOTH
PG_GT_MODE_EYE = pyGazeGazeTracker.GT_MODE_EYE
PG_GT_MODE_HEAD = pyGazeGazeTracker.GT_MODE_HEAD
PG_GT_MODE_DUMMY = pyGazeGazeTracker.GT_MODE_DUMMY

PG_ET_MODE_NORMAL = pyGazeEyeTracker.EYETRACKER_MODE_NORMAL
PG_ET_MODE_DUMMY = pyGazeEyeTracker.EYETRACKER_MODE_DUMMY

PG_HT_MODE_NORMAL = pyGazeHeadTracker.HEADTRACKER_MODE_NORMAL
PG_HT_MODE_DUMMY = pyGazeHeadTracker.HEADTRACKER_MODE_DUMMY

PG_INPUT_NONE = pyGazeInput.LG_INPUT_NONE
PG_INPUT_QUIT = pyGazeInput.LG_INPUT_QUIT
PG_INPUT_ACCEPT = pyGazeInput.LG_INPUT_ACCEPT
PG_INPUT_ABORT = pyGazeInput.LG_INPUT_ABORT
PG_INPUT_KEY = pyGazeInput.LG_INPUT_KEY
PG_INPUT_MESSAGE = pyGazeInput.LG_INPUT_MESSAGE

PG_NO_LOG_FILE = 0
PG_GAZE_LOG_FILE = 1
PG_HEAD_LOG_FILE = 2
PG_EYE_LOG_FILE = 4
#PG_EVENT_LOG_FILE = 8
#PG_CALIB_LOG_FILE = 16
PG_ALL_LOG_FILE = 31

	


	
cdef class Display:
	"""
	The Display class represents the display model used with pyGaze. It loads 
	a libGaze display module and offers functions to interact with this model.
	"""
	cdef pyGazeDisplay.t_display* d_ptr

	def __cinit__(self):
		print "Display.__cinit__"
		self.d_ptr = NULL
	
	def loadModule(self, char* dismod, int debug = 0, int error = 1):
		"""
		Loads a libGaze display module.

				@param dismod:  location of the display module
				@param debug:  if the the module should return debug output messages 
				@param error:  if the the module should return error output messages 
		"""
		self.d_ptr = pyGazeDisplay.d_load_module(dismod , debug, error)
	
	def configure(self, cfg):
		"""
		Configures the loaded display module using a configuration file.
				@param cfg:  the path to the configuration file for the display module.
		"""
		pyGazeDisplay.d_configure(self.d_ptr,cfg)
		
	def get3DPositionFromXY(self, int x, int y):
		"""
		Gets the 3d position of a given pixel (x,y) on the display in the world coordinate system.
			
				@param x:  x value of the pixel
				@param y:  y value of the pixel
				
				@return a list of 3 values = [x, y, z]
		"""
		cdef int ixy[2]
		cdef double* pos
		ixy[0] = x
		ixy[1] = y
		pos = pyGazeDisplay.d_get3DPositionFromXY(self.d_ptr,ixy)
		return [pos[0],pos[1],pos[2]]
	
	def getXYPositionFrom3D(self, pos):
		"""
		Gets the pixel postion [x, y] of a given point on the screen represented by a 3D point in the world coordinate system.
				
				@param pos:  the 3d postion [x, y, z] on the display.
	
				@return:  a two element list with x and y pixel postion [x, y]  
		"""
		cdef double dpos[3]
		cdef int* ixy
		dpos[0] = pos[0]
		dpos[1] = pos[1]
		dpos[2] = pos[2]
		ixy = pyGazeDisplay.d_getXYPositionFrom3D(self.d_ptr,dpos)
		return [ixy[0],ixy[1]]
		
	
#	def getIntersectionWithScreen(self, position, direction):
#		"""	 Gets the intersection on the disply with a line. The line is represented by its 
#			origin (starting position of the line in 3D space in the world coordinate system) and its direction (3D direction vector in the world coordinate system).
#			
#			params:
#				postion = a 3d vector [x, y, z]
#				direction = a 3d vector [x, y, z]
#			returns:
#				a 3d vector representing the intersection point of the line and the display
#				[x, y, z] 
#		"""
#		cdef double *intersection
#		cdef double tmpPosition[3]
#		cdef double tmpDirection[3]
#		cdef int* ixy
#		tmpPosition[0] = position[0]
#		tmpPosition[1] = position[1]
#		tmpPosition[2] = position[2]
#		tmpDirection[0] = direction[0]
#		tmpDirection[1] = direction[1]
#		tmpDirection[2] = direction[2]
#		intersection = pyGazeDisplay.d_getIntersectionWithScreen(self.d_ptr,tmpPosition, tmpDirection)
#		return [intersection[0],intersection[1],intersection[2]]

	def getDisplayCoordsFromGaze(self, gds):
		"""
		Calculates a DisplayCoordSet from a given GazeDataSet.
				@param gds:  the GazeDataSet presenting the gaze of an observer
				
				@return an instance of a DisplayCoordSet representing the intersection of the gaze and the display model.
		"""		
		 
		cdef pyGaze.t_gaze_data_set cgds 
		cdef pyGazeDisplay.t_display_coord_set dcoords
		cgds.gaze_available = gds.gaze_available
		cgds.time = gds.time
		
		cgds.head.time = gds.head.time
		cgds.head.position[0] = gds.head.position[0]
		cgds.head.position[1] = gds.head.position[1]
		cgds.head.position[2] = gds.head.position[2]
		
		cgds.head.orientation[0] =gds.head.orientation[0]
		cgds.head.orientation[1] =gds.head.orientation[1]
		cgds.head.orientation[2] =gds.head.orientation[2]
		cgds.head.orientation[3] =gds.head.orientation[3]
		cgds.head.orientation[4] =gds.head.orientation[4]
		cgds.head.orientation[5] =gds.head.orientation[5]
		cgds.head.orientation[6] =gds.head.orientation[6]
		cgds.head.orientation[7] =gds.head.orientation[7]
		cgds.head.orientation[8] =gds.head.orientation[8]
		
		cgds.eye.time = gds.eye.time 
		cgds.eye.eye_available = gds.eye.eye_available 
		cgds.eye.alpha[0] = cgds.eye.alpha[0]
		cgds.eye.alpha[1] = cgds.eye.alpha[1]
		cgds.eye.beta[0] = cgds.eye.beta[0]
		cgds.eye.beta[1] = cgds.eye.beta[1]
		cgds.eye.size[0] = cgds.eye.size[0]
		cgds.eye.size[1] = cgds.eye.size[1]

		cgds.gaze[0].origin[0] =  gds.gaze[0].origin[0]
		cgds.gaze[0].origin[1] =  gds.gaze[0].origin[1]
		cgds.gaze[0].origin[2] =  gds.gaze[0].origin[2]
		
		cgds.gaze[0].direction[0] =  gds.gaze[0].direction[0]
		cgds.gaze[0].direction[1] =  gds.gaze[0].direction[1]
		cgds.gaze[0].direction[2] =  gds.gaze[0].direction[2]
		
		cgds.gaze[1].origin[0] =  gds.gaze[1].origin[0]
		cgds.gaze[1].origin[1] =  gds.gaze[1].origin[1]
		cgds.gaze[1].origin[2] =  gds.gaze[1].origin[2]
		
		cgds.gaze[1].direction[0] =  gds.gaze[1].direction[0]
		cgds.gaze[1].direction[1] =  gds.gaze[1].direction[1]
		cgds.gaze[1].direction[2] =  gds.gaze[1].direction[2]
		
		pyGazeDisplay.d_getDisplayCoordsFromGazeSet(self.d_ptr, &cgds,  &dcoords)
		
		dc = DisplayCoordSet()
		
		dc.display_coords_available = dcoords.display_coords_available
		
		dc.avg_x = dcoords.avg_x
		dc.avg_y = dcoords.avg_y
		
		dc.displayCoords[0].x = dcoords.displayCoords[0].x
		dc.displayCoords[0].y = dcoords.displayCoords[0].y
		
		dc.displayCoords[0].pos3D[0] = dcoords.displayCoords[0].pos3D[0] 
		dc.displayCoords[0].pos3D[1] = dcoords.displayCoords[0].pos3D[1] 
		dc.displayCoords[0].pos3D[2] = dcoords.displayCoords[0].pos3D[2] 
		
		dc.displayCoords[1].x = dcoords.displayCoords[1].x
		dc.displayCoords[1].y = dcoords.displayCoords[1].y
		
		dc.displayCoords[1].pos3D[0] = dcoords.displayCoords[1].pos3D[0] 
		dc.displayCoords[1].pos3D[1] = dcoords.displayCoords[1].pos3D[1] 
		dc.displayCoords[1].pos3D[2] = dcoords.displayCoords[1].pos3D[2] 
		

		
		return dc

	def getDisplayCoordFromRay(self, ray):
		"""
		Calculates a DisplayCood from a Ray.
				@param ray:  a Ray object presenting the ray from which the intersection with the screen needs to be calculated.
				
				@return a DislpayCoord object containing the xy display coordinates on the display and its 3D position.
		"""
		cdef pyGazeGazeTracker.t_ray cray 
		cdef pyGazeDisplay.t_display_coord dcoord
		
		cray.origin[0] = ray.origin[0]
		cray.origin[1] = ray.origin[1]
		cray.origin[2] = ray.origin[2]
		
		cray.direction[0] = ray.direction[0]
		cray.direction[1] = ray.direction[1]
		cray.direction[2] = ray.direction[2]
		
		pyGazeDisplay.d_getDisplayCoordFromRay(self.d_ptr, &cray,  &dcoord)
		
		pyDCoord = DisplayCoord()
		pyDCoord.x = dcoord.x
		pyDCoord.y = dcoord.y
		
		pyDCoord.pos3D[0] = dcoord.pos3D[0]
		pyDCoord.pos3D[1] = dcoord.pos3D[1]
		pyDCoord.pos3D[2] = dcoord.pos3D[2]
		
		return pyDCoord

cdef class EyeTracker:
	"""
	"""
	cdef pyGazeEyeTracker.t_eyetracker* et_ptr
	cdef int debug
	cdef int error
	
	
	def __cinit__(self, debug = 0, error = 1):	
		self.debug =debug
		self.error = error
		
	def loadModule(self, module, mode):
		"""
			Loads an eyetracker module.
			
			@param module:  path to the eye tracker module 
			@param mode: the mode in which the module should be loaded
					PG_ET_MODE_NORMAL:
					PG_ET_MODE_DUMMY:
		
		"""
		self.et_ptr = pyGazeEyeTracker.et_load_module(module,self.debug, self.error, mode)
	
	def connect(self):
		"""
		Connects to the EyeTracker.
		"""
		pyGazeEyeTracker.et_connect_to_eye_tracker(self.et_ptr)
		
	def disconnect(self):
		"""
		Disconnects the EyeTracker.
		"""
		pyGazeEyeTracker.et_disconnect_eye_tracker(self.et_ptr)
	
	
	def configure(self, cfgFile):
		"""
		Configures the EyeTracker using a configuration file.
			
				@param cfgFile:  name and location of the configuration file
		"""
		pyGazeEyeTracker.et_configure_eye_tracker(self.et_ptr, cfgFile)
		
	def startTracking(self):
		"""
		Starts the tracking of the EyeTracker.
		"""
		pyGazeEyeTracker.et_start_eye_tracking(self.et_ptr)
		
	def stopTracking(self):
		"""
		Stops the tracking of the EyeTracker.
		"""
		pyGazeEyeTracker.et_stop_eye_tracking(self.et_ptr)
		
	def isConnected(self):
		"""
		Checks if the EyeTracker is connected.
		
				@return:  
					
		"""
		return pyGazeEyeTracker.et_is_eye_tracker_connected(self.et_ptr)
	
	def openLogFile(self, file, path):
		"""
		Opens a logfile for logging eyetracking data and messages.
				
					@param file:  the name of the log file which should be generated
					@param path:  the path where the log file should be generated.
					
		"""
		pyGazeEyeTracker.et_open_log_file(self.et_ptr,file, path)
		
	def closeLogFile(self):
		"""
		Closes the logfile opened by the openLogFile function.
		"""
		pyGazeEyeTracker.et_close_log_file(self.et_ptr)
		
	def startLogging(self):
		"""
		Starts the logging of eyetracking data into the opened log file.
		"""
		pyGazeEyeTracker.et_start_logging(self.et_ptr)
		
	def stopLogging(self):
		"""
		Stops the logging of eyetracking data into the opened log file.
		"""
		pyGazeEyeTracker.et_stop_logging(self.et_ptr)
		
	def addLogMessage(self, msg):
		"""
		Writes  a message into the opened log file. A timestamp will be put 
		infront of the message.
		
				@param msg:  the message which will be added to the log file 
		"""
		pyGazeEyeTracker.et_add_log_msg(self.et_ptr, msg)
		
	def getTrackingData(self):
		"""
		"""
		cdef pyGazeEyeTracker.t_raw_eye_data cedata
		pyGazeEyeTracker.et_get_eyetracking_data(self.et_ptr, &cedata)
		edata = RawEyeData()
		edata. time = chdata.time
		edata.eye_available = chdata.eye_available
		edata.pupil_x = [chdata.pupil_x[0],chdata.pupil_x[1]]
		edata.pupil_y = [chdata.pupil_y[0],chdata.pupil_y[1]]
		edata.pupil_size = [chdata.pupil_size[0],chdata.pupil_size[1]]
		return edata

	def waitForFixation(self,eye, fixdur,timeout):
		"""
		Waits for an eye fixation. If the eye if not moving for a specific amount of time the 
		(fixdur) fixation will be accepted. Otherwise if the eye is not fixating for at least this specific amount of time
		the function will timeout.
		
			@param eye:  for which eye should be waited for a fixation
			@param fixdur:  minimal time in millisec. until a fixation is accepted
			@param timeout:  time in millisec untill the function breaks up
		
		"""
		cdef pyGazeEyeTracker.t_eye_fix_data efdata
		pyGazeEyeTracker.et_wait_for_fixation(self.et_ptr,eye,&efdata,fixdur,timeout)

cdef class HeadTracker:
	cdef pyGazeHeadTracker.t_headtracker* ht_ptr
	cdef int debug
	cdef int error
	
	def __cinit__(self, debug = 0, error = 1):	
		self.debug =debug
		self.error = error
		
	def loadModule(self, module, mode):
		self.ht_ptr = pyGazeHeadTracker.ht_load_module(module,self.debug, self.error, mode)
	
	def connect(self):
		"""
		Connects to the HeadTracker.
		"""
		pyGazeHeadTracker.ht_connect_to_head_tracker(self.ht_ptr)
		
	def disconnect(self):
		"""
		Disconnects the HeadTracker.
		"""
		pyGazeHeadTracker.ht_disconnect_head_tracker(self.ht_ptr)
	
	def configure(self, cfgFile):
		"""
		Configures the HeadTracker using a configuration file.
			
				@param cfgFile:  name and location of the configuration file
		"""
		pyGazeHeadTracker.ht_configure_head_tracker(self.ht_ptr, cfgFile)
		
	def startTracking(self):
		"""
		Starts the tracking of the HeadTracker.
		"""
		pyGazeHeadTracker.ht_start_head_tracking(self.ht_ptr)
		
	def stopTracking(self):
		"""
		Stops the tracking of the HeadTracker.
		"""
		pyGazeHeadTracker.ht_stop_head_tracking(self.ht_ptr)
		
	def isConnected(self):
		"""
		Checks if the HeadTracker is connected.
				
				@return:
					
		"""
		return pyGazeHeadTracker.ht_is_head_tracker_connected(self.ht_ptr)
	
	def openLogFile(self, file, path):
		"""
		Opens a logfile for logging headtracking data and messages.
			
				@param file:  the name of the log file which should be generated
				@param path:   the path where the log file should be generated.
					
		"""
		pyGazeHeadTracker.ht_open_log_file(self.ht_ptr,file, path)
		
	def closeLogFile(self):
		"""
		Closes the logfile opened by the openLogFile function.
		"""
		pyGazeHeadTracker.ht_close_log_file(self.ht_ptr)
		
	def startLogging(self):
		"""
		Starts the logging of headtracking data into the opened log file.
		"""
		pyGazeHeadTracker.ht_start_logging(self.ht_ptr)
		
	def stopLogging(self):
		"""
		Stops the logging of headtracking data into the opened log file.
		"""
		pyGazeHeadTracker.ht_stop_logging(self.ht_ptr)
		
	def addLogMessage(self, msg):
		"""
		Writes  a message into the opened log file. A timestamp will be put 
		infront of the message.
		
				@param msg: the message which will be added to the log file 
		"""
		pyGazeHeadTracker.ht_add_log_msg(self.ht_ptr, msg)
		
	def getTrackingData(self, id):
		"""
		Gets the current RawHeadData for the head tracked object with a specific id.
		
				@param id:  the id of the head tracked object 
				
				@return:  RawHeadData of the head tracked object
		"""
		cdef pyGazeHeadTracker.t_raw_head_data chdata
		pyGazeHeadTracker.ht_get_tracking_data(self.ht_ptr, id, &chdata)
		hdata = RawHeadData()
		hdata. time = chdata.time
		hdata.button = chdata.button
		hdata.position = [chdata.position[0],chdata.position[1],chdata.position[2]]
		hdata.matrix = [chdata.matrix[0],chdata.matrix[1],chdata.matrix[2],chdata.matrix[3],chdata.matrix[4],chdata.matrix[5],chdata.matrix[6],chdata.matrix[7],chdata.matrix[8]]
		return hdata
	
cdef class GazeTracker:
	cdef pyGazeGazeTracker.t_gazetracker *gt_ptr
	
	def __cinit__(self, debug = 0, error = 1):
		self.gt_ptr = pyGazeGazeTracker.gt_create_eyeheadtracker(debug,error)
		
	def loadModules(self,eyemod, headmod, mode = PG_GT_MODE_BOTH):
		"""
		Loads all modules for the seperate tracking devices used by the GazeTracker.
		The GazeTracker uses modules for an EyeTracker a HeadTracker and the used Display to
		present the Calibration targets. In addition to the modules a mode can be specified in which the GazeTracker 
		should run. 
		
		 	@param eyemod:  name and location of the EyeTracker module
		 	@param headmod:  name and location of the HeadTracker module
		 	@param mode:  describes the mode the GazeTracker can run in. 
		 	
 				PG_EHT_MODE_BOTH = The GazeTracker connects to both EyeTracker and HeadTracking systems and 
 						uses their current data for all calculations.
 												
 				PG_EHT_MODE_DUMMY = GazeTracker runs in DUMMY mode in the is mode the system does not
 						connect to the EyeTracker and HeadTracker it only simulates a connection to
 						both tracker and used dummy data for all calculations.
 	 
 	 			PG_EHT_MODE_EYE = GazeTracker only connects to the EyeTracker and uses the real life data
 	 					from the EyeTracker. It only simulates a connection to the HeadTracker and 
 	 					uses artificial RawHeadData for all calculations. 
		 		
		 		PG_EHT_MODE_HEAD = GazeTracker only connects to the HeadTracker and uses the real life data
 	 					from the HeadTracker. It only simulates a connection to the EyeTracker and 
 	 					uses artificial RawEyeData for all calculations. 
		
		"""
		pyGazeGazeTracker.gt_load_modules(self.gt_ptr, eyemod, headmod, mode)
	
	def configure(self,eyecfg, headcfg):
		"""
		Configures the GazeTracker and the EyeTracker, HeadTracker and Display modules used
		by the GazeTracker.
		
				@param eyecfg:  path to the configuration file of the EyeTracker module 
				@param headcfg:  path to the configuration file of the HeadTracker module 		
		"""
		pyGazeGazeTracker.gt_configure(self.gt_ptr, eyecfg, headcfg)
	
	def loadCalibrationModule(self, cmod):
		"""
		Loads the Calibration module the GazeTracker uses for calulating the 
		mapping function map the RawEyeData from its EyeTracker to EyeData. With this
		EyeData the GazeData is calculated by the GazeTracker. 
		
			@param cmod:  path to the Calibtion module
		
		"""
		pyGazeGazeTracker.gt_load_calibration_module(self.gt_ptr,cmod)
	
	def connect(self):
		"""
		Connects to the EyeTracker and HeadTracker used by the GazeTracker.
		"""
		pyGazeGazeTracker.gt_connect(self.gt_ptr)
		
	def disconnect(self):
		"""
		Disconnects the EyeTracker and HeadTracker used by the GazeTracker.
		"""
		pyGazeGazeTracker.gt_disconnect(self.gt_ptr)
		
	def startTracking(self):
		"""
		Starts the tracking on the EyeTracker and HeadTracker used by the GazeTracker.
		It also start the tracking of the of gaze by the GazeTracker.
		"""
		pyGazeGazeTracker.gt_start_tracking(self.gt_ptr)
		
	def stopTracking(self):
		"""
		Stops the tracking of the gaze by the GazeTracker.
		It also stops the tracking on the EyeTracker and HeadTracker used by the GazeTracker.
		"""
		pyGazeGazeTracker.gt_stop_tracking(self.gt_ptr)
		
	def startLogging(self):
		"""
		Starts logging of the GazeData into the logfile opend with the openLogFile function.
		This function also starts the logging on the EyeTracker and the HeadTracker used by the GazeTracker.
		"""
		pyGazeGazeTracker.gt_start_logging(self.gt_ptr)
	
	def stopLogging(self):
		"""
		Stops logging of the GazeData into the logfile opend with the openLogFile function.
		This function also stops the logging on the EyeTracker and the HeadTracker used by the GazeTracker.
		"""
		pyGazeGazeTracker.gt_stop_logging(self.gt_ptr)
	
	def openLogFile(self, file, path=".", flag = PG_GAZE_LOG_FILE):
		"""
		Opens a log file for logging the GazeData calcualted by the GazeTracker.
		The .......
		
				@param file:  
				@param path:   
		"""
		pyGazeGazeTracker.gt_open_log_file(self.gt_ptr, file, path,flag)
		
	def closeLogFile(self):	
		"""
		Closes the log files opened by the openLogFile Function.
		This function also closes the log files opened on the EyeTracker and HeadTracker.
		"""  
		pyGazeGazeTracker.gt_close_log_file(self.gt_ptr)
		
	def addLogMessage(self, msg):
		"""
		Adds a messahge to the log file of the GazeTracker.	This function also adds the same message to the log files of the EyeTracker and the HeadTracker
		used by the GazeTracker.
		
				@param msg:  messaged which should be added to the log file
		"""
		pyGazeGazeTracker.gt_add_log_msg(self.gt_ptr, msg)

	def saveCalibrationData(self, calibFile):
		"""
		Saves the calibration data of the GazeTracker into a file. This
		calibration data can be loaded using the loadCalibrationData function.
		
				@param calibFile:  file name of the file the calibration data should be saved in
		"""
		pyGazeGazeTracker.gt_save_calibration_data(self.gt_ptr,  calibFile)

	def loadCalibrationData(self, calibFile):
		"""
		Loads calibration data to calibrate the GazeTracker from a file. A file which contains calibration data
		can be generated using the saveCalibrationData function.
			
				@param calibFile:  name of the file which contains calibration data
		"""
		pyGazeGazeTracker.gt_load_calibration_data(self.gt_ptr, calibFile)

	def setVisualHooks(self, pyVhooks):
		"""
		Sets the VisualisationHooks of the GazeTracker. The VisualisationHooks are
		used by the GazeTracker to present informations and calibration targets during the 
		calibration procedure. The object given as parameter to this function must implement the VisualisationHooks interface.
		
			@param pyVhooks: 	instanze of a class which implements the VisualisationHooks interface. This object is responsible to 
						present information and calibration targets to the user during calibration.
		
			
		"""
		
		cdef pyGazeVisualisationHooks.t_visualisation_hooks *vhooks
		vhooks = pyGazeVisualisationHooks.v_alloc_visualisation_hooks()
		pyGazeVisualisationHooks.extractVisualHooks(pyVhooks,vhooks)
		pyGazeGazeTracker.gt_set_visual_hooks(self.gt_ptr, vhooks)

	def collectEyeHeadRelation(self):
		"""
		Step 1 of the calibration procedure. This function collects the relationship between the eyes position of the head tracked 
		object tracked by the HeadTracker and the eyes of the observer.
		"""
		pyGazeGazeTracker.gt_collect_eyehead_relation(self.gt_ptr)

	def correctHeadDirectionVector(self, alpha, beta):
		"""
		Step 2 of the calibration procedure. This function adjusts the orientation of the head tracked object so
		that it maps to the visual field of view of the observer. 
		
				@param alpha:  horizontal size (deg) of the presented vfov
				@param beta:  vertical size (deg) of the presented vfov
		"""
		pyGazeGazeTracker.gt_correct_head_direction_vector(self.gt_ptr,  alpha,  beta)

	def calibrate(self, alpha, beta, n, m, center):
		"""
		Step 3 of the calibration procedure. Calibrates the mapping function which maps the raw pupil positions in the camera images of the EyeTracker to eye related viewing angles.
		
					@param alpha:  horizontal size (in deg) of the grid used for the calibration 
					@param beta:  vertical size (in deg) of the grid used for the calibration 
					@param n:  number of columns of the fixation target grid
					@param m:  number of rows of the fixation target grid
					@param center:  the position of the center of the fixation target grid in display coordinates
		"""
		cdef double icenter[3]
		if center != None:

			icenter[0]= center[0]
			icenter[1]= center[1]
			icenter[2]= center[2]
			return pyGazeGazeTracker.gt_calibrate(self.gt_ptr,  alpha,  beta,  n,  m,  icenter)
		else:
			return pyGazeGazeTracker.gt_calibrate(self.gt_ptr,  alpha,  beta,  n,  m,  NULL)
			
	def validateCalibration(self,alpha,beta, n, m, center, vDataSet):
		"""
		Step 4 of the calibration procedure. This function validates the calibration of the mapping function calibrated by the 
		calibrate function.
		
				@param alpha:   horizontal size (in deg) of the grid used for the validation 
				@param beta:   vertical size (in deg) of the grid used for the validation 
				@param n:   number of columns of the fixation target grid
				@param m:   number of rows of the fixation target grid
				@param center:  the position of the center of the fixation target grid in display coordinates
				@param vDataSet:  instance of the ValidationDataSet object the validationData will be stored in
		"""
		cdef double icenter[3]
		cdef pyGazeGazeTracker.t_validation_data_set vdataset
		#cdef ValidationData vtmp
		ireturn = PG_OK
		if center != None:
			icenter[0]= center[0]
			icenter[1]= center[1]
			icenter[2]= center[2]
			ireturn = pyGazeGazeTracker.gt_validate_calibration(self.gt_ptr,  &vdataset,  alpha,  beta,  n,  m,  icenter)
		else:
			ireturn = pyGazeGazeTracker.gt_validate_calibration(self.gt_ptr,  &vdataset,  alpha,  beta,  n,  m,  NULL)
		if ireturn == PG_OK:

			vDataSet.num = vdataset.num
			vDataSet.bad = vdataset.bad
			vDataSet.good = vdataset.good
			
			vDataSet.avg_drift[0] = vdataset.avg_drift[0]
			vDataSet.avg_drift[1] = vdataset.avg_drift[1]
			
			vDataSet.max_drift[0] = vdataset.max_drift[0]
			vDataSet.max_drift[1] = vdataset.max_drift[1]
			
			vDataSet.avg_drift_alpha[0] = vdataset.avg_drift_alpha[0]
			vDataSet.avg_drift_alpha[1] = vdataset.avg_drift_alpha[1]
			
			vDataSet.avg_drift_beta[0] = vdataset.avg_drift_beta[0]
			vDataSet.avg_drift_beta[1] = vdataset.avg_drift_beta[1]
			
			vDataSet.vdata = []
			for i in range(vDataSet.num):
				vtmp = ValidationData()
				#vtmp.x = vdataset.vdata[i].x
				#vtmp.y = vdataset.vdata[i].y
				#vtmp.cx = [vdataset.vdata[i].cx[0],vdataset.vdata[i].cx[1]]
				#vtmp.cy = [vdataset.vdata[i].cy[0],vdataset.vdata[i].cy[1]]
				vtmp.fov_alpha = vdataset.vdata[i].fov_alpha
				vtmp.fov_beta = vdataset.vdata[i].fov_beta
				vtmp.alpha = [vdataset.vdata[i].alpha[0], vdataset.vdata[i].alpha[1] ]
				vtmp.beta = [vdataset.vdata[i].beta[0], vdataset.vdata[i].beta[1] ]
				
				vtmp.calpha = [vdataset.vdata[i].calpha[0],vdataset.vdata[i].calpha[1]]
				vtmp.cbeta = [vdataset.vdata[i].cbeta[0],vdataset.vdata[i].cbeta[1]]
				vtmp.cdrift = [vdataset.vdata[i].cdrift[0],vdataset.vdata[i].cdrift[1]]
				vDataSet.vdata.append(vtmp)
		return ireturn
		
	def driftCheck(self, center, alpha, beta, n, m ,count, driftDataSet):
		"""
		
			@param center:  
			@param alpha:
			@param beta:
			@param n:  
			@param m:  
			@param count:
			@param driftDataSet: 
		"""
		
		cdef pyGazeGazeTracker.t_validation_data_set vdataset
		cdef double icenter[3]
		if center != None:
			icenter[0]= center[0]
			icenter[1]= center[1]
			icenter[2]= center[2]
			ireturn = pyGazeGazeTracker.gt_drift_check(self.gt_ptr, &vdataset, icenter, alpha, beta, n, m, count)
		else:
			ireturn = pyGazeGazeTracker.gt_drift_check(self.gt_ptr, &vdataset, NULL, alpha, beta, n, m, count)
			
		#driftDataSet = ValidationDataSet()
		driftDataSet.num = vdataset.num
		driftDataSet.bad = vdataset.bad
		driftDataSet.good = vdataset.good
		
		driftDataSet.avg_drift[0] = vdataset.avg_drift[0]
		driftDataSet.avg_drift[1] = vdataset.avg_drift[1]
		
		driftDataSet.max_drift[0] = vdataset.max_drift[0]
		driftDataSet.max_drift[1] = vdataset.max_drift[1]
		
		driftDataSet.avg_drift_alpha[0] = vdataset.avg_drift_alpha[0]
		driftDataSet.avg_drift_alpha[1] = vdataset.avg_drift_alpha[1]
		
		driftDataSet.avg_drift_beta[0] = vdataset.avg_drift_beta[0]
		driftDataSet.avg_drift_beta[1] = vdataset.avg_drift_beta[1]
		
		
		return ireturn
		
		
		
	def displayLastValidation(self):
		"""
		Displays the result of the last preformed validation of the GazeTracker calibration.
		To display the ValidationDataSet of the last preformed validation the VisualisationHooks
		are used.
		"""
		pyGazeGazeTracker.gt_display_last_validation(self.gt_ptr)

	def doHeadFixation(self, center, fixdur=2000, showInstruction=0):
		"""
		Preforms a fixation of the head orientation at a specific position on the display.
		The function will end when the head is oriented for a specific time to a fixation target
		on the display or until a given timeout is reached.
		
				@param x:  x position of the target in display coordinates
				@param y:  y position of the target in display coordinates
				@param fixdur:   time in millisec unbtil a fixation of the head oirientation is accepted
				@param showInstructions:   1 if instructions for the headFixation should be presented at the beginning or 0 if not
		"""
		cdef double icenter[3]

		icenter[0]= center[0]
		icenter[1]= center[1]
		icenter[2]= center[2]
		pyGazeGazeTracker.gt_do_head_fixation(self.gt_ptr,  icenter,  fixdur,  showInstruction)
		
	def getCurrentGaze(self,gds):
		"""
		Gets the current gaze tracked by the GazeTracker and stores it into a GazeDataSet object.
			
				@param gds:  instance of the GazeDataSet object where the gaze should be stored in
		"""
		cdef pyGaze.t_gaze_data_set cgds 
		pyGazeGazeTracker.gt_get_current_gaze_position(self.gt_ptr,  &cgds)
		#gds = GazeDataset()
		
		gds.gaze_available = cgds.gaze_available
		gds.time = cgds.time
		
		gds.head.time = cgds.head.time
		gds.head.position = [cgds.head.position[0],cgds.head.position[1],cgds.head.position[2]]
		gds.head.orientation = [cgds.head.orientation[0],cgds.head.orientation[1],cgds.head.orientation[2],cgds.head.orientation[3],cgds.head.orientation[4],cgds.head.orientation[5],cgds.head.orientation[6],cgds.head.orientation[7],cgds.head.orientation[8]]
		
		gds.eye.time = cgds.eye.time 
		gds.eye.eye_available = cgds.eye.eye_available 
		gds.eye.alpha = [cgds.eye.alpha[0], cgds.eye.alpha[1]]
		gds.eye.beta = [cgds.eye.beta[0], cgds.eye.beta[1]]
		gds.eye.size = [cgds.eye.size[0], cgds.eye.size[1]]
		
		gds.gaze[0].origin =  [cgds.gaze[0].origin[0] ,cgds.gaze[0].origin[1], cgds.gaze[0].origin[2]]
		gds.gaze[0].direction =  [cgds.gaze[0].direction[0] ,cgds.gaze[0].direction[1], cgds.gaze[0].direction[2]]
		
		gds.gaze[1].origin =  [cgds.gaze[1].origin[0] ,cgds.gaze[1].origin[1], cgds.gaze[1].origin[2]]
		gds.gaze[1].direction =  [cgds.gaze[1].direction[0] ,cgds.gaze[1].direction[1], cgds.gaze[1].direction[2]]
		#return gds	
		
#	def getGazeWithAlphaBeta(self,alpha, beta, gds):
#		"""Stores the current gaze tracked by the GazeTracker into GazeDataSet object.
#			
#			params:
#			
#				gds = instance of the GazeDataSet object where the gaze should be stored in
#		"""
#		cdef pyGaze.t_gaze_data_set cgds 
#		pyGazeGazeTracker.gt_get_gaze_with_alpha_beta(self.gt_ptr,  alpha, beta ,&cgds)
#		#gds = GazeDataset()
#		
#		gds.gaze_available = cgds.gaze_available
#		gds.time = cgds.time
#		
#		gds.head.time = cgds.head.time
#		gds.head.position = [cgds.head.position[0],cgds.head.position[1],cgds.head.position[2]]
#		gds.head.orientation = [cgds.head.orientation[0],cgds.head.orientation[1],cgds.head.orientation[2],cgds.head.orientation[3],cgds.head.orientation[4],cgds.head.orientation[5],cgds.head.orientation[6],cgds.head.orientation[7],cgds.head.orientation[8]]
#		
#		gds.eye.time = cgds.eye.time 
#		gds.eye.eye_available = cgds.eye.eye_available 
#		gds.eye.alpha = [cgds.eye.alpha[0], cgds.eye.alpha[1]]
#		gds.eye.beta = [cgds.eye.beta[0], cgds.eye.beta[1]]
#		gds.eye.size = [cgds.eye.size[0], cgds.eye.size[1]]
#		
#		gds.gaze[0].origin =  [cgds.gaze[0].origin[0] ,cgds.gaze[0].origin[1], cgds.gaze[0].origin[2]]
#		gds.gaze[0].direction =  [cgds.gaze[0].direction[0] ,cgds.gaze[0].direction[1], cgds.gaze[0].direction[2]]
#		
#		gds.gaze[1].origin =  [cgds.gaze[1].origin[0] ,cgds.gaze[1].origin[1], cgds.gaze[1].origin[2]]
#		gds.gaze[1].direction =  [cgds.gaze[1].direction[0] ,cgds.gaze[1].direction[1], cgds.gaze[1].direction[2]]
#		#return gds			
	
	def getRayRelativeToHeadOrientaion(self, alpha, beta):
		"""
		"""
		cdef pyGazeGazeTracker.t_ray cray 
		pyGazeGazeTracker.gt_get_ray_relative_to_head_orientation(self.gt_ptr,  alpha, beta ,&cray)
		pray = Ray()
		pray.origin = [cray.origin[0],cray.origin[1],cray.origin[2]]
		pray.direction = [cray.direction[0],cray.direction[1],cray.direction[2]]
		return pray
		
	
	def getEyeTracker(self):
		"""
		Returns the instance of the EyeTracker module used by the GazeTracker.
				
				@return:  instance of EyeTracker class  
		"""
		cdef pyGazeEyeTracker.t_eyetracker *et
		cdef EyeTracker eyetracker  
		eyetracker = EyeTracker()
		et = pyGazeGazeTracker.gt_get_eyetracker(self.gt_ptr)
		if et != NULL:

			eyetracker.et_ptr = et
			return eyetracker
		else:
			return None 
			
	def getHeadTracker(self):
		"""
		Returns the instance of the HeadTracker module used by the GazeTracker.
				
				@return:  instance of HeadTracker class  
		"""
		cdef pyGazeHeadTracker.t_headtracker *ht
		cdef HeadTracker headtracker  
		headtracker = HeadTracker()
		ht = pyGazeGazeTracker.gt_get_headtracker(self.gt_ptr)
		if ht != NULL:

			headtracker.ht_ptr = ht
			return headtracker
		else:
			return None 

class Input:
	"""
		
			@attrib type: 	
				PG_INPUT_ABORT: a abort signal is send as input
				PG_INPUT_ACCEPT: a accept signal is send as input
				PG_INPUT_KEY: the input is a pressed key (the value of the pressed key is sved under key)
				PG_INPUT_MESSAGE: depricated
				PG_INPUT_NONE: no new input
				PG_INPUT_QUIT: a quit signal is send as input
				
			@attrib key:  the value of the pressd key of a  type PG_INPUT_KEY input 
	"""
	
	
	type = 0
	key = " "

	def __init__(self):
		self.type = PG_INPUT_NONE 

	
class RawEyeData:
	"""
			@attrib time:  the time in milliseconds when the eye data was colleced fromthe tracker
			@attrib eye_available:  for which eye data is available
			@attrib pupil_x:  the x position of the pupil centroid in the camera images of the eye tracker for the left and right eye 
			@attrib pupil_y:  the y position of the pupil centroid in the camera images of the eye tracker for the left and right eye 
			@attrib pupil_size: the size of left and right eye in the camera images of the eye tracker 
	"""
	def __init__(self):
		""""""

class RawHeadData:
	"""
			@attrib time:  the time in milliseconds when the data set was collected from the HeadTracker
			@attrib button:  when a button is mounted to the tracked object this attributed shows if it is pressed (1) or not (0)
			@attrib position:  the 3D postion of the tracked object
			@attrib matrix:  a 9 element array presenting a 3x3 rotation matrix which represents the orientation of the tracked object.
	"""

	def __init__(self):
		""""""

class GazeData:
	"""
	
			@attrib origin:  the 3D position of the orign of the gaze ray.
			@attrib direction:  the 3D direction vector of the gaze ray.
	"""
	origin = [0,0,0]
	direction = [0,0,0]
	def __init__(self):
		""""""

class ValidationData:
	"""
	The ValidationData class contains the validation data for one target presented during a validation or a driftCheck.
			@attrib alpha:  the horizontal angle in degree were the validation target was presented
			@attrib beta:  the vertical angle in degree were the validation target was presented
			@attrib calpha:  the calculated horizontal viewinging angle in degree for the left and right eye
			@attrib cbeta:  the calculated vertical viewinging angle in degree for the left and right eye
			@attrib cdrift:  the overall drift between the presented and calculated angles for the left and right eye
	"""
	alpha = 0.0
	beta =0.0
	calpha = [0.0,0.0]
	cbeta = [0.0,0.0]
	cdrift = [0.0,0.0]
	
	def __init__(self):
		self.calpha =  [0.0,0.0]
		self.cbeta = [0.0,0.0]
		self.cdrift = [0.0,0.0]
	
	def __str__(self):
		str = `[self.alpha,self.beta,self.calpha,self.cbeta,self.cdrift]`
		return str

class ValidationDataSet:
	"""
			@attrib num:
			@attrib bad:  
			@attrib good:  
			@attrib max_drift:
			@attrib avg_drift:
			@attrib avg_drift_alpha:
			@attrib avg_drift_beta:
			@attrib vdata:
	"""	
	num = None
	bad = None
	good = None
	avg_drift = [None,None]
	max_drift = [None,None]
	avg_drift_alpha = [None,None]
	avg_drift_beta = [None,None]
	
	def __init__(self):
		#print "ValidationDataSet()"
		self.num = 0
		self.bad = 0
		self.good= 0
		self.avg_drift = [0.0, 0.0]
		self.max_drift = [0.0,0.0]

	def __str__(self):
		svdata = []
		for i in range(len(self.vdata)):
			svdata.append(self.vdata[i].__str__())
		str = `[[self.num,self.bad,self.good,self.avg_drift,self.max_drift],svdata]`
		return str

		
class HeadData:
	"""
			
			@attrib position:  an array of float values with the size of 3 representing the position of the object(e.g. [x, y, z])
			@attrib orientation:  an array of float values with the size of 9 representing a 3x3 orientation matrix  of the object
				(e.g. [a00, a01, a02, a10, a11, a12, a20, a21, a22])
	"""

	position = [0.0, 0.0, 0.0]
	orientation = [1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0]


class EyeData:
	"""
	The EyeData class contains processed eye data like viewing angles for the left and right eye.
			@attrib eye_available:  for which eye data is available in this data set. Possible values are:
					PG_EYE_BOTH
					PG_EYE_LEFT
					PG_EYE_NONE 
					PG_EYE_RIGHT
			@attrib alpha:  the horizontal viewing angle for the right and left eye  
			@attrib beta:  the vertical viewing angle for the right and left eye  
			@attrib size:   the size of the right and left eye  
	"""
	eye_available = 0
	alpha = [0.0, 0.0]
	beta = [0.0, 0.0]
	size = [0.0, 0.0]
	

class GazeDataSet:
	"""
			@attrib gaze_available:  for which eye GazeData is available in this GazeDataSet.
			@attrib eye:  processed EyeData containing the eye related viewing angles of the gaze.
			@attrib head:  processed HeadData containing the corrected position and orientation of the observers head. 
			@attrib gaze:  a list of GazeData objects for the left and right eye.
	"""
	gaze_available = None
	eye = None
	head = None
	gaze = [None, None]
	
	def __init__(self):
		self.eye = EyeData()
		self.head = HeadData()
		self.gaze = [GazeData(),GazeData()]	
	
class DisplayCoord:
	"""
	The DisplayCoord class represents a display coordinate on a Display. It contains the xy pixel coordinates and its 3D position.
			@attrib x:  the x display position 
			@attrib y:  the y display position 
			@attrib pos3D:  the 3D position of the xy display coordinate.
	"""
	x=0
	y=0
	pos3D=[0.0,0.0,0.0]
	def __init__(self):
		self.pos3D = [0.0,0.0,0.0]
			
class DisplayCoordSet:
	"""
	The DisplayCoordSet class contains display coordinates for both eyes. It also contains the average xy pixel coordinates of both eyes.
			@attrib display_coords_available: For which eye display coordinates are available in the displayCoords list.
			@attrib avg_x:  the average x display coordinate form the left and right eye
			@attrib avg_y:  the average y display coordinate form the left and right eye
			@attrib displayCoords: an arrray containing two DisplayCoord objects for the left and right eye
	"""
	
	display_coords_available = 0
	avg_x = 0
	avg_y = 0
	displayCoords = [None,None]
	
	def __init__(self):
		self.displayCoords[0] = DisplayCoord()
		self.displayCoords[1] = DisplayCoord()
class Ray:
	"""
	The Ray class represents a ray in 3D space.
			@attrib origin:  the 3d position of the origin of the ray 
			@attrib direction:  the 3d direction vector of the ray
	"""
	origin = None
	direction = None

	def __init__(self, origin = [0.0,0.0,0.0], direction = [0.0,0.0,0.0]):
		self.origin = origin
		self.direction = direction
		
		
class IVisualisationHooks:
	"""
	The IVisualisationHooks class is an interface class 
	"""
	def drawCalTarget(self,origin,direction):
		"""
		Draws a calibration target on a 3D viewing ray. Teh ray is represented by the its
		origin and direction in the WCS 3D coordinate system. To draw the target on th 2d 
		display the display module is used to calculate the xy 2d coordinates on the display.
		"""
		print "please override me"

	def eraseScreen(self):
		"""
		Clears all contentent presented on the screen.
		"""
		print "please override me"
	
	def updateScreen(self):
		"""
		Updates the content presented on the screen. All elements which were drawn after the last
		eraseScreen are presented.
		"""
		print "please override me"
		
	def drawValidation(self,origin,direction,vtype):
		print "please override me"

	def drawFov(self,r0,r1,r2,r3):
		print "please override me"
		
	def drawHeadOrientationTarget(self,origin, direction,r ):
		print "please override me"
		
	def drawHeadOrientation(self,origin, direction,r):
		print "please override me"
	
	def showMessage(self,msg):
		"""
		Presents a message on the screen.
		"""
		print "please override me"
	
	def getInput(self):
		"""
		Returns the current input done by the user. 
		The return value is an instance of the Input class.
		"""
		print "please override me"
	
	
	def clearInput(self):
		"""Clears the current Input queue.
		"""
		print "please override me"

