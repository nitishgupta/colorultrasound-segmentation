#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include<iostream>
#include<string>
#include<sstream>
#include<math.h>

using namespace std;
using namespace cv; 

vector<Point> level_set_based(Mat src, Mat *dst, int x, int y, int mode, double thresh)
{
	src = src > thresh;
	vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
	
	Point pt;
	if(mode == 0)
	{ 
		pt.x = x;
		pt.y = y;
	}
	
	else 
	{
		pt.x = x + (*dst).cols/2 ;
		pt.y = y; 
	}

	src.convertTo(src, CV_8UC1);

	findContours( src, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, pt);
	int m = 0;
	int sm =0;
	double max, sec_max;
	
	if(contourArea(contours[1]) < contourArea(contours[0]))
	{
		max = contourArea(contours[0]);
		sec_max=contourArea(contours[1]);
		m = 0; sm = 1;
	}
	else
	{
		max=contourArea(contours[1]);
		sec_max=contourArea(contours[0]);
		sm = 0; m = 1;
	}
	for(int i=2;i<Mat(contours).rows;i++)
	{
		if(contourArea(contours[i]) > max)
		{                              
			sec_max=max;
			sm = m;
			max=contourArea(contours[i]);
			m = i;
		}
		if(contourArea(contours[i])<max && contourArea(contours[i])>sec_max)
		{
			sec_max=contourArea(contours[i]);
			sm = i;
		}
	}

	return contours[sm];

}