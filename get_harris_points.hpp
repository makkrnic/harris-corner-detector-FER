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

using namespace cv;
using namespace std;

int nonzero_t(Mat a, std::vector<CvPoint>& coords);
//void zeros(int **allowed, int x, int y, int min_dist);
void zeros(Mat& allowed_locations, int x, int y, int min_dist);
void prosiri(Mat& allowed_locations, int x, int y, int min_dist);
list<int> vrijednosna(Mat harrisim, std::vector<CvPoint> coords, int k);
std::vector<CvPoint> get_harris_points (Mat harrisim, int min_dist = 10, float threshold = 0.1);

#endif
