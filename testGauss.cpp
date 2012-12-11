#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "gaussian.h"

using namespace cv;
using namespace std;

int main( int argc, char** argv ) {
    if( argc != 3) {
        cout <<" Usage: " << argv[0] << " ImageToBlur BlurRadius" << endl;
        return -1;
    }

    Mat image, blurred;
    image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);   // Read the file

    // create empty result matrix for grayscale image - 8-bit, unsigned, 1-channel
    blurred = Mat(image.size(), CV_8UC1, Scalar(0));

    if(! image.data ) {                              // Check for invalid input
        cout <<  "Could not open or find the image" << endl ;
        return -1;
    }

    grayscaleGaussianBlur(image, blurred, 5, 5);

    namedWindow( "Original image", CV_WINDOW_AUTOSIZE );// Create a window for display.
    namedWindow( "Gaussian blur", CV_WINDOW_AUTOSIZE );// Create a window for display.

    imshow( "Original image", image );                   // Show our image inside it.
    imshow( "Gaussian blur", blurred );                   // Show our image inside it.

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}
