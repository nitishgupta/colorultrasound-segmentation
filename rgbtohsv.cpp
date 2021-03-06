#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include<iostream>
#include<string>
#include<sstream>
#include<math.h>

using namespace std;
using namespace cv; 


void rgbtohsv(Mat image, Mat *hue)
{
	Mat r1,g1,b1;
	vector<Mat> m(3);
	split(image, m);
	float r,g,b;
	float eps = 0.000001;

	b1 = m[0]; 	g1 = m[1]; 	r1 = m[2];

	r1.convertTo(r1, CV_32FC1, 1.0/255.0);	g1.convertTo(g1, CV_32FC1, 1.0/255.0);	b1.convertTo(b1, CV_32FC1, 1.0/255.0);

	Mat img(image.size(),CV_32FC1);
	float max = -1;
	for(int i=0; i<image.rows; i++)
	{
		for(int j=0; j<image.cols; j++)
		{
			r = r1.at<float>(i,j);		g = g1.at<float>(i,j);		b = b1.at<float>(i,j);
			if( (r >= g) && (g >= b) )
				img.at<float>(i,j) = 60*(g - b)/((r - b) + eps);
			
			if ((g > r) && (r >= b))
				img.at<float>(i,j) = 60*(2 - (r - b)/((g - b) + eps));
			
			if ((g >= b) && (b > r))
				img.at<float>(i,j) = 60*(2 + (b - r)/((g - r) + eps));
			
			if( (b > g) && (g > r))
				img.at<float>(i,j) = 60*(4 - (g - r)/((b - r) + eps));
			
			if( (b > r) && (r >= g))
				img.at<float>(i,j) = 60*(4 + (r - g)/((b - g) + eps));
			
			if ((r >= b) && (b > g))
				img.at<float>(i,j) = 60*(6 - (b - g)/((r - g) + eps));
			
        
			if(img.at<float>(i,j) > max)
				max = (img.at<float>(i,j));


			if ( ((img.at<float>(i,j) > 270) && (img.at<float>(i,j) <= 360)))
				img.at<float>(i,j) = 0;
			
			
			

			img.at<float>(i,j) = img.at<float>(i,j)/360;
			

			img.at<float>(i,j) = (0.75 - img.at<float>(i,j))/(0.75);
		}
	}
	img.convertTo(*hue, CV_8UC1, 255.0);
	
		

}