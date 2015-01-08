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


float makebands(Mat clr_image, Mat bmode_roi, Mat elasto_roi, Mat overlap_roi, Mat bmodecontour_roi, int overlap_area, float *feature)
{
	int fno = 0;
	int k=0;
	
    Mat imgbo = bmodecontour_roi < 1;
    Mat imgbi, imgbii; 
	
	distanceTransform(imgbo, imgbo,  CV_DIST_L2, 5);
    distanceTransform(bmodecontour_roi, imgbi,  CV_DIST_L2, 5);

	
	imgbo = imgbo>5;
	imgbo = imgbo < 1;

	imgbi = imgbi>5;
	
    Mat outerband = imgbo - bmodecontour_roi; 
    
    Mat innerband = bmodecontour_roi - imgbi; /* CONTINUE FROM HERE......................................*/

    distanceTransform(imgbi, imgbii, CV_DIST_L2, 5);
    imgbii = imgbii > 5;
    Mat innband = imgbi - imgbii;
    /*
	float hardness = 0;
	float hardnessi = 0;
	int ki =0;
	float hardnesso = 0;
	int ko =0;
	
	float hardnessb = 0;
	int kb =0;

	float hardnessbo = 0;
	int kbo = 0;
	
	float hardnessoo = 0;
	int koo =0;
	
    float ihard=0;
    float ohard=0;
    float ni=0;
    float no=0;

    Mat check = cv::Mat::zeros(overlap_roi.rows, overlap_roi.cols, CV_8UC1);
    imshow("asfasf", check);
    waitKey();

	float sum =0, max_hard=0;
	for(int i = 0; i<overlap_roi.rows; i++)
	{
		for(int j=0; j<overlap_roi.cols; j++)
		{
			
            if ((overlap_roi).at<uchar>(i, j) != 0 )
			{
				hardness += 0.75 - (hue_normal.at<float>(i,j))*0.75;
				k++;
			}

            if (innerband.at<uchar>(i, j) != 0 && outerband.at<uchar>(i, j) != 0)
            {
                check.at<uchar>(i,j) = 255;//innerband.at<uchar>(i, j);
            }

            if (innerband.at<uchar>(i, j) != 0 )
			{
				ihard += 0.75 - (hue_normal.at<float>(i,j))*0.75;
				ni++;
			}

            if (outerband.at<uchar>(i, j) != 0 )
			{
				ohard += 0.75 - (hue_normal.at<float>(i,j))*0.75;
				no++;
			}

			if ((imgB).at<uchar>(i, j) != 0 )
			{
				hardnessi += (0.75 - (hue_normal.at<float>(i,j))*0.75);
				ki++;
			}
			if ((imgC).at<uchar>(i, j) != 0 )
			{
				hardnesso += (0.75 - (hue_normal.at<float>(i,j))*0.75);
				ko++;
			}
			if ((imgD).at<uchar>(i, j) != 0 )
			{
				hardnessoo += (0.75 - (hue_normal.at<float>(i,j))*0.75);
				koo++;
			}
			if ((bmodecontour_roi).at<uchar>(i, j) != 0 )
			{
				hardnessb += (0.75 - (hue_normal.at<float>(i,j))*0.75);
				kb++;
			}
			if ((imgbo).at<uchar>(i, j) != 0 )
			{
				hardnessbo += (0.75 - (hue_normal.at<float>(i,j))*0.75);
				kbo++;
			}


		}
	}
	
	
	cout<<"\n\n"<<"Average Hardness = "<<(hardness/k);
	cout<<"\n\n"<<"Average Hardness  Inside= "<<(hardnessi/ki);
	cout<<"\n\n"<<"Average Hardness  Outside= "<<(hardnesso/ko);
	cout<<"\n\n"<<"Average Hardness   Much Outside= "<<(hardnessoo/koo)<<"\n";
	cout<<"\n\n"<<"Average Hardness  Bmode Overlap= "<<(hardnessb/kb);
	cout<<"\n\n"<<"Average Hardness  Bmode Outside= "<<(hardnessbo/kbo);
	cout<<"\n\n"<<"Overlap Hadness  - Bmode Hardness = "<<(hardness/k) - (hardnessb/kb);
	

    imshow("Check", check);
    waitKey();
    feature[fno++] = (hardness/k);
	feature[fno++] = (hardnessi/ki);
	feature[fno++] = (hardnesso/ko);
	feature[fno++] = (hardnessoo/koo);
	feature[fno++] = (hardnessb/kb);
	feature[fno++] = (hardnessbo/kbo);
	feature[fno++] = (hardness/k) - (hardnessb/kb);

    cout<<"\nInnre hard :: "<<ihard/ni;
    cout<<"\n\nOuter hard :: "<<ohard/no;
    cout<<"\n\nDifference :: "<<ihard/ni - ohard/no;

    return (sum/float(overlap_area));*/
}