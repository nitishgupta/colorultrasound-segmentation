#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include<iostream>
#include<string>
#include<sstream>

using namespace std;
using namespace cv; 

struct boox_cordinates 
{
   int x,y,width,height;
   int done; 
   IplImage* image;
};

void print_coordinates(boox_cordinates *c)
{
	cout<<"\n"<<c->x<<"\t"<<c->y<<"\n";
	cout<<c->x + c->width << "\t"<<c->y<<"\n";
	cout<<c->x<<"\t"<<c->y + c->height<<"\n";
	cout<<c->x + c->width<<"\t"<<c->y + c->height<<"\n";
}

void booxmy_mouse_calback( int event, int x, int y, int flags, void* cordi );

CvRect boox;
bool drawing_boox = false;


void draw_boox( IplImage* img, CvRect rect ){
	cvRectangle( img, cvPoint(boox.x, boox.y), cvPoint(boox.x+boox.width,boox.y+boox.height),
				cvScalar(0xff,0x00,0x00) );
}

// Implement mouse callback
void booxmy_mouse_calback( int event, int x, int y, int flags, void* coordi )
{
	boox_cordinates* cordi = (boox_cordinates*) coordi;
	IplImage* image = (*cordi).image;

	switch( event ){
		case CV_EVENT_MOUSEMOVE: 
			if( drawing_boox ){
				boox.width = x-boox.x;
				boox.height = y-boox.y;
			}
			break;

		case CV_EVENT_LBUTTONDOWN:
			if(cordi->done!=-1)
			{
				drawing_boox = true;
				cordi->done = 1;
				boox = cvRect( x, y, 0, 0 );
				break;
			}
			else
			{	cordi->done=-1;
				break;
}

		case CV_EVENT_LBUTTONUP:
			if(cordi->done!=-1)
			{

				drawing_boox = false;
				if( boox.width < 0 ){
					boox.x += boox.width;
					boox.width *= -1;
				}
				if( boox.height < 0 ){
					boox.y += boox.height;
					boox.height *= -1;
				}
				draw_boox( image, boox );

				(*cordi).x = boox.x;
				(*cordi).y = boox.y;
				(*cordi).width = boox.width;
				(*cordi).height = boox.height;
				(*cordi).done = -1;
				break;
			}

			else
			{	cordi->done=-1;
				break;
			}
	}
}

void roi_bmode(Mat test_image, Mat* roi_image, int *x, int *y, int *width, int *height)
{
	const char* name = "boox Example";
	boox = cvRect(-1,-1,0,0);

	//string path;
	
	//cout<<"\nEnter Image Path to be selected";
	//getline(cin, path);
	
	

	//Mat img = imread(path);


	IplImage copy = test_image;
	
	IplImage* image = &copy;
	IplImage* temp = cvCloneImage(image);
	
	cvNamedWindow( name );
	
	boox_cordinates* coordi = new boox_cordinates;
	coordi->done = 0;
	coordi->image = image;

	// Set up the callback
	cvSetMouseCallback( name, booxmy_mouse_calback, (void*)((boox_cordinates*) coordi));

	// Main loop
	while( 1 ){
		cvCopyImage( image, temp );
		if( coordi->done!=-1 ) 
			draw_boox( temp, boox );
		cvShowImage( name, temp );

		if(cvWaitKey(5) != -1) 
			break;
	}

	//cvDestroyWindow( name );



	*roi_image = test_image(Rect(coordi->x, coordi->y, coordi->width, coordi->height));
	*x = coordi->x;
	*y = coordi->y;
	*width = coordi->width;
	*height = coordi->height;

}
