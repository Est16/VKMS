//////////////////////////////////////
// Chet Corcos & Garrett Menghini   //
// February 25, 2012                //
//////////////////////////////////////

// instructions

// be sure to fill in the user defined inputs (UDIs) section at the beginning of the main function
// to create a new profile:
// press space to capture your face
// press enter to move on
// follow instructions cropping and setting anchor points


// include the necessary libraries
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
using namespace std;

// Include header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>

#include <sqlite3.h> // for database
#include <Carbon/Carbon.h> // for GUI

// define states for creating a profile
#define getImageState 0
#define leftEyeState 1
#define LELEState 2
#define LELEanchorState 3
#define LEREState 4
#define LEREanchorState 5
#define RightEyeState 6
#define RELEState 7
#define RELEanchorState 8
#define REREState 9
#define REREanchorState 10
#define liveState 11

// Create a string that contains the exact cascade name
const char* cascade_name ="/Users/chet/OpenCV-2.3.1/data/haarcascades/haarcascade_frontalface_alt.xml";

// mouse callback functions
void mouse_crop( int event, int x, int y, int flags, void* param );
void mouse_anchor( int event, int x, int y, int flags, void* param );

// helper function prototypes
void capture_frame(CvCapture *capture, IplImage* &img);
void find_faces( IplImage* img, CvMemStorage* &storage, CvHaarClassifierCascade* &cascade, CvSeq* &faces, float scale);
void draw_face_rect(CvRect* r, IplImage* &img);
void crop_and_scale_face(IplImage *imgCamera, IplImage* &imgFace, CvRect *r);
CvRect resizeRect(CvRect* r, float scale);
void draw_box(IplImage* &img);
char* path(const char profile[], const char name[]);
void save(IplImage* &img, const char profile[], const char name[]);
void write_text(IplImage* &img, const char text[]);
void save_cropped_image(IplImage* &img, IplImage* &imgSaved, const char profile[], const char name[]);
void save_anchors(const char profile[]);

CvPoint findMax(CvMat* &mat);
CvPoint findLeftEye(IplImage* &imgSRC ,IplImage* &imgDST);
CvPoint findRightEye(IplImage* &imgSRC ,IplImage* &imgDST);
CvPoint findKernel(IplImage* &imgSRC, const char kernelPath[], CvPoint anchor);
char* makePath(const char name[]);
CvPoint getAnchorPoint(const char name[]);
double getMax(CvMat* &mat);

void setup_new_profile(const char profile[], IplImage* &imgFace);

CvPoint findLELE(IplImage* &imgSRC);
CvPoint findLERE(IplImage* &imgSRC);
CvPoint findRERE(IplImage* &imgSRC);
CvPoint findRELE(IplImage* &imgSRC);
CvPoint findPupil(IplImage* &imgSRC);

void getFeatures(IplImage* &img);
CvPoint moveAndScalePoint(CvPoint point, double s);

void load_anchors(const char profile[]);

// Global Variables
CvPoint RE, LE, FACE;
CvPoint LELE, LERE, RELE, RERE, RP, LP;
int matchType = 3;
//         0 = square difference
//         1 = square difference normalized
//         2 = correlation matching
//         3 = correlation matching normalized ----
//         4 = correlation coefficient - inconsisten
//         5 = correlation coefficient normalized - inconsisten


// global variables
CvRect box;
CvPoint anchor;
bool mouse_draw_anchor = false;
bool mouse_draw_box = false;
int state;

// LELE, LERE, RELE, RERE
int anchors[8]; // 8 x-y points

char profileDir[100] = "/Users/chet/Xcode Apps/OpenCV/images/";
char profile[100] = "/Users/chet/Xcode Apps/OpenCV/images/chet profile 4/"; // directory must already be created

