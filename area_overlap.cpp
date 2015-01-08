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

void area_overlap(Mat clr_image, vector<Point> bmode_contour, vector<Point> elasto_contour, int x, int y, int width, int height, Mat *overlap, Mat *overlap_roi, int *overlap_area)
{
	Point ptb, pte;
	int barea=0;
	int earea=0;
	
	for(int i=0; i<height; i++)
	{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
		for(int j=0; j<width; j++)
		{ 
			ptb.x = j + x;                    ptb.y = i + y;
			pte.x = j + x + clr_image.cols/2; pte.y = i + y;
			if(pointPolygonTest(elasto_contour, pte, false) > 0)
				earea++;
			
			if(pointPolygonTest(bmode_contour, ptb, false) >= 0)
				barea++;
			
			if((pointPolygonTest(elasto_contour, pte, false) > 0) && (pointPolygonTest(bmode_contour, ptb, false) > 0))
			{
				
				(*overlap_area)++; 
				(*overlap_roi).at<uchar>(i, j) = uchar(255);
				
				(*overlap).at<Vec3b>(i+y, j+x )[0] = 0;
				(*overlap).at<Vec3b>(i+y, j+x )[1] = 40;
				(*overlap).at<Vec3b>(i+y, j+x )[2] = 90;

				(*overlap).at<Vec3b>(i+y, j+x+clr_image.cols/2 )[0] = 0;
				(*overlap).at<Vec3b>(i+y, j+x+clr_image.cols/2 )[1] = 40;
				(*overlap).at<Vec3b>(i+y, j+x+clr_image.cols/2 )[2] = 90;
			}
		}
	}
}


