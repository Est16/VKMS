#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main () {
  Mat img = imread("lena.jpg");
  CascadeClassifier cascade;
  if (cascade.load("haarcascade_frontalface_alt.xml")) {
    vector<Rect> faces;
    //cascade.detectMultiScale(img, faces, 1.1, 3, CV_HAAR_DO_CANNY_PRUNING, Size(30, 30), Size(200, 200));
    cascade.detectMultiScale(img, faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT, Size(30, 30), Size(200,200));
    
    printf("%zd face(s) are found.\n", faces.size());
    
    for (int i = 0; i < faces.size(); i++) {
      Rect r = faces[i];
      printf("a face is found at Rect(%d,%d,%d,%d).\n", r.x, r.y, r.width, r.height);
    }
  }
  
  return 0;
}
