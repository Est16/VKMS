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


int main (){

	std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(100);

	Mat img;

	VideoCapture cam = VideoCapture(0);
	if(!cam.isOpened()){
		printf("!No se pudo abrir la camara!\n");
		getchar();//espera que se preione Enter para dejar seguir
		return -1;
	}

	bool SaveImage;
	int nimgs = 0;
	
	std::string direccion = "C:\\Users\\NilssenSteban\\Pictures\\Saved Pictures\\Cam";
	std::stringstream Saveimgs;

	for(;;){
	
	cam >> img;
	
	namedWindow("image",WINDOW_AUTOSIZE);
	imshow("image", img);
	
	if(waitKey(30) == 13){//espera 30ms y pregunta por ENTER

		Saveimgs << direccion << nimgs << ".jpg";
		SaveImage = imwrite(Saveimgs.str().c_str(), img, compression_params);
		Saveimgs = std::stringstream();
		nimgs++;
			if (!SaveImage){
				printf("!image %d saved failed!\n",nimgs);

			}else{
				printf("!image %d saved successful!\n",nimgs);
			}
			if (nimgs == 14){break;}
		}
	}
	getchar();
	return 0;
}
