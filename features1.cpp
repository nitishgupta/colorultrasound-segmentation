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

void rgbtohsv(Mat image, Mat *hue);


float features(Mat clr_image, Mat bmode_roi, Mat elasto_roi, Mat overlap_roi, Mat bmodecontour_roi, int overlap_area, float *feature)
{

    int fno = 0;
	Mat bmode = clr_image.colRange(0 , (clr_image.cols)/2);
	Mat elasto = clr_image.colRange((clr_image.cols)/2, clr_image.cols);
	cvtColor(bmode, bmode, CV_BGR2GRAY);   

	Mat hue_elasto, hue_roi;
	rgbtohsv(elasto, &hue_elasto);
	rgbtohsv(elasto_roi, &hue_roi);
	Mat hue_normal;

	hue_roi.convertTo(hue_normal, CV_32FC1, 1.0/255.0);
	
	
	Mat imgbo = bmodecontour_roi < 1;
    Mat imgi[5];
    Mat innband[5];
	
	
	
	distanceTransform(imgbo, imgbo,  CV_DIST_L2, 5);
    
    distanceTransform(bmodecontour_roi, imgi[0],  CV_DIST_L2, 5);
    imgi[0] = imgi[0] >5;
    innband[0] = bmodecontour_roi - imgi[0];
    
    for(int i=1; i<5; i++)
    {
        distanceTransform(imgi[i-1], imgi[i],  CV_DIST_L2, 5);
        imgi[i] = imgi[i] >5;
        innband[i] = imgi[i-1] - imgi[i];
    }
	
    float h, m, l;
    h = m = l =0.0;
    float t =0.0;
	imgbo = imgbo>5;
	imgbo = imgbo < 1;

	
    Mat outerband = imgbo - bmodecontour_roi; 

    float hardness[8] = {0,0,0,0,0,0,0,0};
    int k[8] = {0,0,0,0,0,0,0,0}; 

    float sum =0, max_hard=0;
	for(int i = 0; i<overlap_roi.rows; i++)
	{
		for(int j=0; j<overlap_roi.cols; j++)
		{
			
			if (bmodecontour_roi.at<uchar>(i, j) != 0 )
			{
				hardness[0] += (0.75 - (hue_normal.at<float>(i,j))*0.75);
				k[0] ++;
                if((0.75 - (hue_normal.at<float>(i,j))*0.75) >= 0.5) 
                    h++;
                else if ((0.75 - (hue_normal.at<float>(i,j))*0.75) < 0.5 && (0.75 - (hue_normal.at<float>(i,j))*0.75) >= 0.35)
                    m++;
                else 
                    l++;

                t++;
			}
            
            if (outerband.at<uchar>(i, j) != 0 )
			{
				hardness[1] += (0.75 - (hue_normal.at<float>(i,j))*0.75);
				k[1]++;
			}
			
            if (overlap_roi.at<uchar>(i, j) != 0 )
			{
				hardness[2] += 0.75 - (hue_normal.at<float>(i,j))*0.75;
				k[2]++;
			}

            if (innband[1].at<uchar>(i, j) != 0 )
			{
				hardness[3] += 0.75 - (hue_normal.at<float>(i,j))*0.75;
				k[3]++;
			}

			if (innband[2].at<uchar>(i, j) != 0 )
			{
				hardness[4] += (0.75 - (hue_normal.at<float>(i,j))*0.75);
				k[4]++;
			}
			if (innband[3].at<uchar>(i, j) != 0 )
			{
				hardness[5] += (0.75 - (hue_normal.at<float>(i,j))*0.75);
				k[5]++;
			}
			if (innband[4].at<uchar>(i, j) != 0 )
			{
				hardness[6] += (0.75 - (hue_normal.at<float>(i,j))*0.75);
				k[6]++;
			}

            if (imgi[4].at<uchar>(i, j) != 0 )
			{
				hardness[7] += (0.75 - (hue_normal.at<float>(i,j))*0.75);
				k[7]++;
			}




		}
	}
	float score;
	/*
	cout<<"\n\n"<<"Average Hardness = "<<(hardness/k);
	cout<<"\n\n"<<"Average Hardness  Inside= "<<(hardnessi/ki);
	cout<<"\n\n"<<"Average Hardness  Outside= "<<(hardnesso/ko);
	cout<<"\n\n"<<"Average Hardness   Much Outside= "<<(hardnessoo/koo)<<"\n";
	cout<<"\n\n"<<"Average Hardness  Bmode Overlap= "<<(hardnessb/kb);
	cout<<"\n\n"<<"Average Hardness  Bmode Outside= "<<(hardnessbo/kbo);
	cout<<"\n\n"<<"Overlap Hadness  - Bmode Hardness = "<<(hardness/k) - (hardnessb/kb);
	*/


   for(int i=0; i<8;i++)
   {
    feature[i] = (hardness[i]/k[i]);
   }

   float high, med, low;
   high = (h/t) * 100;
   med = (m/t)*100;
   low = (l/t)*100;
     
   cout<<"\nPercentage of different hardness levels : ";
   cout<<"\n\nHigh : "<<(h/t) * 100<<"\nMed : "<<(m/t)*100 <<"\nLow : "<<(l/t)*100;
   
       
 
   if(high < 38)    
   {
       if(med < 30)
           score = 1;
   
       else 
            score = 2;
   }

   else if(high < 75)
        score =3;

   else if(high<90)
       score = 4;
   
   else if(feature[1] > 0.5)
        score = 5;
   else
       score = 5;

   return score;
}   