import os,sys

import pyGaze
import time

class pyGazeExperimentException(Exception):
    code =None
    def __init__(self, code):
        self.code = code
        
    def __str__(self):
        exceptionstr = "ERROR"
        
        
        return exceptionstr;

class PyGazeExperiment:
    pygaze = None
    
    #nsme of the experiment
    exp_name = None
    
    #path under wich the experimental data is saved
    path = None
    
    #name of the current subject
    subject_name = None
    #path under which the data for the current subject is saved
    #path + subject_name
    subject_path = None
    
    #counters for current trial, calibration, validation and drift correction 
    trailnr = None
    blocknr = None
    calibnr = None
    validnr = None
    driftnr = None
    
    
    #strings which define modules for the head and eye tracker and the used display
    eyemod = None
    headmod = None
    dismod = None
    
    #locations for the configuration files for the eye, head and display module
    eyecfg = None
    headcfg = None
    discfg = None
    
    #the VisualisationHooks passed to pyGaze.EyeHeadTracker
    vishooks = None
    
    #instance of the pyGaze.EyeHeadTracker
    eyeheadtracker = None
    
    #the maximum mean calibration and drift correction error in angles
    max_calib_error = None
    avg_calib_error = None
    max_drift_error = None
    
    #x y position on the display when the subject was asked to align to the screen
    center = None
    
    
    def __init__(self,expcfg):
        
        #loads the 
        #
        exec "from "+ expcfg +" import *"
        
        self.exp_name = expname
        self.path = expdatapath
        
        if os.path.exists(self.path) == False:
            os.makedirs(self.path)
            
        self.mode = mode
                                  
        self.eyemod =eyemod
        self.headmod =headmod
        self.dismod =dismod
        
        self.eyecfg =eyecfg
        self.headcfg =headcfg
        self.discfg =discfg
        
        self.calibmethod=calibmethod
        self.calibangle = calibangle
        self.calibgrid = calibgrid
        
        self.validangle = validangle
        self.validgrid = validgrid
        
        self.max_calib_error = max_calib_error
        self.max_drift_error = max_drift_error
        self.avg_calib_error = avg_calib_error
        print self.eyemod
        print self.headmod
        print self.dismod
        self.pygaze = pyGaze.pyGaze(0,1)

        
    def __del__(self):
        if self.eyeheadtracker != None:
            self.eyeheadtracker.stop_tracking()
            self.eyeheadtracker.disconnect()
        #create folder 
        #
        
        
    #initalize the EyeHeadTracker.
    #
    def initTracker(self,vhooks):
        print "EyeHeadTracker", self.eyemod,self.headmod,self.dismod
        self.eyeheadtracker = pyGaze.EyeHeadTracker(0,1)
        self.eyeheadtracker.loadModules(self.eyemod,self.headmod,self.dismod,self.mode)
        print "configure"
        self.eyeheadtracker.configure(self.eyecfg,self.headcfg,self.discfg)
        self.eyeheadtracker.loadCalibrationModule(self.calibmethod)
        self.eyeheadtracker.setVisualHooks(vhooks)
        self.vishooks =vhooks
        self.eyeheadtracker.connect()
        self.eyeheadtracker.startTracking()
        
    
    #Collects the natural alignment of the subject to the screen.
    #The orientation of the subject should be straight ahead to the display.
    #
    #    the x y positon on the display, when the subject is oriented straight
    #    ahead to the screen is saved under self.center
    def collect_natural_alignment(self):
        self.vishooks.eraseScreen()
        self.vishooks.showMessage("Please align your head to the screen.")
        self.vishooks.updateScreen()
        
        keypressed = 0
        while keypressed == 0:
            input = self.vishooks.getInput()
            if input.type == pyGaze.PG_INPUT_ACCEPT:
                self.center = self.eyeheadtracker.getGazeWithAlphaBeta(0.0,0.0)
                print "center", self.center
                keypressed = 1

                print "calib INPUT_QUIT"
            elif input.type == pyGaze.PG_INPUT_QUIT:
                accept = False
                keypressed = 1
                self.end()
                print "calib INPUT_ACCEPT"
            
            time.sleep(0.005)
            
    

    # Collects the relation between the eye and the head (position and orientation).
    # At the end it also measures the natural alignment of the subject to the display. 
    #
    def runHeadCorrectionBlock(self):
        ireturn = self.eyeheadtracker.collectEyeHeadRelation()
        if ireturn == pyGaze.PG_QUIT:
               self.end()
        
        ireturn = self.eyeheadtracker.correctHeadDirectionVector(self.calibangle[0],self.calibangle[1])    
        if ireturn == pyGaze.PG_QUIT:
            self.end()
            
        self.collect_natural_alignment()
        
        
    # Calibrates the EyeHeadTracker. Starts with a calibration followed 
    # by a validation. If the mean validation error is less then 
    # self.max_calib error the calibration is accepted
    def runCalibrationBlock(self):
        print ""
        ireturn = None
            
        exit = False
        accept = False
        while exit == False and accept == False:
        
            ireturn = self.eyeheadtracker.calibrate(self.calibangle[0],self.calibangle[1],self.calibgrid[0],self.calibgrid[1],[self.center[0],self.center[1]])
            if ireturn == pyGaze.PG_QUIT:
                self.end()
            
            verror = pyGaze.ValidationDataSet()
            vreturn = self.eyeheadtracker.validateCalibration(self.validangle[0],self.validangle[1],self.validgrid[0],self.validgrid[1],[self.center[0],self.center[1]], verror)                
            if vreturn == pyGaze.PG_QUIT:
                self.end()
            print "verror:",verror
            print "self.max_calib_error: ",self.max_calib_error 
            if verror.avg_drift[0]<self.avg_calib_error and verror.avg_drift[1]<self.avg_calib_error and verror.max_drift[0]<self.max_calib_error and verror.max_drift[1]<self.max_calib_error:
                accept = True
                self.eyeheadtracker.displayLastValidation()
            else:
                self.eyeheadtracker.displayLastValidation()
    
        return verror        
    
    
    
    def runDriftCorrectionBlock(self):
        
        self.vishooks.erase_screen()
        self.vishooks.show_message("Drift Correction. Do NOT Move Head")
        self.vishooks.update_screen()

        time.sleep(1.5)
        driftlr = pyGaze.ValidationDataSet()
        dreturn = self.eyeheadtracker.driftCorrection(self.center[0],self.center[1],20.0,20.0,2,2,1,driftlr)
        if dreturn == pyGaze.PG_QUIT:
            self.end()
        print "driftlr:",driftlr
        print "self.max_drift_error: ",self.max_drift_error
        if driftlr.max_drift[0] > self.max_drift_error or driftlr.max_drift[1] > self.max_drift_error:
            self.runCalibrationBlock(False)
        
    # Performs a head fixation at a to a specific display position (target = [x,y]).
    # if no target is given, the position calculated during the natural_head_alignment
    # is used as target.
    def doHeadFixation(self, target=None):

        if target == None:
            target=self.center
        print "doHeadFixation",target[0],target[1]
        ireturn = self.eyeheadtracker.doHeadFixation(target[0],target[1],2000);
        if ireturn == pyGaze.PG_QUIT:
            self.end()
    
    
    # Logging routines for start of trial, end of trial, etc
    #
    
    def start(self,subject_name):
        print "start"
        self.trailnr = 0
        self.blocknr = 0
        self.calibnr = 0
        self.validnr = 0
        self.driftnr = 0
        self.subject = subject_name
        
        self.subject_path = self.path + subject_name+"/"
        
        if os.path.exists(self.subject_path) == False:
            os.makedirs(self.subject_path)
        #else:
            #raise pyGazeExperimentException(1)
        print "open_log_file", self.subject, self.subject_path
        self.eyeheadtracker.openLogFile(self.subject,self.subject_path,pyGaze.PG_GAZE_LOG_FILE)
        self.eyeheadtracker.addLogMessage("START_EXPERIMENT")
        #self.eyeheadtracker.logEvent("START_EXPERIMENT")

    def end(self):
        print "start"
        self.eyeheadtracker.stopLogging()
        self.eyeheadtracker.addLogMessage("END_EXPERIMENT")
        #self.eyeheadtracker.logEvent("END_EXPERIMENT")
        self.eyeheadtracker.closeLogFile()
        self.eyeheadtracker.stopTracking()
        self.eyeheadtracker.disconnect()
        sys.exit()
        
        
    def startBlock(self, tag= None):
        print "start_block"
        self.blocknr += 1
        if tag == None:
            tag = ""
        msg = "NEW_BLOCK " + `self.blocknr`+" "+ tag   
        self.eyeheadtracker.addLogMessage(msg)
        #self.eyeheadtracker.logEvent(msg)
        #self.eyeheadtracker.startLogging()
         
    def endBlock(self):
        print "end_block"
        msg = "END_BLOCK " + `self.blocknr`   
        #self.eyeheadtracker.stopLogging()
        self.eyeheadtracker.addLogMessage(msg)
        #self.eyeheadtracker.logEvent(msg)
        
    def startTrail(self, tag= None):
        print "start_trial"
        self.trailnr += 1
        if tag == None:
            tag = ""
        msg = "NEW_TRIAL " + `self.trailnr`+" "+ tag   
        self.eyeheadtracker.addLogMessage(msg)
        #self.eyeheadtracker.logEvent(msg)
        self.eyeheadtracker.startLogging()
         
    
    def endTrail(self):
        print "end_trial"
        msg = "END_TRIAL " + `self.trailnr`   
        self.eyeheadtracker.stopLogging()
        self.eyeheadtracker.addLogMessage(msg)
       # self.eyeheadtracker.logEvent(msg)
    
    def addLogMessage(self,msg):
        self.eyeheadtracker.addLogMessage(msg)
    