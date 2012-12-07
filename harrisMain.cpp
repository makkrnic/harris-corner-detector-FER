#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <cxcore.h>
#include <vector>

#include "harris.hpp"

int main (int argc, char *argv[]) {
  IplImage *img = cvCreateImage(cvSize(800,500),IPL_DEPTH_8U,1);
  std::vector<CvPoint> points;
  CvPoint tmpPoint;

  tmpPoint.x = 1;
  tmpPoint.y = 1;

  points.push_back (tmpPoint);

  plot_harris_points (img, points);

  return 0;
}
