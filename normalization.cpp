#include "stdafx.h"
#include<\opencv\build\include\opencv2\imgproc\imgproc.hpp>
#include <cv.h>
#include <highgui.h>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>

using namespace std;
using namespace cv;

Mat normalization(Mat input, float olow, float ohigh)
{
	double minVal; 
    double maxVal; 
    Point minLoc; 
    Point maxLoc;

    minMaxLoc( input, &minVal, &maxVal, &minLoc, &maxLoc);
    cout<<"\n sssMIN : "<<minVal<<"    sssmax: "<<maxVal<<"\n";
	
	Mat output;
	Mat min = Mat::ones(input.size(), CV_32FC1);
	Mat low = Mat::ones(input.size(), CV_32FC1);
	min = min.mul(minVal - 0.01);
	low = low.mul(olow);

	input = input - min;
	input = input.mul(1/(maxVal - minVal));
	input = input.mul(ohigh - olow);
	input = input + low;

	return input;
}


