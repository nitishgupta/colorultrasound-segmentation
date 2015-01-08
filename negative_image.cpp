#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include<iostream>
#include<string>
#include<sstream>
#include<math.h>
//#include<cmath>

#define _USE_MATH_DEFINES


using namespace std;
using namespace cv; 


void negative_image(Mat test_image, Mat* negative_image)
{
	IplImage copy = test_image;
	IplImage* fil_image = &copy;
	cvNot(fil_image, fil_image);
	Mat neg_image(fil_image);
	*negative_image = neg_image.clone();
}




	