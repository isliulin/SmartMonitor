#include "stdafx.h"

#include "VisionLib.h"
#include "ColorDetection.h"
#include "SmokeDetection.h"

vector<Rect> DetectSmoke(Mat image)
{
	//------------------------方法1一----------------------------------------------
	//return FindFeatures(DetectColorWithHSV(image, CigaretteColor()), 1, 1.0, 20);

	//------------------------方法2：根据轮廓和颜色-----------------------------

	RNG rng(12345);
	Mat imagecopy=image.clone();
	vector<Rect>smoke_rect;

	Rect optionalface_rect =Rect(0,0,image.size().width,image.size().height); 

	Mat hsv;
	vector<Mat> channel;  
	Mat h;
	Mat s;
	Mat v;
	cvtColor(imagecopy, hsv, CV_BGR2HSV);  
	split(hsv, channel);  
	h=channel[0];
	s=channel[1];
	v=channel[2];

	Mat white =(h<180)&(s<50)&(v>200);
    #ifdef SHOW 	
	imshow("whiteimg",white);
    #endif	
	medianBlur(white,white,3);
	bwareaopen(white,5);
	//除去太长或者太短的轮廓  
	bwerasebigarea(white,200);
	
    #ifdef SHOW 	
	imshow("pwhiteimg",white);
    #endif	
	Mat result=white.clone();

		
    #ifdef SHOW
	imshow("face",image);
    #endif	
	//
	

	int smokecount=0;
	Rect roi=optionalface_rect;
	int roiarea=optionalface_rect.width*optionalface_rect.height;


	double minarea=roiarea*0.001;
	double maxarea=roiarea*0.01;
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(2, 2));
	Mat whiteroi=white(roi); 
	//dilate(whiteroi, whiteroi, element); ////imshow("dilate", white);
	int low_height_thresh=whiteroi.rows/4;
	int high_height_thresh=whiteroi.rows/2;
	////imshow("lvbo",white);
	vector<vector<Point>> contours;	

	findContours(whiteroi, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);


	/// Approximate contours to polygons + get bounding rects and circles
	vector<vector<Point> > contours_poly( contours.size() );
	//vector<Rect> boundRect( contours.size() );//检测到的所有的轮廓
	vector<Rect> boundRect;//检测到的所有的轮廓
	for( int k = 0; k < contours.size(); k++ )
	{
		approxPolyDP( Mat(contours[k]), contours_poly[k], 3, true );
		boundRect.push_back(boundingRect( Mat(contours_poly[k]) )) ;
				}

		for( int k = 0; k < boundRect.size(); k++ )
		{
			if ((boundRect[k].y>low_height_thresh)&((boundRect[k].y+boundRect[k].height)>high_height_thresh))
			{
				double whitearea = contourArea(contours[k]);
				if (whitearea<maxarea)
				{
						smoke_rect.push_back(boundRect[k]);
						smoke_rect[smokecount].x+=roi.x-5;
						smoke_rect[smokecount].y+=roi.y-5;
						smoke_rect[smokecount].width=20;
						smoke_rect[smokecount].height=20;
						smokecount++;	
				}
			}

		}

	
		if (smoke_rect.size())
		{
			for( int i = 0; i< smoke_rect.size(); i++ )
			{
				Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
				rectangle( imagecopy, smoke_rect[i].tl(),  smoke_rect[i].br(), color, 2, 8, 0 );//其中boundRectneed[i]包含了一个轮廓的矩形框

			}
		}
	#ifdef SHOW 	
	imshow( "res", imagecopy );
    #endif		
	return smoke_rect;



}