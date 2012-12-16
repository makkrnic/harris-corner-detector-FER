#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <cxcore.h>
#include <vector>
#include <getopt.h>

#include "gaussian.h"
#include "get_harris_points.hpp"
#include "harris.hpp"

int main (int argc, char *argv[]) {
  Mat img, imgBlurred;
  std::vector<CvPoint> points;
  CvPoint tmpPoint;

  char usage[] = "-i ime_slike";
  char opt;
  char optstring[] = "i:";
  char *imgName;

  if (argc < 2) {
    fprintf (stderr, "Upotreba: %s %s\n", argv[0], usage);
    return 1;
  }

  while ((opt = getopt (argc, argv, optstring)) != -1) {
    switch (opt) {
      case 'i':
        imgName = optarg;
      break;

      default:
        fprintf (stderr, "Nepodrzana opcija.\n");
        fprintf (stderr, "Upotreba: %s %s\n", argv[0], usage);
        return 1;
    }
  }

  //if ((img = cvLoadImage (imgName, CV_LOAD_IMAGE_GRAYSCALE)) == NULL) {
  img = imread (imgName, CV_LOAD_IMAGE_GRAYSCALE);
  if (img.empty()) {
    fprintf (stderr, "Greska pri citanju fajla.\n");
    return 2;
  }

  
  imgBlurred = Mat (img.size(), CV_8UC1, Scalar::all(0));
  // Prvo treba zagladiti sliku:
  grayscaleGaussianBlur (img, imgBlurred, 11);
  


  points = get_harris_points (img, 10, 0.1); 



  // Testni podaci
  tmpPoint.x = 300;
  tmpPoint.y = 300;

  points.push_back (tmpPoint);

  plotHarrisPoints (imgBlurred, points);

  return 0;
}
