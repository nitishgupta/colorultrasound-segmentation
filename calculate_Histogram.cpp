#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include<iostream>
#include<string>
#include<sstream>
#include<math.h>

using namespace std;
using namespace cv; 


MatND calculate_Histogram(Mat* enhanced_image)
{
		
	int histSize[1] = {256};
	float hranges[2] = {0.0,256.0};
	const float* ranges[1];
	ranges[0] = hranges;
	int channels[1] = {0};

	MatND hist;//compute histogram 
	calcHist(enhanced_image,
			1,// 1 image only 
			channels,//channels 
			cv::Mat(),// no mask 
			hist,// the result 
			1,// 1D histogram 
			histSize,// number of bins 
			ranges// pixel value range 
			);

	return hist;
}




	