int main (int argc, const char * argv[])
{
    /**************************************************************/
    /********************* initialization *************************/
    /**************************************************************/
    
    // UDIs
    state = liveState; // live state if you arent creating a new profile
    float scale = 1.0/10; // how far do we want to scale down the haar detect objects images for speed
        
    // other initialization
    cvNamedWindow("window"); // live, drawn video
    
    IplImage *imgCamera;
    IplImage *imgDrawn;
    IplImage *imgFace;
    IplImage *imgFaceSave;
        
    // instantiate variables
    CvCapture* capture = cvCaptureFromCAM(-1); // capture from video device (Macbook iSight)
    capture_frame(capture, imgCamera);
    imgDrawn = cvCreateImage(cvGetSize(imgCamera), imgCamera->depth, imgCamera->nChannels);
    imgFace = cvCreateImage(cvSize(600, 600), imgCamera->depth, imgCamera->nChannels);
    imgFaceSave = cvCreateImage(cvSize(600, 600), imgCamera->depth, imgCamera->nChannels);
    static CvMemStorage* storage = 0; // Create memory for calculations
    storage = cvCreateMemStorage(0); // Allocate the memory storage
    cvClearMemStorage( storage ); // Clear the memory storage which was used before
    static CvHaarClassifierCascade* cascade = 0; // Create a new Haar classifier
    cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 ); // Load the HaarClassifierCascade
    CvRect *r;
    CvSeq *faces;
    anchor = cvPoint(0, 0);
    bool init = true;
    

    /**************************************************************/
    /********************* implementation *************************/
    /**************************************************************/
    while (1) {
        capture_frame(capture, imgCamera);
        cvCopy(imgCamera, imgDrawn);
        find_faces(imgCamera, storage, cascade, faces, scale);
        
        // for each face found in the image
        for( int i = 0; i < (faces ? faces->total : 0); i++ ){
            r = (CvRect*)cvGetSeqElem( faces, i ); // get the rect from the sequence
            CvRect x = resizeRect(r, scale); // and resize it
            r = &x;
            
            draw_face_rect(r, imgDrawn); // draw the rectange around the face on the imgDrawn
            crop_and_scale_face(imgCamera, imgFace, r);
            
            if (state == liveState) {
                // live eye detection
                if (init) {
                    init = false;
                    cvNamedWindow("window");
                    load_anchors(profile);
                }
                
                // live detection stuff
                
                // get the points in the right place
                // the face image is 600 by 600 so we need to account for this scale
                int w = r->width;
                int x = r->x;
                int y = r->y;
                double s = ((double)w)/600.0;
                // account for where the face is in the image
                FACE = cvPoint(x, y);
                RE = cvPoint(0, 0);
                LE = cvPoint(0, 0);
                
                // get the eye feature points
                getFeatures(imgFace);
                
                LELE = moveAndScalePoint(LELE, s);
                LERE = moveAndScalePoint(LERE, s);
                RERE = moveAndScalePoint(RERE, s);
                RELE = moveAndScalePoint(RELE, s);
                RP = moveAndScalePoint(RP, s);
                LP = moveAndScalePoint(LP, s);
                
                // lets make sure the points line up descently
                bool err = false;
                //left eye
                int width, height;
                width = LERE.x - LELE.x;
                height = abs(LERE.y - LELE.y);
                
                // make sure the edges arent too unaligned
                if (height>10) {
                    err = true;
                }
                // make sure the pupil is near the center
                if (LP.x - LELE.x < width/4 || LP.x - LELE.x > width-width/4) {
                    err = true;
                }
                if (abs(LP.y - LERE.y) > 30) {
                    err = true;
                }
                
                // right eye
                // if widths are different
                if (abs(width - RERE.x + RELE.x) > 20) {
                    err = true;
                }
                width = RERE.x - RELE.x;
                height = abs(RERE.y - RELE.y);
                // make sure the edges arent too unaligned
                if (height>10) {
                    err = true;
                }
                // make sure the pupil is near the center
                if (RP.x - RELE.x < width/4 || RP.x - RELE.x > width-width/4) {
                    err = true;
                }
                if (abs(RP.y - RELE.y) > 30) {
                    err = true;
                }
                if (err) {
                    // not good
                }
                else {
                    // draw the features on
                    cvCircle(imgDrawn, LELE ,3, CV_RGB(0,255,0), -1, 8, 0 );
                    cvCircle(imgDrawn, LERE ,3, CV_RGB(0,255,0), -1, 8, 0 );
                    cvCircle(imgDrawn, LP ,3, CV_RGB(255,0,0), -1, 8, 0 );
                    cvCircle(imgDrawn, RERE ,3, CV_RGB(0,255,0), -1, 8, 0 );
                    cvCircle(imgDrawn, RELE ,3, CV_RGB(0,255,0), -1, 8, 0 );
                    cvCircle(imgDrawn, RP ,3, CV_RGB(255,0,0), -1, 8, 0 );
                }
            }    
        }
        
        if (state == liveState){
            cvShowImage("window", imgDrawn);
            
            char c = cvWaitKey(33); // press escape to quit
            if( c == 27 ) break;
        }
         
        
        if (state == getImageState) {
            char c = cvWaitKey(10);
            // display face til we find a good one
            write_text(imgDrawn, "space - refresh : return - save image");
            cvShowImage("window", imgDrawn);
            
            if (c == 32) { // space
                cvCopy(imgFace, imgFaceSave);
                cvShowImage("face", imgFaceSave);
            }
            
            if (c == 13){ // enter
                state += 1; // leftEyeState
                
                // no longer display live video
                cvDestroyWindow("window");
                cvDestroyWindow("face");
                
                // save the image of the face
                save(imgFaceSave, profile, "face.jpg");
                
                // get all of the sub-images and anchor points we need.
                setup_new_profile(profile, imgFaceSave);
                cvReleaseImage(&imgFaceSave);
            }
        }
        
    }
    
    
    /**************************************************************/
    /******************** release objects *************************/
    /**************************************************************/
    cvReleaseCapture( &capture );
    
    //    cvReleaseImage(&imgCamera); // why doesnt this work??
    cvReleaseImage(&imgDrawn);
    cvReleaseImage(&imgFace);
    
    cvDestroyAllWindows();
    
    cvReleaseMemStorage(&storage);
    
    return 0;
    
}


// goes through the process of creating a profile.
// crop both eyes, and both edges

