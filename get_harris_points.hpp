#ifndef _GET_HARRIS_POINTS_HPP
#define _GET_HARRIS_POINTS_HPP

#include <iostream>
#include <list>
#include <math.h>
#include <algorithm>
#include <array>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

list<Point> get_harris_points (Mat &harrisim, int min_dist = 10, float thresh = 0.1);
#endif
