#include "stdafx.h"
#include<\opencv\build\include\opencv2\imgproc\imgproc.hpp>
#include <cv.h>
#include <highgui.h>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<conio.h>

using namespace std;
using namespace cv;

void roi_select(Mat test_image, Mat test_image2, string path, Mat* roi_image, Mat* roi_image2, int *x, int *y, int *width, int *height);
void contrast_enhancement(Mat* enhanced_image, float ratio);
void negative_image(Mat test_image, Mat* negative_image);
MatND calculate_Histogram(Mat *enhanced_image);
void rgbtohsv(Mat image, Mat *hue);
Mat bmode_only(Mat clr_image, int *x, int *y, int *widht, int *height, Mat bmode_roi, vector<Point> *contour_bmode, Mat *bmodecontour_roi);
void fuzzy_enhancement(Mat *enhanced_image, Mat normal_image);
vector<Point> level_set_based(Mat src, Mat *dst, int x, int y, int mode);
Mat us_bmode(Mat clr_image, int *x, int *y, int *width, int *height, Mat bmode_roi, Mat *elasto_roi, vector<Point> *contour_bmode, vector<Point> *contour_bmode_elasto, Mat *bmodecontour_roi); 
Mat elasto_seg(Mat elasto_roi, Mat clr_image, int x, int y, vector<Point> *contour_elasto);
void area_overlap(Mat clr_image, vector<Point> bmode_contour, vector<Point> elasto_contour, int x, int y, int width, int height, Mat *overlap, Mat *overlap_roi, int *overlap_area);
float features(Mat clr_image, Mat bmode_roi, Mat elasto_roi, Mat overlap_roi, Mat bmodecontour_roi, int overlap_area, float *feature);

int main()
{
	string filename;
	float feature[8];
	float score;
	
	string path = "D:/testdata/colortestdata/";
	load_data:
	cout<<"\nEnter the name of the image to be segmented to be segmented:   ";
		
	getline(cin, filename);

	/*************************************** CHECK FOR EXISTENCE OF IMAGE*******************/
	/// Load the source image
	again: 
	char ch;
	Mat clr_image = imread( path + filename + ".jpg", 1 );
    if( !clr_image.data )                              // Check for invalid input
			{
					cout <<  "Could not open or find the image" << std::endl ;
					goto load_data;
			}
    imshow("Original Image", clr_image);
	waitKey();
	destroyWindow("Original Image");
	Mat contoured;
	int x=0,y=0, width=0,height=0;
	Mat bmode_roi, elasto_roi, bmodecontour_roi;
	vector<Point> bmode_contour;
	vector<Point> contour_bmode_elasto; ////////// CARRY ON FROM HERE////////////////
	
    system("cls");
	cout<<"1. Only B-Mode Image\n2. Both US B-Mode and Color Elastography Image.\nEnter your choice : ";
	cin>>ch;
	cin.ignore();	
    cout<<"\n";

	if(ch == '2')
	{
		contoured =  us_bmode(clr_image, &x, &y, &width, &height, bmode_roi, &elasto_roi, &bmode_contour, &contour_bmode_elasto, &bmodecontour_roi);
	    Mat bothbmode = contoured.clone();
        
		// draw the polygon 
		vector<Point> elasto_contour;
		contoured =  elasto_seg(elasto_roi, clr_image, x, y, &elasto_contour);
	
		//Find Overlap Area
		int overlap_area =0;    
		Mat overlap_roi = cv::Mat::zeros(height, width, CV_8UC1);
		Mat overlap = cv::Mat::zeros(clr_image.rows, clr_image.cols, CV_8UC3);
	
		area_overlap(clr_image, bmode_contour,elasto_contour,x,y,width,height, &overlap, &overlap_roi, &overlap_area);

		imshow("Elastography Contoured", contoured);
        imshow("Bmode Contour overlap on Elasto Side", bothbmode);
		imshow("Overlap Area", overlap + clr_image);
		waitKey();
		//destroyAllWindows();
	
		system("cls");
		cout<<"\nDo you want to segment again : \nYES: y\nNO: n\nEnter your choice : ";
		cin>>ch;
        system("cls");

		if(ch=='y')
        {
            destroyAllWindows();
            goto again;
        }

		float score = features(clr_image,bmode_roi,elasto_roi, overlap_roi, bmodecontour_roi, overlap_area, feature);
		std::ostringstream ss;
		ss << score; 
		std::string s(ss.str());

	

		//Write file to Disk
		//imwrite(path + "seg" + filename[no].replace(filename[no].end()-4, filename[no].end(), "_segmented.jpg"), contoured);
		//imwrite(path + "seg" + filename[no].replace(filename[no].end()-4, filename[no].end(), "_") + s + ".jpg", overlap+contoured);

		cout<<"\n"<<"\n";
		//cout<<"Average Hardness of B-Mode Contour overlapped on Color Elastography= "<<feature[0]/0.75;
        //cout<<"\nAverage Hardness of the Overlap Area : "<<feature[2]/0.75;
        cout<<"\n\nEstimated Score = "<<score;
		cout<<"\n\n\nPress key to exit : ";
        getch();
	}

	else

	{   contoured = bmode_only(clr_image, &x, &y, &width, &height, bmode_roi, &bmode_contour, &bmodecontour_roi);
	    //	imwrite(path + "annotated/" + filename.replace(filename.end()-4, filename.end(), "mask.jpg"), bmodecontour_roi);	
		//imwrite(path + "annotated/" + filename.replace(filename.end()-4, filename.end(), "_segmented.jpg"), contoured);
		//cout<<path + "annotated/" + filename.replace(filename.end()-4, filename.end(), "mask.jpg");
        imshow("BMODE CONTOUR", contoured);
		waitKey();
    }
	return 0;
}


