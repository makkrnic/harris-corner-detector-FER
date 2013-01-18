#ifndef COMPUTEHARRISRESPONSE_H
#define COMPUTEHARRISRESPONSE_H

#include <cstdlib>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

void computeHarrisResponse(cv::Mat &src, cv::Mat &dest, int sigma);

#endif
