#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <cxcore.h>
#include <vector>

#include "harris.hpp"

int main (int argc, char *argv[]) {
  IplImage *img;
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

  if ((img = cvLoadImage (imgName, CV_LOAD_IMAGE_GRAYSCALE)) == NULL) {
    fprintf (stderr, "Greska pri citanju fajla.\n");
    return 2;
  }




  // Testni podaci
  tmpPoint.x = 300;
  tmpPoint.y = 300;

  points.push_back (tmpPoint);

  plotHarrisPoints (img, points);

  return 0;
}
