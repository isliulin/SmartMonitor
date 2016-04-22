//********************************************************************
//*  PedestrianDetection.h
//*  Copyright (c) 2014 CSIS Research Center. All rights reserved.
//* 
//*  version: 1.0
//*  author:  Zhu DaYong 
//*  Created: 2014.03.17
//*  mailto:  zdytaurus@gmail.com
//*********************************************************************/
#ifndef __PEDESTRIANDETECTION_H__
#define __PEDESTRIANDETECTION_H__

#include "stdafx.h"
#include "VisionLib.h"

vector<Rect> DetectPedestrian(Mat view);

#endif // __PEDESTRIANDETECTION_H__