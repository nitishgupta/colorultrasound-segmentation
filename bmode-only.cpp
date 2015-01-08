#include "stdafx.h"
#include<\opencv\build\include\opencv2\imgproc\imgproc.hpp>
#include <cv.h>
#include <highgui.h>
#include<iostream>
#include<string>
#include<sstream>

using namespace std;
using namespace cv;

void roi_bmode(Mat test_image, Mat* roi_image, int *x, int *y, int *width, int *height);
void contrast_enhancement(Mat* enhanced_image, float ratio);
void negative_image(Mat test_image, Mat* negative_image);
MatND calculate_Histogram(Mat *enhanced_image);
void fuzzy_enhancement(Mat *enhanced_image, Mat normal_image);
vector<Point> level_set_based(Mat src, Mat *dst, int x, int y, int mode, double thresh);

Mat bmode_only(Mat clr_image, int *x, int *y, int *width, int *height, Mat bmode_roi, vector<Point> *contour_bmode, Mat *bmodecontour_roi) 
{	
	
	// Convert Image to GrayScale
	Mat gray_image;
	cvtColor(clr_image, gray_image, CV_BGR2GRAY);   

	//Mat bmode = gray_image.colRange(0 , (clr_image.cols)/2);
	//Mat elasto = clr_image.colRange((clr_image.cols)/2, clr_image.cols);

	
	// Selection for B-Mode or Elasto Image
	roi_bmode(gray_image, &bmode_roi, x, y, width, height);
	

	// Median Filter
	Mat filtered_image;
	int window_size = 7;
	medianBlur( bmode_roi, filtered_image, window_size );
	
	//Histogram Equilization
	equalizeHist( filtered_image, filtered_image );
		
	//Contrast Enhancement
	Mat neg_image;
	negative_image(filtered_image, &neg_image);						//Negative Image
	

	Mat normal_image(neg_image.size(), CV_32FC1);
	neg_image.convertTo(normal_image, CV_32FC1, 1.0/255.0);         //Negative Normalized
		
	contrast_enhancement(&normal_image, 4.0);							//Contrast Enhancement
	

	Mat enhanced_image(neg_image.size(), CV_8UC1);
	normal_image.convertTo(enhanced_image, CV_8UC1, 255, 0);		//Back to negative 0..255
	negative_image(enhanced_image, &enhanced_image);			
	
	enhanced_image.convertTo(normal_image, CV_32FC1, 1.0/255.0);		//normal_image = contrast enhanced normalized // enhanced_image = contrast enhanced 0..255
	

	//Fuzzy Enhancement
	
	fuzzy_enhancement(&enhanced_image, normal_image);
	for(int i=0; i<enhanced_image.rows; i++)
	{
		for(int j=0; j<enhanced_image.cols; j++)
		{
			if(enhanced_image.at<float>(i,j) < 0)
				enhanced_image.at<float>(i,j) = 1;
		}
	}

	Mat src = enhanced_image;
	
	// Contour Level Based
	
	*contour_bmode = level_set_based(src, &clr_image, *x, *y, 0, 0.37);
	

	const Point *pts = (const  Point*) Mat(*contour_bmode).data;
	int npts = Mat(*contour_bmode).rows;
	
	// draw the polygon 
	Mat clr_contoured = clr_image.clone();
	
	polylines(clr_contoured, &pts,&npts, 1,
	    		true, 			// draw closed contour (i.e. joint end to start) 
	            Scalar(0,255,255),// colour RGB ordering (here = green) 
	    		1, 		        // line thickness
			    CV_AA, 0);
	

	/////// Drawing Bmode Contour on the elasto side /////////
		
	*bmodecontour_roi = cv::Mat::zeros(clr_image.rows, clr_image.cols, CV_8UC1);
	Point pt;
	for(int i=0; i<clr_image.rows; i++)
	{
		for(int j=0; j<clr_image.cols;j++)
		{
			pt.x = *x + j; pt.y = *y + i;
			if(pointPolygonTest(*contour_bmode, pt, false) >= 0)
				(*bmodecontour_roi).at<uchar>(i + *y, j + *x) = uchar(255);
		}
	}
	

	
	return clr_contoured;

}

 