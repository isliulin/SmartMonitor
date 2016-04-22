//********************************************************************
//*  MovementDetection.h
//*  Copyright (c) 2014 CSIS Research Center. All rights reserved.
//* 
//*  version: 1.0
//*  author:  Zhu DaYong 
//*  Created: 2014.03.17
//*  mailto:  zdytaurus@gmail.com
//*********************************************************************/
#ifndef __MOVEMENTDETECTION_H__
#define __MOVEMENTDETECTION_H__

#include "stdafx.h"
#include "VisionLib.h"

Mat GetFrameDiff(Mat frontFrame, Mat frame);
vector<Rect> GetDynamicRegion(Mat frontFrame, Mat frame);

#endif // __MOVEMENTDETECTION_H__