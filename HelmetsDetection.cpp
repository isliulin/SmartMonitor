#include "stdafx.h"

#include "ColorDetection.h"
#include "HelmetsDetection.h"

vector<Rect> DetectHelmet(Mat view)
{	
	return FindFeatures(DetectColorWithHSV(view, HelmetColor()));
}