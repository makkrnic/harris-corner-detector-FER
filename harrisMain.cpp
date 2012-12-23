#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <cxcore.h>
#include <vector>
#include <unistd.h>

#include "gaussian.h"
#include "get_harris_points.hpp"
#include "harris_plot.hpp"
#include "computeHarrisResponse.h"

int main (int argc, char *argv[]) {
  Mat img, harrisResponse;
  std::list<Point> points;
  Point tmpPoint;

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

  img = imread (imgName, CV_LOAD_IMAGE_GRAYSCALE);
  if (img.empty()) {
      cerr << "Greska pri citanju fajla.\n" << endl;
      return 2;
  }

  
  computeHarrisResponse(img, harrisResponse);

  points = get_harris_points (harrisResponse, 10, 0.1); 

  plotHarrisPoints (img, points);

  return 0;
}
