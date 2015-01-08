#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include<iostream>
#include<string>
#include<sstream>
#include<math.h>

using namespace std;
using namespace cv; 

MatND calculate_Histogram(Mat *enhanced_image);

float entropy(float *p, int i, int j)
{
	float entropy = 0.0;
	for (int x=i; x<=j; x++)
	{
		if(p[x]!=0)
			entropy += p[x]*log(p[x]);
		else 
			continue;
	}
	return (-entropy);
}




void fuzzy_enhancement(Mat *enhanced_image, Mat image)
{
	MatND count = calculate_Histogram(enhanced_image);
	int sum_count = image.rows * image.cols;

	
	float p[256];							//Defining p[i] - Normalized Hisogram;
	for(int i=0; i<256; i++)
	{
			p[i] = count.at<float>(i) / sum_count;
	}
	// PEAK FIND
	int i_1, i_3, peak_val;
	for(i_1=1; i_1<255; i_1++)
	{
		if((count.at<float>(i_1 - 1) < count.at<float>(i_1)) && (count.at<float>(i_1) > count.at<float>(i_1+1)))
		{
			peak_val = count.at<float>(i_1);
			break;
		}
	}
	for (int i=i_1; i<255; i++)
	{
		if(peak_val==count.at<float>(i))
			i_1 = i;
	}
	
	float x = i_1*(1.0/255.0);

	for(i_3=254; i_3>=0; i_3--)
	{
		if(count.at<float>(i_3-1) < count.at<float>(i_3) && count.at<float>(i_3) > count.at<float>(i_3+1))
			break;
	}

	float z = i_3*(1.0/255.0);

	//ENTROPY FIND
	float ent[255], max = 0;
	int i_2;
	ent[0] = ent[254] =0;
	for(int i=1; i<255; i++)
	{
		ent[i] = entropy(p, 0, i) + entropy(p, i, 255); 
		if(ent[i] > max)
		{
			max = ent[i];
			i_2 = i+1;
		}
	}

	float y = i_2*(1.0/255.0);
	
	Mat s((*enhanced_image).size(), CV_32FC1);

	for(int j = 0; j< image.rows; j++)
	{
		for(int k = 0; k < image.cols; k++)
		{
			if(image.at<float>(j,k) <= x)
				s.at<float>(j,k) = 0;

			if ((x < image.at<float>(j,k)) && (image.at<float>(j,k) <= y))
				 s.at<float>(j,k) = (pow(image.at<float>(j,k)-x,2)) / ((y-x)*(z-x));

			if(image.at<float>(j,k) > z)
				s.at<float>(j,k) = 1;
		}
	}

	float thresh = 0.5;
	float t;
	
	for(int iter = 0; iter<2; iter++)
	{

		for(int i = 0; i<s.rows; i++)
		{
			for(int j = 0; j<s.cols; j++)
			{
				t = s.at<float>(i,j);
				if(t <= thresh)
					s.at<float>(i,j) = 2*t*t;
				else
					s.at<float>(i,j) = 1 - 2*(1-t)*(1-t);
			}
		}
	}

	*enhanced_image = s.clone();




}
