#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <opencv2/core/types.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp> // no es necesaria
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp> 

using namespace cv;
using namespace std;

void detectEyes(cv::Mat &frame, cv::CascadeClassifier &faceCascade, cv::CascadeClassifier &eyeCascade)
{
  // Detección de caras
  cv::Mat grayscale;
  cv::cvtColor(frame, grayscale, CV_BGR2GRAY); // convert image to grayscale
  cv::equalizeHist(grayscale, grayscale); // enhance image contrast 
  std::vector<cv::Rect> faces;
  std::vector<cv::Rect> eyes;
  //faceCascade.detectMultiScale(grayscale, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(150, 150));
  faceCascade.detectMultiScale(grayscale, faces, 1.1, 10, CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING, cvSize(0,0), cvSize(300, 300));
  
  if (faces.size() == 0) return; 
  else
  {
	  		for(int i=0; i < faces.size();i++)
			{
			Point pt1(faces[i].x+faces[i].width, faces[i].y+faces[i].height);
			Point pt2(faces[i].x,faces[i].y);
			Mat faceROI = grayscale(faces[i]);
			eyeCascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30,30));
			
				for(size_t j=0; j< eyes.size(); j++)
				{
				//Point center(faces[i].x+eyes[j].x+eyes[j].width*0.5, faces[i].y+eyes[j].y+eyes[j].height*0.5);
				Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
				int radius = cvRound((eyes[j].width+eyes[j].height)*0.25);
				circle(frame, center, radius, Scalar(255,0,0), 2, 8, 0);
				}
				rectangle(frame, pt1, pt2, cvScalar(0,255,0), 2, 8, 0);
			}
  
  
  /*
  if (faces.size() == 0) return; // none face was detected
  // Detección de ojos
  cv::Mat face = frame(faces[0]); // crop the face
  std::vector<cv::Rect> eyes;
  eyeCascade.detectMultiScale(face, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(150, 150)); // same thing as above  
  // Dibujar las regiones detectadas
  rectangle(frame, faces[0].tl(), faces[0].br(), cv::Scalar(255, 0, 0), 2);
  if (eyes.size() != 2) {// both eyes were not detected
  std::cerr << "both eyes were not detected." << std::endl;
  return; 
  }
  
  for (cv::Rect &eye : eyes)
  {
      rectangle(frame, faces[0].tl() + eye.tl(), faces[0].tl() + eye.br(), cv::Scalar(0, 255, 0), 2);
  }
  */
}
}



int main()
{
////////////////////////////////////////////////////////////////////
//    DETECCIÓN DE CARA Y OJOS CON EL ALGORITMO DE VIOLA-JONES    //			
////////////////////////////////////////////////////////////////////

  cv::CascadeClassifier faceCascade;
  cv::CascadeClassifier eyeCascade;
  if (!faceCascade.load("./haarcascade_frontalface_alt.xml"))
  {
      std::cerr << "Could not load face detector." << std::endl;
      return -1;
  }
  if (!eyeCascade.load("./haarcascade_eye_tree_eyeglasses.xml"))
  {
      std::cerr << "Could not load eye detector." << std::endl;
      return -1;
  }
	
	
////////////////////////////////////////////////////////////////////
//						INICIAR LA CÁMARA						  //
////////////////////////////////////////////////////////////////////
	
  cv::VideoCapture cap(0); // the fist webcam connected to your PC
  if (!cap.isOpened())
  {
      std::cerr << "Webcam not detected." << std::endl;
      return -1;
  }
  cv::Mat frame;
  while (1)
  {
      cap >> frame; // outputs the webcam image to a Mat
      detectEyes(frame, faceCascade, eyeCascade); // Llamada a la funcion de deteccion de cara y ojos
      cv::imshow("Webcam", frame); // displays the Mat
      if (cv::waitKey(30) >= 0) break; // takes 30 frames per second. if the user presses any button, it stops from showing the webcam
  }
  return 0;
}




