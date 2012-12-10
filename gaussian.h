#include <opencv2/core/core.hpp>
#include <cassert>

void GaussianBlur(cv::Mat source, cv::Mat destination, unsigned int sizeX, unsigned int sizeY, double sigmaX, double sigmaY);