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

bool debug = false;

int main (int argc, char *argv[]) {
  Mat img, harrisResponse, imgBlurred;
  std::list<Point> points;
  Point tmpPoint;

  char usage[] = "[-d] [-s] [-k kernel_size] [-b integration_sigma ] [-t treshold] -i ime_slike";
  char opt;
  char optstring[] = "si:k:t:b:d";
  char *imgName = NULL;
  bool disableBlur = false;
  unsigned char kernelSize = 3;
  float treshold = 0.1;
  int sigma = 5;

  if (argc < 3) {
    fprintf (stderr, "Upotreba: %s %s\n", argv[0], usage);
    return 1;
  }

  while ((opt = getopt (argc, argv, optstring)) != -1) {
    switch (opt) {
      case 'i':
        imgName = optarg;
      break;

      case 's':
        disableBlur = true;
      break;

      case 'k':
        kernelSize = atoi(optarg)/2 * 2 + 1;
        assert (kernelSize > 2);
      break;

      case 'b':
        sigma = atoi(optarg);
        assert (sigma > 0);
      break;

      case 't':
        treshold = atof (optarg);
        assert (treshold > 0);
      break;

      case 'd':
        debug = true;
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

  
  imgBlurred = Mat (img.size(), CV_8UC1, Scalar::all(0));

  if (disableBlur == true) {
    imgBlurred = img;
  }
  else {
    // Prvo treba zagladiti sliku:
    grayscaleGaussianBlur (img, imgBlurred, kernelSize, kernelSize);
  }

  harrisResponse = Mat (img.size(), CV_64F);

  
  computeHarrisResponse(imgBlurred, harrisResponse, sigma);

  //debug_saveResponse (harrisResponse);
  //debug_loadResponse (harrisResponse);
  
  // for(int row = 0; row < harrisResponse.rows; ++row) {
  //   for(int col = 0; col < harrisResponse.cols; ++col) {
  //     fprintf (stderr, "Element: %f\n", harrisResponse.at<int>(row,col));
  //   }
  // }   
  
  if (debug) imshow ("harrisResponse", harrisResponse);

  points = get_harris_points (harrisResponse, 10, treshold);

  plotHarrisPoints (img, points);

  return 0;
}
