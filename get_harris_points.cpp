#include "get_harris_points.hpp"

using namespace cv;
using namespace std;



int nonzero_t(Mat a, std::vector<CvPoint>& coords)
{
  int k = 0;
	int i;

	for(i = 0; i < a.rows; i++)
	{
		const double* Mi = a.ptr<double>(i);
		for(int j = 0; j < a.cols; j++)
		{
			if (Mi[j]!=0)
			{
        coords.push_back (cvPoint(i,j));
				//coords[k++] = j;
			}
		}
	}
	return k;
}

void zeros(Mat& allowed_locations, int x, int y, int min_dist)
{
	int i;
	int j;
	for (i = 0; i < x; ++i)
	{
		for (j = 0; j < y; ++j)
		{
			if((i>min_dist)&&(i<x-min_dist)&&(j>min_dist)&&(j<y-min_dist))
			{
				allowed_locations.at<int>(i,j) = 1;
			}
			else
			{
				allowed_locations.at<int>(i,j) = 0;
			}
		}
	}
	return;
}

void prosiri(Mat& allowed_locations, int x, int y, int min_dist)
{
	int i;
	int j;
	for (i = x-min_dist+1; i < x+min_dist; ++i)
	{
		for (j = y-min_dist+1; j < y+min_dist; ++j)
		{
			allowed_locations.at<int>(i,j) = 0;
		}
	}
}

list<int> vrijednosna(Mat harrisim, std::vector<CvPoint> coords, int k)
{
	list<int> povratna;
	int i;

	for(i = 0; i < k; i++)
	{
		povratna.push_back(harrisim.at<double>(coords[i].x, coords[i].y));
	}

	return povratna;
}

std::vector<CvPoint> get_harris_points (Mat harrisim, int min_dist, float threshold)
    //""" Return corners from a Harris response image min_dist is the minimum number of pixels separating corners and image boundary. """	
	{
    std::vector<CvPoint>  filtered_coords;
    Mat harrisim_t=Mat();
	list<int> candidate_values;
	list<int>::iterator i;
    int max;
    float corner_threshold;
	//int * coords;
    std::vector<CvPoint> coords;
	int k;

	
	harrisim_t = harrisim.clone();
    // find top corner candidates above a threshold
	MatConstIterator_<double> it = harrisim.begin<double>(), it_end = harrisim.end<double>();
	max = *max_element(it, it_end);    
    corner_threshold = max * threshold;
   
	
	for(int i = 0; i < harrisim_t.rows; i++)
	{
		//const double* Mi = harrisim_t.ptr<double>(i);
		for(int j = 0; j < harrisim_t.cols; j++)
		{
			if (harrisim_t.at<double>(i,j)>corner_threshold)
			{
				harrisim_t.at<double>(i,j)=1;
			}
			else
			{
				harrisim_t.at<double>(i,j)=0;
			}
		}
	}


    //coords = (int *) malloc (999999*sizeof(int));
    // get coordinates of candidates
    k = nonzero_t(harrisim_t, coords);

    
    // ...and their values
    //candidate_values = [harrisim[c[0],c[1]] for c in coords];
	  candidate_values = vrijednosna(harrisim, coords, k);
    
    // sort candidates
    candidate_values.sort();


    // store allowed point locations in array
    //int *allowed_locations = (int *) malloc (harrisim.rows * harrisim.cols * sizeof(int));//[harrisim.rows][harrisim.cols];

    //matrix<int> allowed_locations(harrisim.rows, harrisim.cols);

    Mat allowed_locations (harrisim.size(), CV_8UC1);
    zeros(allowed_locations, harrisim.rows, harrisim.cols, min_dist);

    //allowed_locations[min_dist:-min_dist,min_dist:-min_dist] = 1;
    
	  int b=-1;
    // select the best points taking min_distance into account
    for (i=candidate_values.begin(); i!=candidate_values.end(); i++)
    {
		  b++;
      if (allowed_locations.at<int> (coords[b].x, coords[b].y) == 1)
		  {
			// filtered_coords.push_back (coords[b]);
			// filtered_coords.push_back (coords[b+1]);
        CvPoint tmpPoint = coords[b]; //{coords[b], coords[b+1]};
        filtered_coords.push_back(tmpPoint);
			  prosiri (allowed_locations, coords[b].x, coords[b].y, min_dist);
		  }
    }
  return filtered_coords;
}
