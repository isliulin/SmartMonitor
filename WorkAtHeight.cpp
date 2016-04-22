// WorkAtHeight.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "VisionLib.h"
#include "MovementDetection.h"
#include "HelmetsDetection.h"
#include "WorkAtHeight.h"
#include "PedestrianDetection.h"

vector<Rect> DetectWorkers(Mat frame)
{
	vector<Rect> workers = DetectPedestrian(frame);
	//vector<Rect> helmets = DetectHelmet(frame);
	//int heightRate = 8;

	/*for (int i = 0; i < helmets.size(); i++)
	{
		workers.push_back(Rect(helmets[i].x, helmets[i].y, 
		                       helmets[i].width, helmets[i].height * heightRate));

#ifdef SHOW
	rectangle(frame, workers[i], Scalar(255), 2); 	
#endif
	}*/
	//ShowImage("workers", frame);
	return workers;
}

Mat GetSuspenderRegion(Mat Frame, vector<Point> roi)
{
	Rect roiRect = boundingRect(Mat(roi));

	int x, y, width, height, margin = 50;	
	x = roiRect.x - margin;
	y = 0;
	width  = roiRect.width + 2 * margin;
	height = Frame.size().height;

	Rect detectionRect = Rect(x, y, width, height);
	Mat  detectionROI(Frame, detectionRect);
	
	return detectionROI;	
}

bool DetectSuspender(Mat frontView, Mat nextView)
{
	Mat diff, diffbw;
	diff = GetFrameDiff(frontView, nextView);
	threshold(diff, diffbw, 50, 255.0, THRESH_BINARY); 

	Mat blurView;

	StrengthenFeatures(diffbw, blurView, 15, 150, 4);
	//Filter(diffbw, blurView); 
	//blur(diffbw, blurView, Size(3,3)); 
	//medianBlur(diffbw, blurView, 15);    
	////GaussianBlur(diffbw, blurView, Size(11, 11), 1.5);

	//Mat element = getStructuringElement(MORPH_ELLIPSE, Size(4, 4), Point(1, 1));
	//dilate(blurView, blurView, element);  
	//bwareaopen(blurView, 150);  

#ifdef SHOW
	//ShowImage("blur, dilate and remove small blobs", blurView);
#endif	

	vector<vector<Point>> suspender;	
	findContours(blurView, suspender, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	if (suspender.size() == 0)
		return false;

	Rect suspenderRect = boundingRect(Mat(suspender[0]));   

#ifdef SHOW
	//Mat suspenderView(blurView.size(), CV_8U, Scalar(0));
	//drawContours(suspenderView, suspender, -1, Scalar(255), 2);		
	//rectangle(suspenderView, suspenderRect, Scalar(255), 2); 
	//ShowImage("suspenderRect", suspenderView);
#endif

	int groundClearance = 0;  //300
	if (nextView.size().height - suspenderRect.y > groundClearance)
		return true;

	return false;
}

vector<Rect> IntoUnsafeArea(vector<Point> unsafeArea, vector<Rect> workers, Mat frame)
{		
	vector<Rect>          irregularWorkers;
	vector<vector<Point>> contours(1);

	contours[0] = unsafeArea;

	for (int i = 0; i < workers.size(); i++)
	{
		Rect footRect = workers[i];
		footRect.y = footRect.y + footRect.height * 0.8;
		footRect.height = footRect.height * 0.2;

		Mat workerImg(frame.size(), CV_8U, Scalar(0));
		Mat areaImg(frame.size(), CV_8U, Scalar(0));
		
		/*-------------------------------------------------*/
		// detect edge of worker to compute intersection of worker and unsafe area
		/*Mat worker = Mat(frame, workers[i]); 		
		cvtColor(worker, worker, COLOR_BGR2GRAY); 
		
		Mat workerEdge;
		Canny(worker, workerEdge, 125, 350); 
		#ifdef
		ShowImage("workerEdge", workerEdge);
		#endif

		Mat roi = Mat(workerImg, workers[i]);
		workerEdge.copyTo(roi);  
		#ifdef
		ShowImage("workerEdge", workerImg);
		#endif   */
		/*------------------------------------------------*/
		
		rectangle(workerImg, footRect, Scalar(255, 255, 255), CV_FILLED);		
		drawContours(areaImg, contours, -1, Scalar(255, 255, 255), -1);
		Mat	intersection = workerImg & areaImg; 
		
	/*	namedWindow("work", CV_WINDOW_NORMAL);
		imshow("work", workerImg);
		namedWindow("area", CV_WINDOW_NORMAL);
		imshow("area", areaImg);	
		waitKey();*/
#ifdef SHOW		
		//ShowImage("work", workerImg);
		//ShowImage("area", areaImg);	
		//ShowImage("intersection", intersection);		
#endif

		if (countNonZero(intersection) > 0)
			irregularWorkers.push_back(workers[i]);
	}

	return irregularWorkers;
}

// detect the violation of work at height regulations
vector<Rect> DetectWorkAtHeight(Mat frontFrame, Mat nextFrame, vector<Point> unsafeArea)
{
	vector<Rect> irregularWorkers;
	Mat frontView = GetSuspenderRegion(frontFrame, unsafeArea);
	Mat nextView  = GetSuspenderRegion(nextFrame, unsafeArea); 

	if (DetectSuspender(frontView, nextView))
	{
		vector<Rect> workers = DetectWorkers(nextFrame);
		irregularWorkers = IntoUnsafeArea(unsafeArea, workers, nextFrame);
	}

#ifdef SHOW
	for (int i = 0; i < irregularWorkers.size(); i++)
		rectangle(nextFrame, irregularWorkers[i], Scalar(0, 0, 255), 2);
	ShowImage("irregularWorkers", nextFrame);
#endif
	
	return irregularWorkers;
}