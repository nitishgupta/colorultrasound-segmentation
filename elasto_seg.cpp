#include "stdafx.h"
#include<\opencv\build\include\opencv2\imgproc\imgproc.hpp>
#include <cv.h>
#include <highgui.h>
#include<iostream>
#include<string>
#include<sstream>

using namespace std;
using namespace cv;

void roi_select(Mat test_image, Mat test_image2, string path, Mat* roi_image, Mat* roi_image2, int *x, int *y, int *width, int *height);
void contrast_enhancement(Mat* enhanced_image, float ratio);
void negative_image(Mat test_image, Mat* negative_image);
MatND calculate_Histogram(Mat *enhanced_image);
void fuzzy_enhancement(Mat *enhanced_image, Mat normal_image);
vector<Point> level_set_based(Mat src, Mat *dst, int x, int y, int mode, double thresh);
Mat normalization(Mat input, float olow, float ohigh);
void rgbtohsv(Mat image, Mat *hue);

Mat elasto_seg(Mat elasto_roi, Mat clr_image, int x, int y, vector<Point> *contour_elasto) 
{
	
	Mat hue;
	rgbtohsv(elasto_roi, &hue);
    //imshow("elsf", elasto_roi) ;\
    //imshow("Hue", hue);
   // waitKey();
	
	// Median Filter
	Mat filtered_image;
	int window_size = 7;
	medianBlur(hue, filtered_image, window_size );
	
	
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
			if(enhanced_image.at<float>(i,j) < 0 )
				enhanced_image.at<float>(i,j) = 1;
		}
	}
	
	Mat src = enhanced_image; //////////////// CHANGE//////////////

	// Contour Level Based
	*contour_elasto = level_set_based(src, &clr_image, x, y, 1, 0.15);
	
	const Point *pts = (const  Point*) Mat(*contour_elasto).data;
	int npts = Mat(*contour_elasto).rows;
	
	// draw the polygon 
	Mat clr_contoured = clr_image.clone();
	polylines(clr_contoured, &pts,&npts, 1,
	    		true, 			// draw closed contour (i.e. joint end to start) 
	            Scalar(3,255,255),// colour RGB ordering (here = green) 
	    		1, 		        // line thickness
			    CV_AA, 0);

	//Mat intere = cv::Mat::zeros(src.rows, src.cols, CV_32F);
	Point pt;
	Mat intere = cv::Mat::zeros(clr_image.rows, clr_image.cols, CV_32F);
	for(int i=0; i<src.rows; i++)
	{
		for(int j=0; j<src.cols; j++)
		{ 
			pt.x = j + x + clr_image.cols/2; pt.y = i + y;
			if(pointPolygonTest(*contour_elasto, pt, false) < 0)
				(intere).at<float>(i + y,j + x + clr_image.cols/2) = 1;

		}
	}
	
	
	return clr_contoured;
 }

 