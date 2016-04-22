#include "stdafx.h"

#include "VisionLib.h"
#include "ColorDetection.h"

ColorRange Red()
{
	ColorRange rgs;

//	rgs.h.push_back(Range(1, 6));
//	rgs.h.push_back(Range(176, 180));
	rgs.s.push_back(Range(108, 156));	
//	rgs.v.push_back(Range(230, 256));

	rgs.h.push_back(Range(1, 15));
	rgs.h.push_back(Range(150, 180));
	rgs.v.push_back(Range(200, 256));

	return rgs;
}

ColorRange Yellow()
{
	ColorRange rgs;

	/*rgs.h.push_back(Range(28, 33));	
	rgs.s.push_back(Range(58, 116));	
	rgs.v.push_back(Range(230, 256));*/

	//Scalar(0, 109, 209), Scalar(179, 179, 255

	/* (h > 20) & (h < 39) & 
				(s > 26)  &(h < 177)&
				(v > 180)&(v < 256) ;*/


	rgs.h.push_back(Range(15, 39));	
	rgs.s.push_back(Range(26, 179));	  // 177
	rgs.v.push_back(Range(180, 256));

	return rgs;	
}

ColorRange White()
{
	ColorRange rgs;

	rgs.h.push_back(Range(0, 180));	
	rgs.s.push_back(Range(0, 6));	
	rgs.v.push_back(Range(230, 256));

	return rgs;
}

vector<ColorRange> HelmetColor()
{
	//inRange(hsvImg, Scalar(0, 109, 209), Scalar(179, 179, 255), helmetView);
	//	/*redHelmet = ((h > 0) & (h < 6) | (h > 175) & (h < 179)) & 
	//		(s > 92) & (s < 230) & 
	//		(v > 200) & (v <= 255); */
	//
	//	redHelmet = ((h >= 1) & (h <= 4) | (h >= 177) & (h <= 180)) & 
	//		         (s >= 90) & (s <= 220) & (v >= 140) & (v <= 256);
	//	//redHelmet = (h >= 168) & (h <= 178) & (s >= 1) & (s < 220) & (v >= 140) & (v <= 256);
	//	//yellowHelmet = (h > 20) & (h < 34) & (s > 58) & (s < 215) & (v > 180) & (v < 255); 
	//	yellowHelmet = (h >= 20) & (h <= 39) & (s >= 26) & (s <= 177) & (v >= 180) & (v <= 256); 

	vector<ColorRange> clrHelmet;

	clrHelmet.push_back(Red());
	clrHelmet.push_back(Yellow());	
	
	return clrHelmet;
}

vector<ColorRange> CigaretteColor()
{
	vector<ColorRange> clrCigarette;

	clrCigarette.push_back(White());

	return clrCigarette;
}

vector<ColorRange> SkinColor()
{
	ColorRange rgs;

	rgs.h.push_back(Range(0, 20));	
	rgs.h.push_back(Range(150, 180));
	rgs.s.push_back(Range(10, 200));	
	rgs.v.push_back(Range(55, 220));

	vector<ColorRange> clrSkin;

	clrSkin.push_back(rgs);

	return clrSkin;	
}

Mat BuildHSVRange(Mat view, vector<Range> ranges)
{
	Mat result;
	for (int i = 0; i < ranges.size(); i++)
	{
		Mat  rgv = (view >= ranges[i].start) & (view <= ranges[i].end);

		if (i == 0)
			result = rgv;
		else
			result = result | rgv;
	}
	return result;
}

Mat DetectColorWithHSV(Mat view, vector<ColorRange> colorRanges)
{		
	Mat          hsvImg, h, s, v;
	vector<Mat>  channel;	

	cvtColor(view, hsvImg, CV_BGR2HSV);
	split(hsvImg, channel); 

	h = channel[0];
	s = channel[1];
	v = channel[2];	

	Mat hview, sview, vview, colorView;

	for (int i = 0; i < colorRanges.size(); i++)
	{
		Mat rgv; 
		hview = BuildHSVRange(h, colorRanges[i].h);
		sview = BuildHSVRange(s, colorRanges[i].s);
		vview = BuildHSVRange(v, colorRanges[i].v);

		rgv = hview & sview & vview;

		if (i == 0)
			colorView = rgv;
		else
			colorView = colorView | rgv;
	}	

#ifdef SHOW 	
	ShowImage("Color View", colorView);
#endif	

	return colorView;
}