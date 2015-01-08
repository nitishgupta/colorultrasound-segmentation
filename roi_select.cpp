#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include<iostream>
#include<string>
#include<sstream>

using namespace std;
using namespace cv; 

struct box_coordinates 
{
   int x,y,width,height;
   int done; 
   IplImage* image;
};

void print_coordinates(box_coordinates *c)
{
	cout<<"\n"<<c->x<<"\t"<<c->y<<"\n";
	cout<<c->x + c->width << "\t"<<c->y<<"\n";
	cout<<c->x<<"\t"<<c->y + c->height<<"\n";
	cout<<c->x + c->width<<"\t"<<c->y + c->height<<"\n";
}

void my_mouse_callback( int event, int x, int y, int flags, void* cordi );

CvRect box;
bool drawing_box = false;


void draw_box( IplImage* img, CvRect rect ){
	cvRectangle( img, cvPoint(box.x, box.y), cvPoint(box.x+box.width,box.y+box.height),
				cvScalar(0xff,0x00,0x00) );
}

// Implement mouse callback
void my_mouse_callback( int event, int x, int y, int flags, void* coordi )
{
	box_coordinates* cordi = (box_coordinates*) coordi;
	IplImage* image = (*cordi).image;

	switch( event ){
		case CV_EVENT_MOUSEMOVE: 
			if( drawing_box ){
				box.width = x-box.x;
				box.height = y-box.y;
			}
			break;

		case CV_EVENT_LBUTTONDOWN:
			if(cordi->done!=-1)
			{
				drawing_box = true;
				cordi->done = 1;
				box = cvRect( x, y, 0, 0 );
				break;
			}
			else
			{	cordi->done=-1;
				break;
}

		case CV_EVENT_LBUTTONUP:
			if(cordi->done!=-1)
			{

				drawing_box = false;
				if( box.width < 0 ){
					box.x += box.width;
					box.width *= -1;
				}
				if( box.height < 0 ){
					box.y += box.height;
					box.height *= -1;
				}
				draw_box( image, box );

				(*cordi).x = box.x;
				(*cordi).y = box.y;
				(*cordi).width = box.width;
				(*cordi).height = box.height;
				(*cordi).done = -1;
				break;
			}

			else
			{	cordi->done=-1;
				break;
			}
	}
}

void roi_select(Mat test_image, Mat test_image2, string path, Mat* roi_image, Mat* roi_image2, int *x, int *y, int *width, int *height)
{
	const char* name = "Box Example";
	box = cvRect(-1,-1,0,0);

	//string path;
	
	//cout<<"\nEnter Image Path to be selected";
	//getline(cin, path);
	
	

	//Mat img = imread(path);


	IplImage copy = test_image;
	
	IplImage* image = &copy;
	IplImage* temp = cvCloneImage(image);
	
	cvNamedWindow( name );
	
	box_coordinates* coordi = new box_coordinates;
	coordi->done = 0;
	coordi->image = image;

	// Set up the callback
	cvSetMouseCallback( name, my_mouse_callback, (void*)((box_coordinates*) coordi));

	// Main loop
	while( 1 ){
		cvCopyImage( image, temp );
		if( coordi->done!=-1 ) 
			draw_box( temp, box );
		cvShowImage( name, temp );

		if(cvWaitKey(5) != -1) 
			break;
	}

	//cvDestroyWindow( name );



	*roi_image = test_image(Rect(coordi->x, coordi->y, coordi->width, coordi->height));
	*roi_image2= test_image2(Rect(coordi->x, coordi->y, coordi->width, coordi->height));
	*x = coordi->x;
	*y = coordi->y;
	*width = coordi->width;
	*height = coordi->height;

}