void setup_new_profile(const char profile[], IplImage* &imgFace){
    
    bool init = true;
    bool not_done = true;
    
    CvRect rectLELE; // where is LELE within the left eye
    CvRect rectLERE;
    CvRect rectRELE;
    CvRect rectRERE;
    
    CvSize sizeEye = cvSize(150, 60);
    CvSize sizeEdge = cvSize(40, 40);
    CvSize sizeEdgeScaled = cvSize(240, 240);
    double edgeScale = 6;

//    IplImage *imgFace = cvLoadImage(path(profile, "face.jpg"));
    IplImage *imgFaceDrawn = cvCreateImage(cvGetSize(imgFace), imgFace->depth, imgFace->nChannels);
    IplImage *imgLeftEye = cvCreateImage(sizeEye, imgFace->depth, imgFace->nChannels);
    IplImage *imgLeftEyeDrawn = cvCreateImage(sizeEye, imgFace->depth, imgFace->nChannels);
    IplImage *imgRightEye = cvCreateImage(sizeEye, imgFace->depth, imgFace->nChannels);
    IplImage *imgRightEyeDrawn = cvCreateImage(sizeEye, imgFace->depth, imgFace->nChannels);
    IplImage *imgLELE = cvCreateImage(sizeEdge, imgFace->depth, imgFace->nChannels);
    IplImage *imgLERE = cvCreateImage(sizeEdge, imgFace->depth, imgFace->nChannels);
    IplImage *imgRELE = cvCreateImage(sizeEdge, imgFace->depth, imgFace->nChannels);
    IplImage *imgRERE = cvCreateImage(sizeEdge, imgFace->depth, imgFace->nChannels);
    
    IplImage *imgEdgeScaled = cvCreateImage(sizeEdgeScaled, imgFace->depth, imgFace->nChannels);
    IplImage *imgEdgeScaledDrawn = cvCreateImage(sizeEdgeScaled, imgFace->depth, imgFace->nChannels);
    
    while (not_done) {
    
        char c = cvWaitKey(10);
        switch (state) {
            case getImageState:
                printf("error1");
                break;
            case leftEyeState:
                if (init){ 
                    init = false;
                    cvNamedWindow("face"); // create window to show face
                    box = cvRect(0, 0, sizeEye.width, sizeEye.height); // initialize the cropping box size
                    cvCopy(imgFace, imgFaceDrawn);
                    write_text(imgFaceDrawn, "crop left eye");
                    
                    cvSetMouseCallback("face", mouse_crop, imgFaceDrawn);
                    
                }
                
                cvShowImage("face", imgFaceDrawn);  // show face with box
                
                if (mouse_draw_box) {               // draw new box is necessary
                    mouse_draw_box = false;                         
                    cvCopy(imgFace, imgFaceDrawn);
                    write_text(imgFaceDrawn, "crop left eye");
                    draw_box(imgFaceDrawn);
                }
                
                if (c == 13){ // enter
                    state += 1;
                      
                    save_cropped_image(imgFace, imgLeftEye, profile, "leftEye.jpg"); // crop the box and save it
                    
                    cvDestroyWindow("face");
                    init = true; // for net state
                }
                break;
                
            case LELEState:
                if (init) {
                    init = false;
                    box = cvRect(0, 0, sizeEdge.width, sizeEdge.height);
                    cvNamedWindow("eye");
                    cvResizeWindow("eye", sizeEye.width, sizeEye.height);
                    
                    cvCopy(imgLeftEye, imgLeftEyeDrawn);
                    write_text(imgLeftEyeDrawn, "LELE");
                    
                    cvSetMouseCallback("eye", mouse_crop, imgLeftEyeDrawn);
                }
                
                cvShowImage("eye", imgLeftEyeDrawn); // show eye with box
                
                if (mouse_draw_box) {
                    mouse_draw_box = false;
                    cvCopy(imgLeftEye, imgLeftEyeDrawn);
                    write_text(imgLeftEyeDrawn, "LELE");
                    draw_box(imgLeftEyeDrawn);
                }
                

                if (c == 13){ // enter
                    state += 1;
                    
                    save_cropped_image(imgLeftEye, imgLELE, profile, "LELE.jpg"); // crop the box and save it
                    rectLELE = box;                                   // remember where the LELE is w.r.t. left eye
                    
                    cvDestroyWindow("eye");
                    
                    init = true; // for net state
                }
                break;
                
            case LELEanchorState:
                if (init) {
                    init = false;
                    cvNamedWindow("edge");
                    cvNamedWindow("eye");
                    cvCopy(imgLeftEye, imgLeftEyeDrawn);
                    
                    cvResize(imgLELE, imgEdgeScaled);
                    cvCopy(imgEdgeScaled, imgEdgeScaledDrawn);
                    write_text(imgEdgeScaledDrawn, "anchor");
                    
                    cvSetMouseCallback("edge", mouse_anchor, imgEdgeScaledDrawn);
                }
                
                cvShowImage("eye", imgLeftEyeDrawn);
                cvShowImage("edge", imgEdgeScaledDrawn);
                
                if (mouse_draw_anchor) {
                    mouse_draw_anchor = false;
                    
                    cvCopy(imgLeftEye, imgLeftEyeDrawn);
                    cvCopy(imgEdgeScaled, imgEdgeScaledDrawn);
                    write_text(imgEdgeScaledDrawn, "anchor");
                    
                    // drawn on eye and edge pics
                    cvCircle(imgLeftEyeDrawn, cvPoint(rectLELE.x + cvRound(anchor.x/edgeScale), rectLELE.y + cvRound(anchor.y/edgeScale) ) ,3, CV_RGB(0,255,0), -1, 8, 0 );
                    cvCircle(imgEdgeScaledDrawn, cvPoint(anchor.x,anchor.y ) ,3, CV_RGB(0,255,0), -1, 8, 0 );
                    
                }

                if (c == 13){ // enter
                    state += 1;
                    
                    // save anchor point
                    anchors[0] = cvRound(anchor.x/edgeScale);
                    anchors[1] = cvRound(anchor.y/edgeScale);
                                        
                    cvDestroyWindow("edge");
                    cvDestroyWindow("eye");
                    init = true; // for next state
                }
                break;
                
            case LEREState:
                if (init) {
                    init = false;
                    box = cvRect(0, 0, sizeEdge.width, sizeEdge.height);
                    cvNamedWindow("eye");
                    cvCopy(imgLeftEye, imgLeftEyeDrawn);
                    write_text(imgLeftEyeDrawn, "LERE");
                    
                    cvSetMouseCallback("eye", mouse_crop, imgLeftEyeDrawn);
                }
                
                cvShowImage("eye", imgLeftEyeDrawn); // show eye with box
                
                if (mouse_draw_box) {
                    mouse_draw_box = false;
                    cvCopy(imgLeftEye, imgLeftEyeDrawn);
                    draw_box(imgLeftEyeDrawn);
                    write_text(imgLeftEyeDrawn, "LERE");
                }
                
                
                if (c == 13){ // enter
                    state += 1;
                    
                    save_cropped_image(imgLeftEye, imgLERE, profile, "LERE.jpg"); // crop the box and save it
                    rectLERE = box;                                   // remember where the LERE is w.r.t. left eye
                    
                    cvDestroyWindow("eye");
                    
                    init = true; // for next state
                }
                break;
                
            case LEREanchorState:
                if (init) {
                    init = false;
                    cvNamedWindow("edge");
                    cvNamedWindow("eye");
                    
                    cvResize(imgLERE, imgEdgeScaled);
                    cvCopy(imgEdgeScaled, imgEdgeScaledDrawn);
                    write_text(imgEdgeScaledDrawn, "anchor");
                    
                    cvSetMouseCallback("edge", mouse_anchor, imgEdgeScaledDrawn);
                }
                
                cvShowImage("eye", imgLeftEyeDrawn);
                cvShowImage("edge", imgEdgeScaledDrawn);
                
                if (mouse_draw_anchor) {
                    mouse_draw_anchor = false;
                    
                    cvCopy(imgLeftEye, imgLeftEyeDrawn);
                    cvCopy(imgEdgeScaled, imgEdgeScaledDrawn);
                    write_text(imgEdgeScaledDrawn, "anchor");
                    
                    // drawn on eye and edge pics
                    cvCircle(imgLeftEyeDrawn, cvPoint(rectLERE.x + cvRound(anchor.x/edgeScale), rectLERE.y + cvRound(anchor.y/edgeScale) ) ,3, CV_RGB(0,255,0), -1, 8, 0 );
                    cvCircle(imgEdgeScaledDrawn, cvPoint(anchor.x,anchor.y ) ,3, CV_RGB(0,255,0), -1, 8, 0 );
                    
                }
                
                if (c == 13){ // enter
                    state += 1;
                    
                    // save anchor point
                    anchors[2] = cvRound(anchor.x/edgeScale);
                    anchors[3] = cvRound(anchor.y/edgeScale);
                    
                    cvDestroyWindow("edge");
                    cvDestroyWindow("eye");
                    
                    init = true; // for next state
                }
                break;
                
                
                
            case RightEyeState:
                if (init){ 
                    init = false;
                    cvNamedWindow("face"); // create window to show face
                    box = cvRect(0, 0, sizeEye.width, sizeEye.height); // initialize the cropping box size
                    cvCopy(imgFace, imgFaceDrawn);
                    write_text(imgFaceDrawn, "crop right eye");
                    
                    cvSetMouseCallback("face", mouse_crop, imgFaceDrawn);
                    
                }
                
                cvShowImage("face", imgFaceDrawn);  // show face with box
                
                if (mouse_draw_box) {               // draw new box is necessary
                    mouse_draw_box = false;                         
                    cvCopy(imgFace, imgFaceDrawn);
                    draw_box(imgFaceDrawn);
                    write_text(imgFaceDrawn, "crop right eye");
                }
                
                if (c == 13){ // enter
                    state += 1;
                    
                    save_cropped_image(imgFace, imgRightEye, profile, "rightEye.jpg"); // crop the box and save it
                    
                    cvDestroyWindow("face");
                    init = true; // for net state
                }
                break;
                
            case RELEState:
                if (init) {
                    init = false;
                    box = cvRect(0, 0, sizeEdge.width, sizeEdge.height);
                    cvNamedWindow("eye");
                    cvCopy(imgRightEye, imgRightEyeDrawn);
                    write_text(imgRightEyeDrawn, "RELE");
                    
                    cvSetMouseCallback("eye", mouse_crop, imgRightEyeDrawn);
                }
                
                cvShowImage("eye", imgRightEyeDrawn); // show eye with box
                
                if (mouse_draw_box) {
                    mouse_draw_box = false;
                    cvCopy(imgRightEye, imgRightEyeDrawn);
                    draw_box(imgRightEyeDrawn);
                    write_text(imgRightEyeDrawn, "RELE");
                }
                
                
                if (c == 13){ // enter
                    state += 1;
                    
                    save_cropped_image(imgRightEye, imgRELE, profile, "RELE.jpg"); // crop the box and save it
                    rectRELE = box;                                   // remember where the RELE is w.r.t. left eye
                    
                    cvDestroyWindow("eye");
                    
                    init = true; // for net state
                }
                break;
                
            case RELEanchorState:
                if (init) {
                    init = false;
                    cvNamedWindow("edge");
                    cvNamedWindow("eye");
                    
                    cvResize(imgRELE, imgEdgeScaled);
                    cvCopy(imgEdgeScaled, imgEdgeScaledDrawn);
                    write_text(imgEdgeScaledDrawn, "anchor");
                    
                    cvSetMouseCallback("edge", mouse_anchor, imgEdgeScaledDrawn);
                }
                
                cvShowImage("eye", imgRightEyeDrawn);
                cvShowImage("edge", imgEdgeScaledDrawn);
                
                if (mouse_draw_anchor) {
                    mouse_draw_anchor = false;
                    
                    cvCopy(imgRightEye, imgRightEyeDrawn);
                    cvCopy(imgEdgeScaled, imgEdgeScaledDrawn);
                    write_text(imgEdgeScaledDrawn, "anchor");
                    
                    // drawn on eye and edge pics
                    cvCircle(imgRightEyeDrawn, cvPoint(rectRELE.x + cvRound(anchor.x/edgeScale), rectRELE.y + cvRound(anchor.y/edgeScale) ) ,3, CV_RGB(0,255,0), -1, 8, 0 );
                    cvCircle(imgEdgeScaledDrawn, cvPoint(anchor.x,anchor.y ) ,3, CV_RGB(0,255,0), -1, 8, 0 );
                    
                }
                
                if (c == 13){ // enter
                    state += 1;
                    
                    // save anchor point
                    anchors[4] = cvRound(anchor.x/edgeScale);
                    anchors[5] = cvRound(anchor.y/edgeScale);
                    
                    cvDestroyWindow("edge");
                    cvDestroyWindow("eye");
                    init = true; // for next state
                }
                break;
                
            case REREState:
                if (init) {
                    init = false;
                    box = cvRect(0, 0, sizeEdge.width, sizeEdge.height);
                    cvNamedWindow("eye");
                    cvCopy(imgRightEye, imgRightEyeDrawn);
                    write_text(imgRightEyeDrawn, "RERE");
                    
                    cvSetMouseCallback("eye", mouse_crop, imgRightEyeDrawn);
                }
                
                cvShowImage("eye", imgRightEyeDrawn); // show eye with box
                
                if (mouse_draw_box) {
                    mouse_draw_box = false;
                    cvCopy(imgRightEye, imgRightEyeDrawn);
                    draw_box(imgRightEyeDrawn);
                    write_text(imgRightEyeDrawn, "RERE");
                }
                
                
                if (c == 13){ // enter
                    state += 1;
                    
                    save_cropped_image(imgRightEye, imgRERE, profile, "RERE.jpg"); // crop the box and save it
                    rectRERE = box;                                   // remember where the RERE is w.r.t. left eye
                    
                    cvDestroyWindow("eye");
                    
                    init = true; // for next state
                }
                break;
                
            case REREanchorState:
                if (init) {
                    init = false;
                    cvNamedWindow("edge");
                    cvNamedWindow("edge");
                    cvNamedWindow("eye");
                    
                    cvResize(imgRERE, imgEdgeScaled);
                    cvCopy(imgEdgeScaled, imgEdgeScaledDrawn);
                    write_text(imgEdgeScaledDrawn, "anchor");
                    
                    cvSetMouseCallback("edge", mouse_anchor, imgEdgeScaledDrawn);
                }
                
                cvShowImage("eye", imgRightEyeDrawn);
                cvShowImage("edge", imgEdgeScaledDrawn);
                
                if (mouse_draw_anchor) {
                    mouse_draw_anchor = false;
                    
                    cvCopy(imgRightEye, imgRightEyeDrawn);
                    cvCopy(imgEdgeScaled, imgEdgeScaledDrawn);
                    write_text(imgRightEyeDrawn, "anchor");
                    
                    // drawn on eye and edge pics
                    cvCircle(imgRightEyeDrawn, cvPoint(rectRERE.x + cvRound(anchor.x/edgeScale), rectRERE.y + cvRound(anchor.y/edgeScale) ) ,3, CV_RGB(0,255,0), -1, 8, 0 );
                    cvCircle(imgEdgeScaledDrawn, cvPoint(anchor.x,anchor.y ) ,3, CV_RGB(0,255,0), -1, 8, 0 );
                    
                }
                
                if (c == 13){ // enter
                    state += 1;
                    
                    // save anchor point
                    anchors[6] = cvRound(anchor.x/edgeScale);
                    anchors[7] = cvRound(anchor.y/edgeScale);
                    
                    cvDestroyWindow("edge");
                    cvDestroyWindow("eye");
                    
                    init = true; // for next state
                }
                break;
            
            case liveState:
            {
                save_anchors(profile);
                not_done = false;
                break;
            }
                
            default:
                state = getImageState;
                break;
        }
    }
    
    cvReleaseImage(&imgFaceDrawn);
    cvReleaseImage(&imgLeftEye);
    cvReleaseImage(&imgLeftEyeDrawn);
    cvReleaseImage(&imgRightEye);
    cvReleaseImage(&imgRightEyeDrawn);
    cvReleaseImage(&imgLELE);
    cvReleaseImage(&imgLERE);
    cvReleaseImage(&imgRELE);
    cvReleaseImage(&imgRERE);
    cvReleaseImage(&imgEdgeScaled);
    cvReleaseImage(&imgEdgeScaledDrawn);
    
}

