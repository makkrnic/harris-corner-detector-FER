#include "gaussian.h"

using namespace cv;
using namespace std;

void GaussianBlur(Mat source, Mat destination, unsigned int sizeX, unsigned int sizeY, double sigmaX, double sigmaY) {
	// Assert that the sizes are odd
	assert(sizeX % 2 != 0);
	assert(sizeY % 2 != 0);
	
}