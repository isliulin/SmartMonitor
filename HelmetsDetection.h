//********************************************************************
//*  HelmetsDetection.h
//*  Copyright (c) 2014 CSIS Research Center. All rights reserved.
//* 
//*  version: 1.0
//*  author:  Zhu DaYong 
//*  Created: 2014.03.17
//*  mailto:  zdytaurus@gmail.com
//*********************************************************************/
#ifndef __HELMETSDETECTION_H__
#define __HELMETSDETECTION_H__

#include "stdafx.h"
#include "VisionLib.h"

vector<Rect> DetectHelmet(Mat frame);

#endif // __HELMETSDETECTION_H__