// mouse call back for drawing box and cropping
void mouse_crop( int event, int x, int y, int flags, void* img ){
    switch( event ){
        case CV_EVENT_MOUSEMOVE: 
            break;
            
        case CV_EVENT_LBUTTONDOWN:
            break;
            
        case CV_EVENT_LBUTTONUP:
            box.x = x - box.width/2;
            box.y = y - box.height/2;
            mouse_draw_box = true;
            break;
    }
}

// mouse call back for selecting anchor point
void mouse_anchor( int event, int x, int y, int flags, void* param ){
    switch( event ){
        case CV_EVENT_MOUSEMOVE: 
            break;
            
        case CV_EVENT_LBUTTONDOWN:
            anchor.x = x;
            anchor.y = y;
            break;
            
        case CV_EVENT_LBUTTONUP:
            mouse_draw_anchor = true;
            break;
    }
}

// saves and image to a path specified by profile, and a name specified by name
void save(IplImage* &img, const char profile[], const char name[]){
    
    char *path = new char[100];
    strcpy(path, profile);
    strcat(path,  name);
    
    cvSaveImage(path, img);
}

// crops an image based on the global variable, box, and saves it
void save_cropped_image(IplImage* &img, IplImage* &imgSaved, const char profile[], const char name[]){
    cvSetImageROI(img, box);
    cvCopy(img, imgSaved);
    cvResetImageROI(img);
    save(imgSaved, profile, name);
}

