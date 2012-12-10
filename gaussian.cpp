#include "gaussian.h"

using namespace cv;
using namespace std;

#define PI 3.141592654

void GaussianBlur(Mat source, Mat destination, int sizeX, int sizeY) {
	// Assert that the kernel sizes are odd and positive
	assert(sizeX % 2 == 1);
	assert(sizeY % 2 == 1);

    int radiusX = (sizeX-1)/2;
    int radiusY = (sizeY-1)/2;

	double *kernelX = new double[sizeX];
	double *kernelY = new double[sizeY];

    // sigma has to be r/2 in order to get a normalized kernel
    double sigmaX = radiusX/2.0;
    double sigmaY = radiusY/2.0;

    double x, y;

    // Calculate kernels
    for (int i = 0; i < sizeX; ++i) {
        x = abs(radiusX - i);
//        cout << "x = " << x << endl;
        kernelX[i] = (1/sqrt(2*PI*sigmaX*sigmaX))*exp(-(x*x)/(2*sigmaX*sigmaX));
        cout << "kernelX[" << i << "] = " << kernelX[i] << endl;
    }
    if(sizeX == sizeY) {
        delete [] kernelY;
        kernelY = kernelX;
    }
    else {
        for (int i = 0; i < sizeY; ++i) {
            y = abs(radiusY-i);
            kernelY[i] = (1/sqrt(2*PI*sigmaY*sigmaY))*exp(-(y*y)/(2*sigmaY*sigmaY));
            cout << "kernelY[" << i << "] = " << kernelY[i] << endl;
        }
    }




    // dealocate
    delete [] kernelX;
    if(sizeY != sizeX) {
        delete [] kernelY;
    }
}
