#include <cv.h>
#include <highgui.h>
#include <list>

#include "harris_plot.hpp"

void plotHarrisPoints (Mat imageMat, std::list<Point>& filtered_coords) {
  //CvSize imSize = cvSize (image->width, image->height);
  IplImage image = imageMat;
  CvSize imSize = imageMat.size();

  IplImage *imgColored = cvCreateImage (imSize, IPL_DEPTH_8U, 3);

  cvCvtColor (&image, imgColored, CV_GRAY2BGR);

  // crvena boja. Iz nekog razloga je BGR umjesto RGB.
  CvScalar pointColor = cvScalar (0, 0, 255);
  fprintf (stderr, "\nBroj tocaka: %d\n", filtered_coords.size());

  for (std::list<Point>::iterator point = filtered_coords.begin();
        point != filtered_coords.end(); ++point) {
    // Dodaj svaku tocku na sliku koja ce se prikazati.
    //fprintf (stderr, "Dodajem tocku: (%d, %d)\n", point->x, point->y);
    cvCircle (imgColored, *point, 3, pointColor, CV_FILLED);
  }
  
  cvNamedWindow ("HarrisPoints");
  cvResizeWindow ("HarrisPoints", imSize.width, imSize.height);

  cvShowImage ("HarrisPoints", imgColored);
  while (cvWaitKey (0) != 0x10001B);
  cvDestroyWindow ("HarrisPoints");

  cvReleaseImage (&imgColored);
  
  return;
}
