#include <cv.h>
#include <highgui.h>

#include "harris.hpp"

void plotHarrisPoints (IplImage *image, std::vector<CvPoint>& filtered_coords) {
  
  cvNamedWindow ("HarrisPoints", 0);
  cvShowImage ("HarrisPoints", image);
  cvWaitKey ();
  cvDestroyWindow ("HarrisPoints");
  
  return;
}
