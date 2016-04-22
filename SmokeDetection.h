//********************************************************************
//*  SmokeDetection.h
//*  Copyright (c) 2014 CSIS Research Center. All rights reserved.
//* 
//*  version: 1.0
//*  author:  Zhu DaYong 
//*  Created: 2014.03.22
//*  mailto:  zdytaurus@gmail.com
//*********************************************************************/
#ifndef __SMOKEDETECTION_H__
#define __SMOKEDETECTION_H__

#include "stdafx.h"
#include "VisionLib.h"

vector<Rect> DetectSmoke(Mat frame);

#endif // __SMOKEDETECTION_H__