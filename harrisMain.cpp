#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <cxcore.h>

#include "harris.hpp"

int main (int argc, char *argv[]) {
  IplImage *img = cvCreateImage(cvSize(800,500),IPL_DEPTH_8U,1);

  plot_harris_points (img, NULL);

  return 0;
}