// draws a box on the image based on the global variable, box
void draw_box(IplImage* &img){
    cvRectangle( img, cvPoint(box.x, box.y), cvPoint(box.x+box.width,box.y+box.height),
                cvScalar(0xff,0x00,0x00) );
}

// captures a frame for video
void capture_frame(CvCapture *capture, IplImage* &img){
    // img is the destination
    
    img = cvQueryFrame(capture); // capture the image from the camera
    if (!img) {
        //error
        fprintf( stderr, "ERROR: Camera is not working");
    } // error checking
    cvFlip(img, img, 1); // flip the image so you see a mirror image of yourself
    
}

// uses a haar classifier to recognize faces
void find_faces( IplImage* img, CvMemStorage* &storage, CvHaarClassifierCascade* &cascade, CvSeq* &faces, float scale){
    // this function finds the faces and returns a sequence
    
    cvClearMemStorage( storage ); // Clear the memory storage which was used before
    
    IplImage* tmp = cvCreateImage(cvSize(cvGetSize(img).width*scale, cvGetSize(img).height*scale), img->depth, img->nChannels);
    
    cvResize(img, tmp);
    
    if( !cascade ){
        fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
        return;
    }// Check whether the cascade has loaded successfully. Else report and error and quit
    
    if( cascade )
    {
        float faceFraction = 1.0/16; // face can be a minimum size of 1/16 the size of the image
        int w = (int)(cvGetSize(tmp).width*faceFraction);
        int h = (int)(cvGetSize(tmp).height*faceFraction);
        faces = cvHaarDetectObjects( tmp, cascade, storage, 1.2, 2, CV_HAAR_DO_CANNY_PRUNING,cvSize(w, h)); // create sequence of images for however many faces are detected
    }
    
    // resize all of the rects
    
    cvReleaseImage(&tmp);
    
}

