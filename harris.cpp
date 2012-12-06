#include <cv.h>
#include <highgui.h>

void plot_harris_points (IplImage *image, void *filtered_coords) {
  
  cvNamedWindow ("HarrisPoints", 0);
  cvShowImage ("HarrisPoints", image);
  cvWaitKey ();
  cvDestroyWindow ("HarrisPoints");
  
  return;
}
