//********************************************************************
//*  WorkAtHeight.h
//*  Copyright (c) 2014 CSIS Research Center. All rights reserved.
//* 
//*  version: 1.0
//*  author:  Zhu DaYong 
//*  Created: 2014.03.17
//*  mailto:  zdytaurus@gmail.com
//*********************************************************************/
#ifndef __WORKATHEIGHT_H__
#define __WORKATHEIGHT_H__

#include "stdafx.h"

vector<Rect> DetectWorkAtHeight(Mat frontFrame, Mat nextFrame, vector<Point> unsafeArea);
bool DetectSuspender(Mat frontView, Mat nextView);
Mat GetSuspenderRegion(Mat Frame, vector<Point> roi);

vector<Rect> IntoUnsafeArea(vector<Point> unsafeArea, vector<Rect> workers, Mat frame);

#endif // __WORKATHEIGHT_H__