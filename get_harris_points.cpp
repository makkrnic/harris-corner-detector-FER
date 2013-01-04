#include "get_harris_points.hpp"

using namespace cv;
using namespace std;

class HarrisPointValue {
public:
    HarrisPointValue(Point p, double val);
    Point point;
    double value;
};

HarrisPointValue::HarrisPointValue(Point p, double val) {
    point = p;
    value = val;
}

void nonzero_t(Mat &harrisim_t, list<Point> &coords) {
//    int num = countNonZero(harrisim_t);

    for(int i = 0; i < harrisim_t.rows; i++) {
        for(int j = 0; j < harrisim_t.cols; j++) {
            if ((int)harrisim_t.at<uchar>(i, j) != 0) {
                coords.push_back(Point(j, i));
            }
        }
    }
    return ;
}

void vrijednosna(Mat &harrisim, list<Point> &coords, list<HarrisPointValue> &candidate_values) {
    for(list<Point>::iterator it = coords.begin(); it != coords.end(); ++it) {
        candidate_values.push_back(HarrisPointValue(*it, harrisim.at<double>(*it)));
    }

    return ;
}

bool compareValue(HarrisPointValue &first, HarrisPointValue &second) {
    if(first.value > second.value) {
        return true;
    }

    return false;
}

bool checkMask(Mat &mask, Point point, int min_dist) {
    if(mask.at<uchar>(point) == 0) {
        return false;
    }
    else {
        for (int i = point.x - min_dist + 1; i < point.x + min_dist; ++i) {
            for( int j = point.y - min_dist + 1; j < point.y + min_dist; ++j ) {
                mask.at<uchar>(j, i) = 0; 
        }
    }
        return true;
    }
}

void thresholdMat(Mat &src, Mat &dest, double thresh, int high) {
    CV_Assert(src.size() == dest.size());
    for (int i = 0; i < src.rows; ++i) {
        for( int j = 0; j < src.cols; ++j ) {
            if(src.at<double>(i, j) >= thresh) {
                dest.at<uchar>(i, j) = high;
            }
            else {
                dest.at<uchar>(i, j) = 0;
            }
        }
    }

    return ;
}

list<Point> get_harris_points (Mat &harrisim, int min_dist, float thresh) {
//""" Return corners from a Harris response image min_dist is the minimum number of pixels separating corners and image boundary. """

    list<Point> coords, filtered_coords;
    Mat harrisim_t = Mat(harrisim.size(), CV_8UC1, Scalar::all(0));
    list<HarrisPointValue> candidate_values;

    // find top corner candidates above a threshold
    double max;
    // finds the max value
    minMaxLoc(harrisim, NULL, &max);
    double corner_threshold = max * thresh;

    // thresholding of corners - sets everything above threshold to 255, and below to 0
    thresholdMat(harrisim, harrisim_t, corner_threshold, 255);

    // get coordinates of candidates
    nonzero_t(harrisim_t, coords);

    // ...and their values
    vrijednosna(harrisim, coords, candidate_values);

    cout << "Kandidati: " << candidate_values.size() << endl;

    // sort candidates
    candidate_values.sort(compareValue);

    // store allowed point locations in array
    Mat mask = Mat(harrisim.size(), CV_8UC1, Scalar::all(255));
    for( int i = 0; i < min_dist; ++i ) {
        mask.row(i).setTo(Scalar(0));
        mask.row(mask.rows-i-1).setTo(Scalar(0));
        mask.col(i).setTo(Scalar(0));
        mask.col(mask.cols-i-1).setTo(Scalar(0));
    }

//    list<HarrisPointValue>::const_iterator iter, end;
    // select the best points taking min_distance into account
    for (auto iter = candidate_values.begin(); iter != candidate_values.end(); ++iter) {
//        cout << " " << iter->point.x << ", " << iter->point.y << endl;
        if (checkMask(mask, iter->point, min_dist)) {
            filtered_coords.push_back(Point(iter->point));
        }
    }
    cout << "Kutevi: " << filtered_coords.size() << endl;
    return filtered_coords;
}
