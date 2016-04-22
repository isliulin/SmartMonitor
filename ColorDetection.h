//********************************************************************
//*  ColorDetection.h
//*  Copyright (c) 2014 CSIS Research Center. All rights reserved.
//* 
//*  version: 1.0
//*  author:  Zhu DaYong 
//*  Created: 2014.03.19
//*  mailto:  zdytaurus@gmail.com
//*********************************************************************/
#ifndef __COLORDETECTION_H__
#define __COLORDETECTION_H__

#include "stdafx.h"

struct ColorRange
{  	
	vector<Range> h;  
	vector<Range> s;  
	vector<Range> v;
};  

vector<ColorRange> SkinColor();
vector<ColorRange> HelmetColor();
vector<ColorRange> CigaretteColor();

Mat DetectColorWithHSV(Mat view, vector<ColorRange> colorRanges);

#endif // __COLORDETECTION_H__