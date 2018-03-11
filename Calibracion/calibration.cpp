#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string> 
#include <sstream> 

using namespace cv;

static void calcChessboardCorners(Size boardSize, float squareSize, std::vector<cv::Point3f>& corners)
{
	corners.clear();
    for( int i = 0; i < boardSize.height; i++ )
        for( int j = 0; j < boardSize.width; j++ )
            corners.push_back(Point3f(float(j*squareSize),
                                      float(i*squareSize), 0)); 
}

static void saveparams(const std::string& filename, const Mat& cameraMatrix, const Mat& distCoeffs,
					   const std::vector<Mat>& rvecs,const std::vector<Mat>& tvecs,const const double& RMS)
{
		FileStorage fs( filename, FileStorage::WRITE );
		fs << "Calibrate_Accuracy" << RMS;
		fs << "Camera_Matrix" << cameraMatrix;
		fs << "Distortion_Coefficients" << distCoeffs;
		fs << "Rotation_Vector" << rvecs;
		fs << "Translation_vector" << tvecs;

		if( !rvecs.empty() && !tvecs.empty() ){

			CV_Assert(rvecs[0].type() == tvecs[0].type());
			Mat bigmat((int)rvecs.size(), 6, rvecs[0].type());
			for( int i = 0; i < (int)rvecs.size(); i++ ){
				Mat r = bigmat(Range(i, i+1), Range(0,3));
				Mat t = bigmat(Range(i, i+1), Range(3,6));

				CV_Assert(rvecs[i].rows == 3 && rvecs[i].cols == 1);
				CV_Assert(tvecs[i].rows == 3 && tvecs[i].cols == 1);
      
				r = rvecs[i].t();
				t = tvecs[i].t();
        }
        cvWriteComment( *fs, "Rotation vector + Translation vector", 0 );
        fs << "extrinsic_parameters" << bigmat;
    }
		fs.release();
}

int main(){

	cv::Size patternsize = cv::Size(9,6);//esquinas interiores del tablero de ajedrez
	std::vector<cv::Point3f> corners3D;
	std::vector<cv::Point2f> corners2D;//findChessboardCorners guarda los puntos del tablero aqui
	std::vector<std::vector<cv::Point2f>> coord2D;//Ubicacion de las esquinas detectadas en la imagen
	std::vector<std::vector<cv::Point3f>> coord3D;//Ubicacion real de los puntos 3D

	std::string direccion = "C:\\Users\\NilssenSteban\\Pictures\\Saved Pictures\\Cam";
	std::stringstream imgs;

	calcChessboardCorners(patternsize,18,corners3D);

	Mat img, imgGray;
	bool found;

	for (int  i = 0; i < 14; i++){

		imgs << direccion << i << ".jpg";
		img = imread(imgs.str().c_str());
		cvtColor(img, imgGray, COLOR_BGR2GRAY);
		imgs=std::stringstream();

		found = findChessboardCorners(imgGray,patternsize,corners2D,CALIB_CB_ADAPTIVE_THRESH + 
			                                                        CALIB_CB_NORMALIZE_IMAGE + 
																	CALIB_CB_FAST_CHECK);
		if( found) {
	
			cornerSubPix(imgGray, corners2D, Size(11, 11), Size(-1, -1),TermCriteria( 
				                                                        TermCriteria::EPS + 
																		TermCriteria::COUNT, 30, 0.1 ));
			drawChessboardCorners(img,patternsize,Mat(corners2D),found);
			coord2D.push_back(corners2D);
			coord3D.push_back(corners3D);
		} 
		 namedWindow("image",WINDOW_AUTOSIZE);
		 imshow("image", img);
		 waitKey(1500);
	}

	cvDestroyWindow("image");

	Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
	Mat distCoeffs = Mat::zeros(8, 1, CV_64F);
	std::vector<Mat> rvecs;
	std::vector<Mat> tvecs;
   
	double rms = calibrateCamera(coord3D, coord2D, img.size(), cameraMatrix,
                  distCoeffs, rvecs, tvecs, 
									CALIB_FIX_PRINCIPAL_POINT +
									CALIB_FIX_ASPECT_RATIO +
									CALIB_ZERO_TANGENT_DIST 
									,TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 30, 2.22e-16));

	std::cout << "RMS: " << rms << std::endl;
	std::cout << "Camera matrix: " << cameraMatrix << std::endl;
	std::cout << "Distortion _coefficients: " << distCoeffs << std::endl;

	saveparams("C:\\Users\\NilssenSteban\\Pictures\\Saved Pictures\\DataCam.yml",cameraMatrix,distCoeffs,
				rvecs,tvecs,rms);

	Mat imageUndistorted,image;
	VideoCapture capture = VideoCapture(0);
	namedWindow("imgOriginal",WINDOW_AUTOSIZE);
	namedWindow("imgCalibrada",WINDOW_AUTOSIZE);

	while(1){

    capture >> image;
    undistort(image, imageUndistorted, cameraMatrix, distCoeffs);//corrigo distorsion radial

    imshow("imgOriginal", image);
    imshow("imgCalibrada", imageUndistorted);
    waitKey(200);
	}
	waitKey(0);
	getchar();
	return 0;
}