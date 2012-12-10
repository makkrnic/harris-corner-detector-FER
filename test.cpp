#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "gaussian.h"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: " << argv[0] << " ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image, blurred;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    GaussianBlur(image, blurred, 5, 5);

    namedWindow( "Original image", CV_WINDOW_AUTOSIZE );// Create a window for display.
    namedWindow( "Gaussian blur", CV_WINDOW_AUTOSIZE );// Create a window for display.

    imshow( "Original image", image );                   // Show our image inside it.
    imshow( "Gaussian blur", blurred );                   // Show our image inside it.

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}
