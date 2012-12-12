#include <cv.h>
#include <highgui.h>

#include "harris.hpp"

void plotHarrisPoints (Mat imageMat, std::vector<CvPoint>& filtered_coords) {
  //CvSize imSize = cvSize (image->width, image->height);
  IplImage image = imageMat;
  CvSize imSize = imageMat.size();

  IplImage *imgColored = cvCreateImage (imSize, IPL_DEPTH_8U, 3);

  cvCvtColor (&image, imgColored, CV_GRAY2BGR);

  // crvena boja. Iz nekog razloga je BGR umjesto RGB.
  CvScalar pointColor = cvScalar (0, 0, 255);

  for (std::vector<CvPoint>::iterator point = filtered_coords.begin();
        point != filtered_coords.end(); ++point) {
    // Dodaj svaku tocku na sliku koja ce se prikazati.
    cvCircle (imgColored, *point, 5, pointColor, CV_FILLED);
  }
  
  cvNamedWindow ("HarrisPoints", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_NORMAL);
  cvResizeWindow ("HarrisPoints", imSize.width, imSize.height);

  cvShowImage ("HarrisPoints", imgColored);
  cvWaitKey ();
  cvDestroyWindow ("HarrisPoints");

  cvReleaseImage (&imgColored);
  
  return;
}
