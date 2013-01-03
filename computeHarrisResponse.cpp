#include "computeHarrisResponse.h"
#include "gaussian.h"

using namespace cv;
using namespace std;

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

/**
 * Uzima matrice m1 i m2, velicine w*h, mnozi odgovarajuce elemente:
 * (0,0) * (0,0); (0,1) * (0,1)... i sprema ih u 
 * matricu dest.
 * dest treba biti alocirana.
 */
void emul (int w, int h, unsigned char *m1, unsigned char *m2, unsigned char *dest) {
  int x, y = 0;

  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      dest[y * w + x] = (unsigned char)(((ushort)m1[y * w + x] * (ushort)m2[y * w + x])/65535.0 * 255);
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

  free(imgData);

  Mat compatMat_XGrad = Mat (imgSize, CV_8UC1);
  compatMat_XGrad.data = xGrad;
  //imshow ("Xgrad", compatMat_XGrad);
  
  Mat compatMat_YGrad = Mat (imgSize, CV_8UC1);
  compatMat_YGrad.data = yGrad;
  //imshow ("Ygrad", compatMat_YGrad);

  grayscaleGaussianBlur (compatMat_XGrad, compatMat_XGrad, 5);
  grayscaleGaussianBlur (compatMat_YGrad, compatMat_YGrad, 5);


  unsigned char *Ixx = (unsigned char *) malloc (numPixels * sizeof (unsigned char));
  unsigned char *Ixy = (unsigned char *) malloc (numPixels * sizeof (unsigned char));
  unsigned char *Iyy = (unsigned char *) malloc (numPixels * sizeof (unsigned char));
  
  
  emul (imgSize.width, imgSize.height, xGrad, xGrad, Ixx);
  emul (imgSize.width, imgSize.height, xGrad, yGrad, Ixy);
  emul (imgSize.width, imgSize.height, yGrad, yGrad, Iyy);

  free(xGrad);
  free(yGrad);

  Mat compatMat_Ixx = Mat (imgSize, CV_8UC1);
  compatMat_Ixx.data = (uchar *)Ixx;
  imshow ("Ixx", compatMat_Ixx);
  //cvNamedWindow ("Ixx");
  //imshow("Ixx", compatMat_Ixx);
  //while (cvWaitKey (0) != 0x10001B);

  //fprintf (stderr, "Vrijednosti xgrada:\n");
  //for (int i = 0; i < imgSize.height; i++) {
  //  for (int j = 0; j < imgSize.width; j++) {
  //    fprintf (stderr, "(%3d, %3d): %i\n", i, j, Ixx[imgSize.width * i + j]);
  //    getchar();
  //  }
  //}


  
  Mat compatMat_Ixy = Mat (imgSize, CV_8UC1);
  compatMat_Ixy.data = (uchar *)Ixy;
  imshow ("Ixy", compatMat_Ixy);

  Mat compatMat_Iyy = Mat (imgSize, CV_8UC1);
  compatMat_Iyy.data = (uchar *)Iyy;
  imshow ("Iyy", compatMat_Iyy);

  grayscaleGaussianBlur (compatMat_Ixx, compatMat_Ixx, 5);
  grayscaleGaussianBlur (compatMat_Iyy, compatMat_Iyy, 5);
  grayscaleGaussianBlur (compatMat_Ixy, compatMat_Ixy, 5);

  
  double *harrisResponse = (double *) malloc (numPixels * sizeof (double));

  for (int y = 0; y < imgSize.height; y++) {
    for (int x = 0; x < imgSize.width; x++) {
      double det = (int)Ixx[imgSize.width * y + x] * (int)Iyy[imgSize.width * y + x] - (int)Ixy[imgSize.width * y + x] * (int)Ixy[imgSize.width * y + x];
      double trace = (int)Ixx[imgSize.width * y + x] + (int)Iyy[imgSize.width * y + x];
      if (det != 0) {
//        printf ("\n\n\nDet nije 0\n\n\n");
      }

      harrisResponse[imgSize.width * y + x] = det - 0.06 * (double)trace * (double)trace;
    }
  }


  Mat compatMat_resp = Mat (imgSize, CV_64F);
  compatMat_resp.data = (uchar *)harrisResponse;
  //dest.data = (uchar *)Ixy;
  imshow ("response", compatMat_resp);

  memcpy (dest.data, harrisResponse, sizeof (double) * numPixels);

  free(Ixx);
  free(Ixy);
  free(Iyy);
  free(harrisResponse);
  
  return;
}
