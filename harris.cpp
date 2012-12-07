#include <cv.h>
#include <highgui.h>

#include "harris.hpp"

void plotHarrisPoints (IplImage *image, std::vector<CvPoint>& filtered_coords) {
  // crvena boja. Iz nekog razloga je BGR umjesto RGB.
  CvScalar pointColor = cvScalar (0, 0, 255);

  for (std::vector<CvPoint>::iterator point = filtered_coords.begin();
        point != filtered_coords.end(); ++point) {
    // Dodaj svaku tocku na sliku koja ce se prikazati.
    cvCircle (image, *point, 5, pointColor, 5);
  }
  
  cvNamedWindow ("HarrisPoints", 0);
  cvShowImage ("HarrisPoints", image);
  cvWaitKey ();
  cvDestroyWindow ("HarrisPoints");
  
  return;
}
