#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include<iostream>
#include<string>
#include<sstream>
#include<math.h>

using namespace std;
using namespace cv; 


void contrast_enhancement(Mat* enhanced_image, float ratio)
{
		
	const int cols = (*enhanced_image).cols;
	int xc, yc;
	const int rows = (*enhanced_image).rows;
	float val = 0.00;
	float exponent = 0.00;
	float sigmax = float(cols/ratio);
	float sigmay = float(rows/ratio);

	float max=0.0;
	if(rows % 2 == 0)
		xc = rows /2;
	else 
		xc = (rows+1)/2;

	if(cols%2 == 0)
		yc = cols/2;
	else
		yc = (cols+1)/2;


	Mat kernelX = getGaussianKernel(rows, sigmay);
	Mat kernelY = getGaussianKernel(cols, sigmax);
	Mat kernelXY = kernelX * kernelY.t();
	
	for(int i=0; i<rows;i++)
	{
		for(int j=0; j<cols; j++)
		{
			
			(*enhanced_image).at<float>(i,j) = (*enhanced_image).at<float>(i,j) * float(kernelXY.at<double>(i,j));
			if((*enhanced_image).at<float>(i,j) > max)
			{max = (*enhanced_image).at<float>(i,j);}
			/*
			exponent = float(pow(i - xc,2.0)/2.0*sigmax*sigmax + pow(j - yc,2.0)/ 2.0*sigmay*sigmay);
			val = exp( -exponent);
			(*enhanced_image).at<float>(i,j) = val * (*enhanced_image).at<float>(i,j);
			if((*enhanced_image).at<float>(i,j) > max)
			{max = (*enhanced_image).at<float>(i,j);}*/
		}
	}
	for(int i=0; i<rows;i++)
	{
		for(int j=0; j<cols; j++)
		{
			(*enhanced_image).at<float>(i,j) = (*enhanced_image).at<float>(i,j)/max;
		}
	}

}




	