// resizes a rect based on a scale 
CvRect resizeRect(CvRect* r, float scale){
    return cvRect(r->x/scale, r->y/scale, r->width/scale, r->height/scale);
}

// draws rectangles around faces. nothing exciting here
void draw_face_rect(CvRect* r, IplImage* &img){
    // this function draws the face rectangles on an image
    
    CvPoint pt1, pt2;
    
    // Find the dimensions of the face,and scale it if necessary
    pt1.x = r->x;
    pt2.x = (r->x+r->width);
    pt1.y = r->y;
    pt2.y = (r->y+r->height);
    
    // Draw the rectangle in the input image
    cvRectangle( img, pt1, pt2, CV_RGB(255,0,0), 3, 8, 0 );
}

// crops face from image and scales it to 600 by 600
void crop_and_scale_face(IplImage *imgCamera, IplImage* &imgFace, CvRect *r){
    // get face image
    cvSetImageROI(imgCamera, *r);
    
    IplImage *tmp = cvCreateImage(cvSize(r->width, r->height), imgCamera->depth, imgCamera->nChannels);
    cvCopy(imgCamera, tmp); // the rize width to height ratio here is 1:1 for the built in classifier
    
    cvResetImageROI(imgCamera);
    
    // scale image size accordingly
    //    imgFace = NULL;
    cvResize(tmp, imgFace);
    
    cvReleaseImage(&tmp);
}

// displays text on an image
void write_text(IplImage* &img, const char text[]){
    
    CvPoint pt1 = cvPoint(10,25);
    CvScalar red = CV_RGB(250,0,0);
    
    // Text variables
    double hscale = 0.5;
    double vscale = 0.5;
    double shear = 0.2;
    int thickness = .5;
    int line_type = 8;
    
    CvFont font1;
    cvInitFont(&font1,CV_FONT_HERSHEY_DUPLEX,hscale,vscale,shear,thickness,line_type);
    
    cvPutText(img,text,pt1,&font1,red);
}

// save the anchor points in a text file
void save_anchors(const char profile[]){
    // save the anchor points to a text file
    char *path = new char[100];
    strcpy(path, profile);
    path = strcat(path,  "anchors.txt");
    
    char *w = new char[100];
    sprintf(w, "%d %d %d %d %d %d %d %d", anchors[0], anchors[1], anchors[2], anchors[3], anchors[4], anchors[5], anchors[6], anchors[7]);
    
    ofstream f;
    f.open(path);
    f.clear();
    f << w;
    f.close();
        
}

void load_anchors(const char profile[]){
    char *path = new char[100];
    strcpy(path, profile);
    path = strcat(path,  "anchors.txt");
    
    ifstream f(path);
    char read[10];
    int idx = 0;
    while(!f.eof()){
        f >> read;
        anchors[idx] = atoi(read);
        idx++;
    }
 
//    printf("%d %d %d %d", anchors[0], anchors[1], anchors[2], anchors[3]);    
}




//class profile {   
//    char *name;
//    IplImage * leftEye;
//    IplImage * rightEye;
//    IplImage * LELE;
//    IplImage * LERE;
//    IplImage * RELE;
//    IplImage * RERE;
//    int anchorPoints[8];
//    
//public:
//    void setName(char n[]){
//        name = n;
//    }
//    char* getName(){
//        return name;
//    }
//    
//    void setLeftEye(IplImage* img){
//        leftEye = img;
//    }
//    IplImage* getLeftEye(){
//        return leftEye;
//    }
//    
//    void setRightEye(IplImage* &img){
//        rightEye = img;
//    }
//    IplImage* getRightEye(){
//        return rightEye;
//    }
//    
//    void setLELE(IplImage* &img){
//        LELE = img;
//    }
//    IplImage* getLELE(){
//        return LELE;
//    }
//    
//    void setLERE(IplImage* &img){
//        LERE = img;
//    }
//    IplImage* getLERE(){
//        return LERE;
//    }
//    
//    void setRELE(IplImage* &img){
//        RELE = img;
//    }
//    IplImage* getRELE(){
//        return RELE;
//    }
//    
//    void setRERE(IplImage* &img){
//        RERE = img;
//    }
//    IplImage* getRERE(){
//        return RERE;
//    }
//    
//    void setAnchors(int a[8]){
//        for (int i = 0; i<8; i++) {
//            anchorPoints[i] = a[i];
//        }
//    }
//    int* getAnchors(){
//        return anchorPoints;
//    }
//    
//};


