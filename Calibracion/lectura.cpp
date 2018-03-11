#include <opencv2/core/core.hpp>
#include <iostream>
#include <stdio.h>
//#include <stdlib.h>
//#include <string> 
//#include <sstream> 

//#include <opencv2/core/mat.hpp>
//#include <opencv2/highgui/highgui.hpp>


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


	int main()
		{
			
			
			cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
			std::cout << "Matriz de camara: " << cameraMatrix << std::endl;
			cv::Mat Distortion = cv::Mat::zeros(8, 1, CV_64F);
			std::cout << "Distorsion de camara " << Distortion << std::endl;
			
			cv::FileStorage storageRead("MiCam.yml", cv::FileStorage::READ);
			storageRead["Camera_Matrix"] >> cameraMatrix;
		    storageRead["Distortion_Coefficients"] >> Distortion;
			
			std::cout << "Matriz de camara: " << cameraMatrix << std::endl;
			std::cout << "Distorsion de camara " << Distortion << std::endl;
			
			
			/*
			int numberInput = 10;
			int numberOutput;
		 
			cv::Mat matrixInput(5, 5, CV_8UC1, cv::Scalar(3));
			cv::Mat matrixOutput;
		 
			cv::FileStorage storageWrite("test.yml", cv::FileStorage::WRITE);
			storageWrite << "matrix" << matrixInput;
			storageWrite << "number" << numberInput;
			storageWrite.release();
		 
			cv::FileStorage storageRead("test.yml", cv::FileStorage::READ);
			storageRead["matrix"] >> matrixOutput;
			storageRead["number"] >> numberOutput;
			
			std::cout << "numberOutput: " << numberOutput << std::endl;
			std::cout << "matrixOutput: " << matrixOutput << std::endl;
			
			
			storageRead.release();

			return 0;
			* */
			return 0;
		}
