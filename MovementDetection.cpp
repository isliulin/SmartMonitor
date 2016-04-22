#include "stdafx.h"
#include "VisionLib.h"
#include "MovementDetection.h"

Mat GetFrameDiff(Mat frontFrame, Mat frame)
{
	cvtColor(frontFrame, frontFrame, COLOR_BGR2GRAY); 
	cvtColor(frame, frame, COLOR_BGR2GRAY); 

	Mat diff;
	absdiff(frame, frontFrame, diff);

#ifdef SHOW
	ShowImage("diff View", diff);
#endif	
	return diff;
}

vector<Rect> GetDynamicRegion(Mat frontFrame, Mat frame)
{
	Mat diff = GetFrameDiff(frontFrame, frame);
	int threshold  = 50;
	diff = diff > threshold;
	return FindFeatures(diff);
}