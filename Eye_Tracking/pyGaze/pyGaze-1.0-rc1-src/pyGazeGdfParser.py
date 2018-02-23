import pyGaze
from scipy import *

class PyGazeGdfParser:
    GAZE = 1
    DISPLAY_GAZE = 2
    EYE = 4
    HEAD = 8
    DISPLAY_HEAD = 16
    ALL = 31
    
    pareseOption = 1
    display = None
    gdfFile = None
    
    def setDisplay(self,dis):
        self.display = dis
        
    def loadGDF(self,gdfFileName):
        self.gdfFile = open(gdfFileName, "r")
        
    def setParseOption(self,po):
        self.pareseOption = po
        
    def parseGDF(self,outPutFileName):
        outFile = open(outPutFileName, "w")
        
        for line in self.gdfFile:
            line = line.strip()
            s = line.split("\t")
            if s[0] == "M":
                print "MESSAGE:",line
                outFile.write(line+"\n")
            elif s[0] == "V":
                print "VALIDATION:",line
                outFile.write(line+"\n")
            elif s[0] == "D":
                print "DRIFT:",line
                outFile.write(line+"\n")
            elif s[0] == "G":
                if len(s)>1:
                    
                    gds = pyGaze.GazeDataSet()
                    gds.time = long(s[1])
                    gds.gaze_available = int(s[2])
                    
                    gds.gaze[0].position = [float(s[3]),float(s[4]),float(s[5])]
                    gds.gaze[0].direction = [float(s[6]),float(s[7]),float(s[8])]
                    
                    gds.gaze[1].position = [float(s[9]),float(s[10]),float(s[11])]
                    gds.gaze[1].direction = [float(s[12]),float(s[13]),float(s[14])]

                    gds.eye.time = long(s[15])
                    gds.eye.eye_available = int(s[16])
                    gds.eye.alpha = [float(s[17]),float(s[20])] 
                    gds.eye.beta = [float(s[18]),float(s[21])] 
                    gds.eye.size = [float(s[19]),float(s[22])] 
                    
                    gds.head.time = long(s[23]) 
                    
                    gds.head.position = [float(s[24]),float(s[25]) ,float(s[26]) ] 
                              
                    gds.head.orientation = [float(s[27]),float(s[28]) ,float(s[29]) ,float(s[30]) ,float(s[31]) ,float(s[32]) ,float(s[33]) ,float(s[34]) ,float(s[35]) ] 
                    
                    outFile.write("G\t")
                    outFile.write(str(gds.time))
                    outFile.write("\t")
                    outFile.write(`gds.gaze_available`)
                    
                    if (self.pareseOption | PyGazeGdfParser.DISPLAY_GAZE):
                        dc = self.display.getDisplayCoordsFromGaze(gds)
                        outFile.write("\t")
                        disGaze = "%d\t%d\t%d\t%d"%(dc.displayCoords[0].x,dc.displayCoords[0].y,dc.displayCoords[1].x,dc.displayCoords[1].y)
                        outFile.write(disGaze)
                    
                    
                    if (self.pareseOption | PyGazeGdfParser.GAZE):
                        for i in range(2):
                            outFile.write("\t")
                            gazeData ="%f\t%f\t%f\t%f\t%f\t%f"%(gds.gaze[i].position[0],gds.gaze[i].position[1],gds.gaze[i].position[2],gds.gaze[i].direction[0],gds.gaze[i].direction[1],gds.gaze[i].direction[2])
                            outFile.write(gazeData)

                    if (self.pareseOption | PyGazeGdfParser.EYE): 
                        for i in range(2):
                            outFile.write("\t")
                            eyeData = "%f\t%f\t%f"%(gds.eye.alpha[i],gds.eye.beta[i],gds.eye.size[i])
                            outFile.write(eyeData)

                    
                    if (self.pareseOption | PyGazeGdfParser.DISPLAY_HEAD):
                        direction = array([1.0,0.0,0.0]) 
                        mat = array(gds.head.orientation)
                        mat33 = reshape(mat,(3,3))
                        direction = dot(mat33,direction)
                        ray = pyGaze.Ray(gds.head.position,direction)
                        disCoord = self.display.getDisplayCoordFromRay(ray)
                        outFile.write("\t")
                        disHeadData = "%d\t%d"%(disCoord.x,disCoord.y)
                        outFile.write(disHeadData)                        
                        
                        
                    if (self.pareseOption | PyGazeGdfParser.HEAD):
                        outFile.write("\t")
                    
                        eulerXYZ = pyGaze.decomposeRotationMatix(gds.head.orientation)
                        eulerXYZ[0] = eulerXYZ[0]*180.0/pi
                        eulerXYZ[1] = eulerXYZ[1]*180.0/pi
                        eulerXYZ[2] = eulerXYZ[2]*180.0/pi
                        headData = "%f\t%f\t%f\t%f\t%f\t%f\t"%(gds.head.position[0],gds.head.position[1],gds.head.position[2],eulerXYZ[0],eulerXYZ[1],eulerXYZ[2])
                        outFile.write(headData)
                    outFile.write("\n")  
                    
                    
        outFile.close()
        

            
            
        
        