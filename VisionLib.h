//********************************************************************
//*  VisionLib.h
//*  Copyright (c) 2014 CSIS Research Center. All rights reserved.
//* 
//*  version: 1.0
//*  author:  Zhu DaYong 
//*  Created: 2014.03.17
//*  mailto:  zdytaurus@gmail.com
//*********************************************************************/
#ifndef __VISIONLIB_H__
#define __VISIONLIB_H__

#include "stdafx.h"

//#define SHOW

void Frame2Pic(VideoCapture vc, string path, 
	           Range rg = Range(), Rect rc = Rect());

void ShowImage(string name, Mat view);
void ExpandRect(Mat view, Rect& rc, int offset);

void bwerasebigarea(Mat& im, double size);
vector<vector<Point>> bwareaopen(Mat& im, double size);

void StrengthenFeatures(Mat src, Mat& dst, int blurSize = 15, 
	                    double removeSize = 200.0, int dilateSize = 10);

vector<Rect> FindConnectedDomain(Mat view);
vector<Rect> FindFeatures(Mat view, int blurSize = 15, double removeSize = 200.0, 
	                     int dilateSize = 10);

void bwerasebigarea(Mat& im, double size);

Mat ZoomImage (Mat img);

void ExpandRectDifSize(Mat view, Rect& rc, int offsetWidth,int offsetHeight);

#endif // __VISIONLIB_H__