void getFeatures(IplImage* &img){ // img is an image of a face
    // crop out the left and right eye
    IplImage *imgGrey = cvCreateImage(cvGetSize(img), img->depth, 1);
    cvCvtColor(img, imgGrey, CV_BGR2GRAY);
    
    CvRect rLE = cvRect(imgGrey->width/16, imgGrey->height/4, imgGrey->width/2 - imgGrey->width/16, imgGrey->height/4);
    cvSetImageROI(imgGrey, rLE);
    IplImage *imgLeft = cvCreateImage(cvSize(rLE.width, rLE.height), imgGrey->depth, 1);
    cvCopy(imgGrey, imgLeft);
    cvResetImageROI(imgGrey);
    
    CvRect rRE = cvRect(imgGrey->width/2, imgGrey->height/4, imgGrey->width/2 - imgGrey->width/16, imgGrey->height/4);
    cvSetImageROI(imgGrey, rRE);
    IplImage *imgRight = cvCreateImage(cvSize(rRE.width, rRE.height), imgGrey->depth, 1);
    cvCopy(imgGrey, imgRight);
    cvResetImageROI(imgGrey);
    
    // create some points to keep track of where we are with cropping.
    // these points tag the top left of the cropped image within the original image
    LE.x += imgGrey->width/16;
    LE.y += imgGrey->height/4;
    RE.x += imgGrey->width/2;
    RE.y += imgGrey->height/4;
    
    // release stuff
    cvReleaseImage(&imgGrey);
    
    // also check environment
    
    // crop the eyes even tighter using template matching cross-correlation
    IplImage *imgLeftEye, *imgRightEye;
    CvPoint tmp = findLeftEye(imgLeft, imgLeftEye);
    LE.x += tmp.x;
    LE.y += tmp.y;
    cvReleaseImage(&imgLeft);
    
    tmp = findRightEye(imgRight, imgRightEye);
    RE.x += tmp.x;
    RE.y += tmp.y;
    cvReleaseImage(&imgRight);
    
    //    cvShowImage("Window", imgLeftEye);
    //    cvShowImage("Window2", imgRightEye);
    
    // now we need to find the corners of the eyes and the pupil
    // split up the image of the eye into 2 peices so the edges have to be found on either side
    CvRect LLE = cvRect(0, 0, imgLeftEye->width/2, imgLeftEye->height);
    cvSetImageROI(imgLeftEye, LLE);
    IplImage *imgLeftLeftEye = cvCreateImage(cvSize(imgLeftEye->width/2, imgLeftEye->height), imgLeftEye->depth, imgLeftEye->nChannels);
    cvCopy(imgLeftEye, imgLeftLeftEye);
    cvResetImageROI(imgLeftEye);
    
    CvRect LRE = cvRect(imgLeftEye->width/2, 0, imgLeftEye->width/2, imgLeftEye->height);
    cvSetImageROI(imgLeftEye, LRE);
    IplImage *imgLeftRightEye = cvCreateImage(cvSize(imgLeftEye->width/2, imgLeftEye->height), imgLeftEye->depth, imgLeftEye->nChannels);
    cvCopy(imgLeftEye, imgLeftRightEye);
    cvResetImageROI(imgLeftEye);
    
    CvRect RLE = cvRect(0, 0, imgRightEye->width/2, imgRightEye->height);
    cvSetImageROI(imgRightEye, RLE);
    IplImage *imgRightLeftEye = cvCreateImage(cvSize(imgRightEye->width/2, imgRightEye->height), imgRightEye->depth, imgRightEye->nChannels);
    cvCopy(imgRightEye, imgRightLeftEye);
    cvResetImageROI(imgRightEye);
    
    CvRect RRE = cvRect(imgRightEye->width/2, 0, imgRightEye->width/2, imgRightEye->height);
    cvSetImageROI(imgRightEye, RRE);
    IplImage *imgRightRightEye = cvCreateImage(cvSize(imgRightEye->width/2, imgRightEye->height), imgRightEye->depth, imgRightEye->nChannels);
    cvCopy(imgRightEye, imgRightRightEye);
    cvResetImageROI(imgRightEye);
    
    
    // Left Eye, Left Edge
    LELE = findKernel(imgLeftLeftEye, makePath("LELE.jpg"),getAnchorPoint("LELE"));
    LELE.x += LE.x;
    LELE.y += LE.y;
    cvReleaseImage(&imgLeftLeftEye);
    
    
    // Left Eye, Right Edge
    LERE = findKernel(imgLeftRightEye, makePath("LERE.jpg"),getAnchorPoint("LERE"));
    LERE.x += LE.x + imgLeftEye->width/2;
    LERE.y += LE.y;
    cvReleaseImage(&imgLeftRightEye);
    
    // Right Eye, Left Edge
    
    RELE = findKernel(imgRightLeftEye, makePath("RELE.jpg"),getAnchorPoint("RELE"));
    RELE.x += RE.x;
    RELE.y += RE.y;
    cvReleaseImage(&imgRightLeftEye);
    
    // Right Eye, Right Edge
    
    RERE = findKernel(imgRightRightEye, makePath("RERE.jpg"),getAnchorPoint("RERE"));
    RERE.x += RE.x + imgRightEye->width/2;
    RERE.y += RE.y;
    cvReleaseImage(&imgRightRightEye);
    
    // Left Pupil
    LP = findPupil(imgLeftEye);
    LP.x += LE.x;
    LP.y += LE.y;
    
    // Right Pupil
    RP = findPupil(imgRightEye);
    RP.x += RE.x;
    RP.y += RE.y;
    
    cvReleaseImage(&imgRightEye);
    cvReleaseImage(&imgLeftEye);
}

CvPoint findLeftEye(IplImage* &imgSRC ,IplImage* &imgDST){
    // lets get a tighter crop by template matching with the eye
    // load the image of the eye
    
    IplImage *imgKernel = cvLoadImage(makePath("leftEye.jpg"));
    IplImage *imgKernelGray = cvCreateImage(cvGetSize(imgKernel), imgKernel->depth, 1);
    cvCvtColor(imgKernel,imgKernelGray , CV_BGR2GRAY);
    cvReleaseImage(&imgKernel);
    
    // convert to matrix
    CvMat buffer;
    CvMat* matKernel;
    matKernel = cvGetMat(imgKernelGray, &buffer);
    cvReleaseImage(&imgKernelGray);
    
    // correlate with the eye image
    CvMat *matCorr = cvCreateMat(imgSRC->height - matKernel->rows + 1, imgSRC->width- matKernel->cols + 1, CV_32FC1);
    cvMatchTemplate(imgSRC, matKernel, matCorr, matchType);
    
    // find max
    CvPoint point = findMax(matCorr);
    cvReleaseMat(&matCorr);
    
    // create cropped image of eye
    CvRect r = cvRect(point.x, point.y, matKernel->width, matKernel->height);
    cvReleaseMat(&matKernel);
    
    cvSetImageROI(imgSRC, r);
    imgDST = cvCreateImage(cvSize(r.width, r.height), imgSRC->depth, imgSRC->nChannels);
    cvCopy(imgSRC, imgDST);
    cvResetImageROI(imgSRC);
    
    return point;
}

