#include "computeHarrisResponse.h"
#include "gaussian.h"

using namespace cv;
using namespace std;

inline double getElement(Mat &matrix, int i, int j) {
//    assert(matrix.type() == CV_8UC1);
    if (i < 0 || i >= matrix.rows || j < 0 || j >= matrix.cols) {
        return 0.0;
    }
    else {
        return matrix.at<double>(i, j);
    }
}

void computeHarrisResponse(Mat &src, Mat &dest) {

      int scale = 1;
      int delta = 0;
      int ddepth = CV_16S;
      int sigma = 3;
      int ksize = 3; //ovo je velicina prozora za sobel funkciju...tipa 3x3
      int wsize = 1;

      // provjeriti assertovima dimenzije i broj kanala

      Mat blurred = Mat(src.size(), CV_8UC1, Scalar::all(0));
      grayscaleGaussianBlur(src, blurred, 11);

      Mat derivX, derivY;
      Scharr( blurred, derivX, ddepth, 1, 0, scale, delta, BORDER_DEFAULT ); //e sad prva gausova derivacija po x smjeru
      Scharr( blurred, derivY, ddepth, 0, 1, scale, delta, BORDER_DEFAULT ); //prva gausova derivacija po y smjeru

      Mat dx = Mat(derivX.size(), CV_8UC1);
      Mat dy = Mat(derivY.size(), CV_8UC1);

      convertScaleAbs(derivX, dx);
      convertScaleAbs(derivY, dy);

      grayscaleGaussianBlur(dx, dx, 5);
      grayscaleGaussianBlur(dy, dy, 5);

      Mat Ixx = Mat(src.size(), CV_32F);
      Mat Ixy = Mat(src.size(), CV_32F);
      Mat Iyy = Mat(src.size(), CV_32F);

      Ixx = dx.mul(dx); //tu mnozim medusobno da dobijem Ix^2, Ixy i Iy^2
      Ixy = dx.mul(dy);
      Iyy = dy.mul(dy);

//      GaussianBlur( Ixx, Ixx, Size(0,0), sigma, BORDER_DEFAULT ); //opet mutim sa gausom za jos manji noise
//      GaussianBlur( Ixy, Ixy, Size(0,0), sigma, BORDER_DEFAULT ); //u pythonu je lik mutil dva put pa sam i ja
//      GaussianBlur( Iyy, Iyy, Size(0,0), sigma, BORDER_DEFAULT );

//      convertScaleAbs(Ixx, Ixx);
//      convertScaleAbs(Ixy, Ixy);
//      convertScaleAbs(Iyy, Iyy);

      Mat DetA = Mat(src.size(), CV_32F);
      Mat DetB = Mat(src.size(), CV_32F);
      Mat Trace = Mat(src.size(), CV_32F);
      Mat Score = Mat(src.size(), CV_32F);
      dest = Mat(src.size(), CV_32F);

      DetA = Ixx.mul(Iyy); //za determinantu dijelovi
      DetB = Ixy.mul(Ixy);
      Mat Det = Mat(src.size(), CV_32F);;
      subtract(DetA, DetB, Det); //tu izracunam determinantu

      Trace = Ixx+Iyy;
      divide(Det, Trace, Score);

      // Make it all positive, because we're only interested in the size of the change, not direction
      convertScaleAbs(Score, dest);

      imshow("blur", blurred);
      imshow("derivx", dx);
      imshow("derivy", dy);
      imshow("Ixx", Ixx);
      imshow("Ixy", Ixy);
      imshow("Iyy", Iyy);
      imshow("dest", dest);

      return ;
}

