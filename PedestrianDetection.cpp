#include "stdafx.h"

#include "PedestrianDetection.h"

vector<Rect> DetectPedestrian(Mat view)
{
	/*HOGDescriptor hog;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	vector<Rect> found, found_filtered; 
	hog.detectMultiScale(view, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);*/
	
	vector<Rect> found;
	CascadeClassifier upbodyDetector;
	upbodyDetector.load("haarcascade_fullbody.xml");
	//upbodyDetector.load("haarcascade_upperbody.xml");
	upbodyDetector.detectMultiScale(view, found);


	/*vector<Rect> found_filtered;
	for(int i = 0; i < found.size(); i++)
	{
		Rect r = found[i];
		int  j = 0;
		for(; j < found.size(); j++)
		{
			if(j != i && (r & found[j]) == r)
				break;
		}
		if(j == found.size())
			found_filtered.push_back(r);
	}*/
	
#ifdef SHOW
	//Mat img = view.clone();
	//for(int i = 0; i < found.size(); i++)
	//	rectangle(img, found[i], Scalar(255, 0, 0), 2);
	//ShowImage("DetectPedestrian", img);
#endif

	/*if (found.size() == 0)
		return found;

	Mat img = view.clone();
	for(int i = 0; i < found.size(); i++)
		rectangle(img, found[i], Scalar(255, 0, 0), 2);	

	namedWindow("DetectPedestrian", CV_WINDOW_NORMAL);
	imshow("DetectPedestrian", img);
	waitKey();*/
	return found;
}