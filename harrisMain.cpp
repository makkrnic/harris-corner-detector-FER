#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <cxcore.h>
#include <vector>

#include "harris.hpp"

int main (int argc, char *argv[]) {
  IplImage *img = cvCreateImage(cvSize(800,500),IPL_DEPTH_8U, 3);
  std::vector<CvPoint> points;
  CvPoint tmpPoint;

  tmpPoint.x = 300;
  tmpPoint.y = 300;

  points.push_back (tmpPoint);

  plotHarrisPoints (img, points);

  return 0;
}
