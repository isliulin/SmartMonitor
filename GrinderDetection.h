//********************************************************************
//*  GrinderDetection.h
//*  Copyright (c) 2014 CSIS Research Center. All rights reserved.
//* 
//*  version: 1.0
//*  author:  Zhu DaYong & Hou Xiaorong & ½ªÉºÉº 
//*  Created: 2014.03.19
//*  mailto:  zdytaurus@gmail.com
//*********************************************************************/
#ifndef __GRINDERDETECTION_H__
#define __GRINDERDETECTION_H__

#include "stdafx.h"
#include "VisionLib.h"

#define REGUALR_TURNS      0
#define NO_5TURNS          1
#define REGULAR_OPERATION  2
#define STAND_ON_COIL      3
#define PEOPLE_STAND_WRONG 4

Mat DetectCoil(Mat view);

vector<int> DetectGrinderOperation(vector<Mat> frames);

int DetectTurnsOfSteelRope(vector<Mat> frames, Rect rectGrinder);

Scalar FindPullingRope(vector<Mat> frames, Rect rectGrinder);

Rect LocalizeGrinder(vector<Mat> frames);

vector<Rect> GetCoilRegionPeople(Mat img);

#endif // __GRINDERDETECTION_H__