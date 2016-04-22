#include "stdafx.h"
#include "VisionLib.h"

vector<vector<Point>> bwareaopen(Mat& im, double size)
{
	vector<vector<Point>> remains;

	// Only accept CV_8UC1
	if (im.channels() != 1 || im.type() != CV_8U)
		return remains;

	Mat view = Mat::zeros(im.size(), CV_8UC1);
	
	vector<vector<Point>> contours;
	findContours(im, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	
	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		
		// Remove small objects by drawing the large objects with white color
		if (area >= size)
		{
			drawContours(view, contours, i, Scalar(255), CV_FILLED);
			remains.push_back(contours[i]);
		}

		/*if (area > -1.0 && area <= size)
			drawContours(im, contours, i, CV_RGB(0,0,0),CV_FILLED);
		else			
			remains.push_back(contours[i]);*/
	}	
	im = view;
	return remains;
}

Mat bwperim(Mat im)
{
	int dilationType = MORPH_RECT;
	int dilationSize = 1;
	Mat onePixelWideElement = getStructuringElement( dilationType,
		Size( 2 * dilationSize + 1, 2 * dilationSize + 1),
		Point(dilationSize, dilationSize));

	Mat dilated; 
	dilate(im, dilated, onePixelWideElement);
	return dilated - im;	
}

void Filter(Mat src, Mat& dst)
{
	int size = 3;  
	Mat kernel = Mat::ones(size, size, CV_32F)/ (float)(size * size);  
	filter2D(src, dst, -1, kernel); 
}

void ShowImage(string name, Mat view)
{
	namedWindow(name, CV_WINDOW_NORMAL);
	//imshow(name, view);
}

void ExpandRect(Mat view, Rect& rc, int offset)
{
	int viewWidth  = view.size().width;
	int viewHeight = view.size().height;

	int exWidth = rc.width + offset * 2;
	int exHeight = rc.height + offset * 2;

	if (rc.x - offset < 0)
		rc.x = 0;
	else
		rc.x = rc.x - offset;

	if (rc.y - offset < 0)
		rc.y = 0;
	else
		rc.y = rc.y - offset;

	if (rc.x + exWidth > viewWidth)
		rc.width = viewWidth - rc.x;
	else
		rc.width += offset * 2;

	if (rc.y + exHeight > viewHeight)
		rc.height = viewHeight - rc.y;
	else
		rc.height += offset * 2;
}

void StrengthenFeatures(Mat src, Mat& dst, int blurSize, double removeSize, int dilateSize)
{	
	//Filter(view, view); 
	//blur(view, view, Size(3,3)); 
	medianBlur(src, dst, blurSize);    
	//GaussianBlur(view, view, Size(11, 11), 1.5);
	bwareaopen(dst, removeSize); 

	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(dilateSize, dilateSize), Point(2, 2)); 
	dilate(dst, dst, element);  
	//morphologyEx(view, view, MORPH_DILATE, element);	

#ifdef SHOW
	ShowImage("blur, dilate and remove small blobs", dst);
#endif	
}

vector<Rect> FindConnectedDomain(Mat view)
{
	vector<Rect>          rects;
	vector<vector<Point>> contours;	

	findContours(view, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	
	for (int i = 0; i < contours.size(); i++)
	{
		Rect rc = boundingRect(Mat(contours[i]));
		rects.push_back(rc);

#ifdef SHOW 			
		//rectangle(view, rc, Scalar(255), 2); 
#endif
	}	
#ifdef SHOW
	//drawContours(view, contours, -1, Scalar(255), -1);
#endif

	return rects;
}

vector<Rect> FindFeatures(Mat view, int blurSize, double removeSize, int dilateSize)
{	
	Mat featuresView;
	StrengthenFeatures(view, featuresView, blurSize, removeSize, dilateSize);

	return FindConnectedDomain(featuresView);
}

void Frame2Pic(VideoCapture vc, string path, Range rg, Rect rc)
{
	int totalFrameNumber = (int)vc.get(CV_CAP_PROP_FRAME_COUNT);  
	////cout << "The total number of the video is " << totalFrameNumber << " Frame" <<endl;  

	double rateFrame = vc.get(CV_CAP_PROP_FPS);  
	////cout<< "The rate of Frame is " << rateFrame << " fps" << endl;
	
	Mat frame;	
	string filename;
	
	for(int nframe = 1; nframe < totalFrameNumber; nframe++)
	{			
		filename = path + format("%d.jpg", nframe);
		vc.read(frame);
		Mat view = frame;

		if (rc.width > 0)
			view = frame(rc);

		if (rg.empty())
			imwrite(filename, view);
		else
		{
			if (nframe >= rg.start && nframe <= rg.end)
			{
				imwrite(filename, frame);

				if (nframe == rg.end)
					break;
			}
		}		
	}
}

void bwerasebigarea(Mat& im, double size)
{
	// Only accept CV_8UC1
	if (im.channels() != 1 || im.type() != CV_8U)
		return;

	// Find all contours
	vector<vector<Point> > contours;
	findContours(im.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++)
	{
		// Calculate contour area
		double area = contourArea(contours[i]);

		// Remove small objects by drawing the contour with black color
		if ( area >= size)
			drawContours(im, contours, i, CV_RGB(0,0,0), -1);
	}
}

Mat ZoomImage (Mat img)
{

	 const int DETECTION_WIDTH=640;
	 Mat smallImg;
	 float scale=img.cols/(float)DETECTION_WIDTH;
	 if(img.cols>DETECTION_WIDTH)
	 {
		 int scaledHight=cvRound(img.rows/scale);//缩放的高度
		 resize(img,smallImg,Size(DETECTION_WIDTH,scaledHight));
	 }else
	 {
		 smallImg=img;
	 }
	 return smallImg;
}

void ExpandRectDifSize(Mat view, Rect& rc, int offsetWidth,int offsetHeight  )
{
	int viewWidth  = view.size().width;
	int viewHeight = view.size().height;

	int exWidth = rc.width + offsetWidth * 2;
	int exHeight = rc.height + offsetHeight * 2;


	
		if (rc.x - offsetWidth < 0)
			rc.x = 0;
		else
			rc.x = rc.x - offsetWidth*0.5;

		if (rc.y - offsetHeight < 0)
			rc.y = 0;
		else
			rc.y = rc.y - offsetHeight;

		if (rc.x + exWidth > viewWidth)
			rc.width = viewWidth - rc.x;
		else
			rc.width += offsetWidth * 2;
	
		if (rc.y + exHeight > viewHeight)
			rc.height = viewHeight - rc.y;
		else
			rc.height += offsetHeight * 2;



	
}

