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

void calculate_xgrad (int w, int h, unsigned char *src, unsigned char * dst) {
  int x,y, sum;

  for (y = 1; y < h -1; y++) {
    for (x = 1; x < w -1; x++) {
      /***************************
       * Racunamo x gradijent    *
       * prema Sobelovoj matrici *
       * -1 0 1                  *
       * -2 0 2                  *
       * -1 0 1                  *
       ***************************/
      sum =
          -     src[(y - 1) * w + (x - 1)]
          +     src[(y - 1) * w + (x + 1)]
          - 2 * src[(y    ) * w + (x - 1)]
          + 2 * src[(y    ) * w + (x + 1)]
          -     src[(y + 1) * w + (x - 1)]
          +     src[(y + 1) * w + (x + 1)];

      dst[y* w + x] = abs(sum);
    }
  }
}

void calculate_ygrad (int w, int h, unsigned char *src, unsigned char * dst) {
  int x,y, sum;

  for (y = 1; y < h -1; y++) {
    for (x = 1; x < w -1; x++) {
      /***************************
       * Racunamo y gradijent    *
       * prema Sobelovoj matrici *
       * -1 -2 -1                *
       *  0  0  0                *
       *  1  2  1                *
       ***************************/
      sum =
          -     src[(y - 1) * w + (x - 1)]
          - 2 * src[(y - 1) * w + (x    )]
          -     src[(y - 1) * w + (x + 1)]
          +     src[(y + 1) * w + (x - 1)]
          + 2 * src[(y + 1) * w + (x    )]
          +     src[(y + 1) * w + (x + 1)];

      dst[y* w + x] = abs(sum);
    }
  }
}

void computeHarrisResponse(Mat &src, Mat &dest) {
  CvSize imgSize = src.size();

  Mat srcBlurred = Mat (imgSize, CV_8UC1);
  grayscaleGaussianBlur (src, srcBlurred, 5);
  int numPixels = imgSize.width * imgSize.height;
  unsigned char *imgData = (unsigned char *) malloc (sizeof (unsigned char) * numPixels);

  memcpy (imgData, srcBlurred.data, sizeof (unsigned char) * numPixels);

  unsigned char *xGrad = (unsigned char *) calloc (numPixels, sizeof (unsigned char));
  unsigned char *yGrad = (unsigned char *) calloc (numPixels, sizeof (unsigned char));
  

  calculate_xgrad (imgSize.width, imgSize.height, imgData, xGrad);
  calculate_ygrad (imgSize.width, imgSize.height, imgData, yGrad);

  
  

  // Mat compatMat_XGrad = Mat (imgSize, CV_8UC1);
  // compatMat_XGrad.data = xGrad;
  // imshow ("Xgrad", compatMat_XGrad);
  // 
  // Mat compatMat_YGrad = Mat (imgSize, CV_8UC1);
  // compatMat_YGrad.data = yGrad;
  // imshow ("Ygrad", compatMat_YGrad);



  return;
}
