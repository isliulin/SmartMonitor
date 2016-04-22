//********************************************************************
//*  HelmetsDetection.h
//*  Copyright (c) 2014 CSIS Research Center. All rights reserved.
//* 
//*  version: 1.0
//*  author:  Zhu DaYong 
//*  Created: 2014.03.17
//*  mailto:  zdytaurus@gmail.com
//*********************************************************************/

#include "stdafx.h"
#include "VisionLib.h"

void DetectConstructionSite(vector<Mat> orgMat,vector<Rect> &noHelmetPedestrianRect,vector<Rect> &smokePedestrianRect,bool &ClassDetectionResult,vector<Rect> &noBeltPedestrianRect);
Rect GetHeadRect(Rect PedestrianRect);
vector<Rect> GetFaceRect(Mat src,Rect HogHeadRect);
vector<Rect> GetHelmetRect(Mat src,Rect HogHeadRect);
vector<Rect> GetSmokeRect(Mat faceImg,Rect FaceRect);
Rect GetSmokeResult(Mat &src,Rect PedestrianRect,vector<Rect> helmetRectFirst);