CvPoint findRightEye(IplImage* &imgSRC ,IplImage* &imgDST){
    // lets get a tighter crop by template matching with the eye
    // load the image of the eye
    
    IplImage *imgKernel = cvLoadImage(makePath("rightEye.jpg"));
    IplImage *imgKernelGray = cvCreateImage(cvGetSize(imgKernel), imgKernel->depth, 1);
    cvCvtColor(imgKernel,imgKernelGray , CV_BGR2GRAY);
    cvReleaseImage(&imgKernel);
    
    // convert to matrix
    CvMat buffer;
    CvMat* matKernel;
    matKernel = cvGetMat(imgKernelGray, &buffer);
    cvReleaseImage(&imgKernelGray);
    
    // correlate with the eye image
    CvMat *matCorr = cvCreateMat(imgSRC->height - matKernel->rows + 1, imgSRC->width- matKernel->cols + 1, CV_32FC1);
    cvMatchTemplate(imgSRC, matKernel, matCorr, matchType);
    
    // find max
    CvPoint point = findMax(matCorr);
    cvReleaseMat(&matCorr);
    
    // create cropped image of eye
    CvRect r = cvRect(point.x, point.y, matKernel->width, matKernel->height);
    cvReleaseMat(&matKernel);
    
    cvSetImageROI(imgSRC, r);
    imgDST = cvCreateImage(cvSize(r.width, r.height), imgSRC->depth, imgSRC->nChannels);
    cvCopy(imgSRC, imgDST);
    cvResetImageROI(imgSRC);
    
    return point;
}

double getMax(CvMat* &mat){
    double value;
    double maxVal = 0;
    for (int row = 0; row < mat->rows; row++) {
        for (int col = 0; col < mat->cols; col++) {
            value = cvGetReal2D(mat, row, col);
            if (value > maxVal) {
                maxVal = value;
            }
        }
    }
    return maxVal;
}

CvPoint findMax(CvMat* &mat){
    double value;
    double maxVal = 0;
    CvPoint max;
    for (int row = 0; row < mat->rows; row++) {
        for (int col = 0; col < mat->cols; col++) {
            value = cvGetReal2D(mat, row, col);
            if (value > maxVal) {
                maxVal = value;
                max.x = col;
                max.y = row;
                
            }
        }
    }
    return max;
}

char* makePath(const char name[]){

    char *path = new char[100];
    strcpy(path, profile);
    strcat(path,  name);
    
    return path;
}

CvPoint getAnchorPoint(const char name[]){
    CvPoint point;
    if (strcmp(name, "LELE") == 0) {
        point.x = anchors[0];
        point.y = anchors[1];
    }
    if (strcmp(name, "LERE") == 0) {
        point.x = anchors[2];
        point.y = anchors[3];
    }
    if (strcmp(name, "RELE") == 0) {
        point.x = anchors[4];
        point.y = anchors[5];
    }
    if (strcmp(name, "RERE") == 0) {
        point.x = anchors[6];
        point.y = anchors[7];
    }
    return point;
}

CvPoint findKernel(IplImage* &imgSRC, const char kernelPath[], CvPoint anchor){    
    // load the image of the eye
    IplImage *imgKernel = cvLoadImage(kernelPath);
    
    // get anchor point
    IplImage *imgKernelGray = cvCreateImage(cvGetSize(imgKernel), imgKernel->depth, 1);
    cvCvtColor(imgKernel,imgKernelGray , CV_BGR2GRAY);
    cvReleaseImage(&imgKernel);
    
    // convert to matrix
    CvMat buffer;
    CvMat* matKernel;
    matKernel = cvGetMat(imgKernelGray, &buffer);
    cvReleaseImage(&imgKernelGray);
    
    // correlate with the eye image
    CvMat *matCorr = cvCreateMat(imgSRC->height - matKernel->rows + 1, imgSRC->width- matKernel->cols + 1, CV_32FC1);
    cvMatchTemplate(imgSRC, matKernel, matCorr, matchType);
    
    // find max
    CvPoint point = findMax(matCorr);
    point.x += anchor.x;
    point.y += anchor.y;
    cvReleaseMat(&matCorr);
    cvReleaseMat(&matKernel);
    
    return point;
}

CvPoint findPupil(IplImage* &imgSRC){    
    // load the image of the eye
    
    char *path = new char[100];
    strcpy(path, profileDir);
    strcat(path,  "pupil.jpg");
    
    IplImage *imgKernel = cvLoadImage(path);
    CvPoint anchor = cvPoint(imgKernel->width/2, imgKernel->height/2);
    
    IplImage *imgKernelGray = cvCreateImage(cvGetSize(imgKernel), imgKernel->depth, 1);
    cvCvtColor(imgKernel,imgKernelGray , CV_BGR2GRAY);
    cvReleaseImage(&imgKernel);
    
    // convert to matrix
    CvMat buffer;
    CvMat* matKernel;
    matKernel = cvGetMat(imgKernelGray, &buffer);
    cvReleaseImage(&imgKernelGray);
    
    // correlate with the eye image
    CvMat *matCorr = cvCreateMat(imgSRC->height - matKernel->rows + 1, imgSRC->width- matKernel->cols + 1, CV_32FC1);
    cvMatchTemplate(imgSRC, matKernel, matCorr, matchType);
    
    // find max
    CvPoint point = findMax(matCorr);
    point.x += anchor.x;
    point.y += anchor.y;
    cvReleaseMat(&matCorr);
    cvReleaseMat(&matKernel);
    
    return point;
}

CvPoint moveAndScalePoint(CvPoint point, double s){
    point.x = FACE.x + cvRound(((double)point.x) * s);
    point.y = FACE.y + cvRound(((double)point.y) * s);
    return point;
}
