#include <cstdio>
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

std::vector<cv::Point> centers;

cv::Point stabilize(std::vector<cv::Point> &points, int windowSize)
{
  float sumX = 0;
  float sumY = 0;
  int count = 0;
  for (int i = std::max(0, (int)(points.size() - windowSize)); i < points.size(); i++)
  {
      sumX += points[i].x;
      sumY += points[i].y;
      ++count;
  }
  if (count > 0)
  {
      sumX /= count;
      sumY /= count;
  }
  return cv::Point(sumX, sumY);
}

////////////////////////////////////////////////////////////////////
//						DETECTAR LA PUPILA						  //
////////////////////////////////////////////////////////////////////

cv::Vec3f getEyeball(cv::Mat &eye, std::vector<cv::Vec3f> &circles)
{
  std::vector<int> sums(circles.size(), 0);
  for (int y = 0; y < eye.rows; y++)
  {
      uchar *ptr = eye.ptr<uchar>(y);
      for (int x = 0; x < eye.cols; x++)
      {
          int value = static_cast<int>(*ptr);
          for (int i = 0; i < circles.size(); i++)
          {
              cv::Point center((int)std::round(circles[i][0]), (int)std::round(circles[i][1]));
              int radius = (int)std::round(circles[i][2]);
              if (std::pow(x - center.x, 2) + std::pow(y - center.y, 2) < std::pow(radius, 2))
              {
                  sums[i] += value;
              }
          }
          ++ptr;
      }
  }
  int smallestSum = 9999999;
  int smallestSumIndex = -1;
  for (int i = 0; i < circles.size(); i++)
  {
      if (sums[i] < smallestSum)
      {
          smallestSum = sums[i];
          smallestSumIndex = i;
      }
  }
  return circles[smallestSumIndex];
}


////////////////////////////////////////////////////////////////////
//						DETECTAR EL IRIS						  //
////////////////////////////////////////////////////////////////////
cv::Rect getLeftmostEye(std::vector<cv::Rect> &eyes)
{
  int leftmost = 99999999;
  int leftmostIndex = -1;
  for (int i = 0; i < eyes.size(); i++)
  {
      if (eyes[i].tl().x < leftmost)
      {
          leftmost = eyes[i].tl().x;
          leftmostIndex = i;
      }
  }
  return eyes[leftmostIndex];
}



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

// Llamada a la duncion de deteccion del iris

cv::Rect eyeRect = getLeftmostEye(eyes);

// Llamada a la funcion de openCV que detecta un area circular dentro de la region ocular

cv::Mat face = frame(faces[0]); // crop the face
cv::Mat eye = face(eyeRect); // crop the leftmost eye
cv::cvtColor(eye, grayscale, CV_BGR2GRAY); // convert image to grayscale
cv::equalizeHist(eye, eye);
std::vector<cv::Vec3f> circles;
cv::HoughCircles(eye, circles, CV_HOUGH_GRADIENT, 1, eye.cols / 8, 250, 15, eye.rows / 8, eye.rows / 3);


if (circles.size() > 0)
  {
      cv::Vec3f eyeball = getEyeball(eye, circles);
      cv::Point center(eyeball[0], eyeball[1]);
      centers.push_back(center);
      center = stabilize(centers, 5); // we are using the last 5
  }
  
  cv::imshow("Eye", eye);
  
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





