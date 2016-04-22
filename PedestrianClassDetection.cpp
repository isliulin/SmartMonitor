#include "stdafx.h"
#include "VisionLib.h"
#include "PedestrianClassDetection.h"

int DetectSupervisorClass(Rect upperbody_rect,Mat image)
{
	Rect roi=upperbody_rect;
	Mat body=image(roi);

	int matchClass =0;
	int nowClass=0;
	
	Mat hsv;
	vector<Mat> channel;  
	Mat h;
	Mat s;
	Mat v;

	cvtColor(body, hsv, CV_BGR2HSV);  
	split(hsv, channel);   
	h=channel[0];
	s=channel[1];
	v=channel[2];

	Mat red;
	Mat yellow;

	red=((h > 1) & (h < 6) | (h > 176)&(h < 180)) & (s > 108)&(s < 156)&(v > 230)&(s < 256);

	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(2, 2));
	dilate(red, red, element);
	medianBlur(red,red,3);
	bwareaopen(red,20);
    #ifdef SHOW 	
	 imshow("red",red);
    #endif
			

			// img.at<uchar>(x, y);
	yellow= (h > 20) & (h < 39) & 
			(s > 26)  &(h < 177)&
			(v > 180)&(v < 256) ;
	dilate(yellow, yellow,element);
	medianBlur(yellow,yellow,5);
	bwareaopen(yellow,20);
			
			
	//red row-col change
	int Rmax=0;
	for (int x=0;x<red.rows;x++)
	{
		int Rchange=0;
		for (int y=0;y<red.cols-1;y++)
		{
			 if(((red.at<uchar>(x, y)==0)&(red.at<uchar>(x, y+1)==255))|((red.at<uchar>(x, y)==255)&(red.at<uchar>(x, y+1)==0)))
			 {
                  Rchange++;
			 }
		}
		if (Rchange>Rmax)
		{
			Rmax=Rchange;
		}
	}
	int Cmax=0;
	for (int y=0;y<red.cols;y++)
	{
		int Cchange=0;
		for (int x=0;x<red.rows-1;x++)
		{
			if(((red.at<uchar>(x, y)==0)&(red.at<uchar>(x+1, y)==255))|((red.at<uchar>(x, y)==255)&(red.at<uchar>(x+1, y)==0)))
			{
			Cchange++;
			}

		}
		if (Cchange>Cmax)
		{
			Cmax=Cchange;
		}
	}
	int maxredchange=Rmax+Cmax;

	//yellow row-col change
	int YRmax=0;
	for (int x=0;x<yellow.rows;x++)
	{
		int Rchange=0;
		for (int y=0;y<yellow.cols-1;y++)
		{
			if(((yellow.at<uchar>(x, y)==0)&(yellow.at<uchar>(x, y+1)==255))|((yellow.at<uchar>(x, y)==255)&(yellow.at<uchar>(x, y+1)==0)))
			{
				Rchange++;
			}
		}
		if (Rchange>YRmax)
		{
			YRmax=Rchange;
		}
	}
	int YCmax=0;
	for (int y=0;y<yellow.cols;y++)
	{
			int Cchange=0;
		for (int x=0;x<yellow.rows-1;x++)
		{
			if(((yellow.at<uchar>(x,y)==0)&(yellow.at<uchar>(x+1, y)==255))|((yellow.at<uchar>(x, y)==255)&(yellow.at<uchar>(x+1, y)==0)))
			{
				Cchange++;
			}

		}
		if (Cchange>YCmax)
		{
			YCmax=Cchange;
		}
	}
	int maxyellowchange=YRmax+YCmax;

	if ((maxredchange>8)&(maxyellowchange>8))
	{
		nowClass=1;
		
	}

	return nowClass;
}
