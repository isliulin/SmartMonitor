#include "stdafx.h"

#include "math.h"
#include "VisionLib.h"
#include "MovementDetection.h"
#include "GrinderDetection.h"
#include "PeopleDetection.h"

Mat GetGrinderView(Mat view)
{
	Mat diff = view.clone();
	diff = diff > 5;

	StrengthenFeatures(diff, diff, 5, 10.0, 7);	

	Mat element = getStructuringElement(MORPH_RECT, Size(30, 30), Point(1, 1));
	erode(diff, diff, element);

#ifdef SHOW
	ShowImage("erode diff", diff);	
#endif

	return diff;
}

void DetectRope(vector<Mat> frames)
{
	/*vector<Point2f> centroids;

	for (int i = 0; i < frames.size(); i++)
	{
		vector<Point2f> ctds = DetectRope(frames[i]);
		centroids.insert(centroids.end(), ctds.begin(), ctds.end());
	}
	Scalar sc = mean(centroids);*/
}

//vector<Point2f> DetectRope(Mat grinderView)
//{
//	Mat view;
//	cvtColor(grinderView, view, COLOR_BGR2GRAY);
//	equalizeHist(view, view);
//	view = view > 150;
//
//	ShowImage("equalizeHist", view);
//	view *= 255.0; 
//	
//	int size = 2;  
//	Mat kernel = Mat::ones(size, size, CV_32F) / (float)(size * size);  
//	filter2D(view, view, -1, kernel);	
//
//	ShowImage("filter2D", view);
//
//	Mat view1 = view > 130;
//	Mat view2 = view < 220;
//	Mat view3 = view1 & view2;
//	ShowImage("view3", view3);
//
//	Mat image = view3 * 255.0;
//	filter2D(image, image, -1, kernel);
//
//	Mat image1 = image > 130;
//	Mat image2 = image < 220;
//	Mat image3 = image1 & image2;
//
//	ShowImage("二次滤波", image3);
//	
//	cv::morphologyEx(image3, image3, cv::MORPH_CLOSE, 
//		cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(6, 6)));
//
//	ShowImage("imclose", image3);
//
//	vector<vector<Point>> ropes;
//	ropes = bwareaopen(image3, 30.0);	
//
//	ShowImage("bwareaopen", image3);
//
//	//计算轮廓矩
//	vector<Moments> mu(ropes.size());
//	for( int i = 0; i < ropes.size(); i++ )
//	{ mu[i] = moments( ropes[i], false ); }
//
//	//计算轮廓的质心
//	vector<Point2f> mc(ropes.size());
//	for( int i = 0; i < ropes.size(); i++ )
//	{ mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }
//
//	//画轮廓及其质心
//	Mat drawing = Mat::zeros( image3.size(), CV_8UC3 );
//	RNG rng(12345);
//	for( int i = 0; i< ropes.size(); i++ )
//	{
//		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//		drawContours( drawing, ropes, i, color, 2);
//		circle( drawing, mc[i], 4, CV_RGB(255,0,0), -1, 8, 0 );
//	}
//	ShowImage("centroid", drawing);
//
//	Mat mt = Mat(mc);
//	Scalar a = mean(mc);
//	//cout << a;
//
//	return mc;
//}

double EvaluateSlopeAngle(Vec4i line)
{	
	if (line[2] - line[0] == 0)
		return 90.0;
	double theta = (double)(line[3] - line[1]) / (double)(line[2] - line[0]);

	return atan(theta) / CV_PI * 180.0;
}

Scalar FindPullingRope(Mat view)
{
	Mat diff = view.clone();
	diff = diff > 10;
	StrengthenFeatures(diff, diff, 5, 50.0, 5);	
	Canny(diff, diff, 150, 350);
	threshold(diff, diff, 128, 255, THRESH_BINARY);

	vector<Vec4i> lines;
	vector<Vec4i> ropeLines;
		 
	HoughLinesP(diff, lines, 1, CV_PI/180, 80, 50, 10);

	for(int i = 0; i < lines.size(); i++)
	{
		if (abs(EvaluateSlopeAngle(lines[i])) > 10.0)
		{
			ropeLines.push_back(lines[i]);
			//line(frame, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(0, 255, 0), 2);
		}		
	}
	//ShowImage("lines", frame);	
	
	Scalar sc;
	if (ropeLines.size() != 0)
		sc = mean(ropeLines);
	
	return sc;
}

Scalar FindPullingRope(vector<Mat> diffs, Rect rectGrinder)
{
	Rect rectRope;
	rectRope.x = 0;
	rectRope.y = 0;
	rectRope.width = diffs[0].cols;
	rectRope.height = rectGrinder.y + rectGrinder.height / 2;

	vector<Scalar> locations;
	for (int i = 0; i < diffs.size(); i++)
		locations.push_back(FindPullingRope(diffs[i](rectRope)));
		//locations.push_back(FindPullingRope(frames[i](rectRope), frames[i + 2](rectRope)));

	return mean(locations);	
}

Rect LocalizeGrinder(vector<Mat> diffs)
{
	Mat grinderView;	

	for(int i = 0; i < diffs.size(); i++)
	{
		Mat view = GetGrinderView(diffs[i]);

		if (grinderView.empty())
			grinderView = view;
		else
			grinderView = grinderView & view;				
	}
#ifdef SHOW
	//ShowImage("grinderView", grinderView);
#endif

	vector<Rect> rcs = FindConnectedDomain(grinderView);

	if (rcs.empty())
		return Rect();
	else
	{
		ExpandRect(grinderView, rcs[0], 50);
#ifdef SHOW
		//rectangle(grinderView, rcs[0], Scalar(255), 2);
		//ShowImage("grinder", grinderView);
#endif
		return rcs[0];
	}	
}

int DetectTurnsOfSteelRope(Mat view)
{
	cvtColor(view, view, COLOR_BGR2GRAY); 
	equalizeHist(view, view);
	threshold(view, view, 160, 255.0, THRESH_BINARY);	
	bwareaopen(view, 25.0);
	
	//ShowImage("view", view); waitKey();

	int cellsize = 8;
	int rowStep, colStep; 

	// 在view上每次移动8*8的region，计算该region中黑白的个数之差
	// 如果差值小于阈值，则作为一个合格的绳子块，以此记录一行的绳子数
	if(view.rows > cellsize)
		rowStep = (view.rows + 1) - cellsize; 

	if(view.cols > cellsize)
		colStep = (view.cols + 1) - cellsize;
		
	double threshold = cellsize * cellsize / 3.0;
	vector<int> numsRope(rowStep);

	if((rowStep > 0) && (colStep > 0))
	{		
		for (int i = 0; i < rowStep; i++)
		{
			numsRope[i] = 0;
			for (int j = 0; j < colStep; j++)
			{
				Mat cell = view(Range(i, i + cellsize), Range(j, j + cellsize));				
				Scalar sc = sum(sum(cell / 255));
				int white = sc[0];
				int black = cellsize * cellsize - white;				
				if(abs(white - black) < threshold)
					numsRope[i]++;
			}
		}					
	}
	Scalar turns = mean(numsRope) / 5;
	////cout << turns << endl;
	return turns[0];
}

int DetectTurnsOfSteelRope(vector<Mat> frames, Rect rectGrinder)
{
	vector<double> numsRope(frames.size());

	for (int i = 0; i < frames.size(); i++)
	{
		Mat viewGrinder = frames[i](rectGrinder);
		numsRope[i] = DetectTurnsOfSteelRope(viewGrinder);		
	}
	//cout << (int)mean(numsRope)[0] << endl;
	return (int)(mean(numsRope)[0]+2);
}

Rect GetCoilRegion(Mat view, Rect rectGrinder, Scalar pullingRope)
{
	Rect rectCoil;
	double xOffset = 20.0;
	double yOffset = 20.0;

	rectCoil.x = xOffset + (pullingRope[0] + pullingRope[2]) / 2.0;
	rectCoil.y = 0;
	rectCoil.width  = view.cols - rectCoil.x;
	rectCoil.height = rectGrinder.y - yOffset;

#ifdef SHOW
	rectangle(view, rectCoil, Scalar(255), 2);
	ShowImage("rect coil", view);
#endif	

	return rectCoil;
}

Mat DetectCoil(Mat view)
{
//	GaussianBlur(view, view, Size(5, 5), 3);
//	cvtColor(view, view, COLOR_BGR2GRAY); 
//
//	int scale = 1, delta = 0;
//	int ddepth = CV_16S;
//	Mat grad_x, grad_y;
//	Mat abs_grad_x, abs_grad_y;
//	Mat grad;
//	
//	Sobel(view, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);	
//	convertScaleAbs(grad_x, abs_grad_x);
//	
//	Sobel(view, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
//	convertScaleAbs(grad_y, abs_grad_y);
//
//	// Total Gradient (approximate)
//	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
//
//#ifdef SHOW
//	ShowImage("addWeighted", grad);
//#endif
//
//	grad = grad > 50;
//	bwareaopen(grad, 80.0);
//
//#ifdef SHOW
//	ShowImage("Coil", grad);
//#endif
//	return grad;
  Mat src,gray,dst,bwimg,bwconvert,I4,I5;
  src=view;
  if(src.empty())
  {
	  return src;
  }
  cvtColor(src, gray, CV_BGR2GRAY);
  equalizeHist( gray, dst );
  threshold(dst,bwimg, 128,255, THRESH_OTSU);
  bwareaopen(bwimg, 60);//remove small white block
  ////imshow("bw",bwimg);
  ////cout<<"bwimg"<<bwimg<<endl;
  bwconvert=abs(bwimg-255);
  bwareaopen(bwconvert, 60);//remove small black block
  ////imshow("bw1",bwconvert);
  bwconvert=abs(bwconvert-255);
  bwimg=bwimg+bwconvert;
  Mat element = getStructuringElement(MORPH_RECT, Size(4, 4), Point(2, 2)); //, Point(2, 2)
  erode(bwimg, I4, element);
  dilate(I4, I4, element); 
  element = getStructuringElement(  MORPH_RECT, Size(5, 5), Point(2, 2)); 
  erode(bwimg, I5, element); 
  element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(2,2)); 
  dilate(I5, I5, element); 
 // bwimg=(I4==I5)==0;//bwimg:0/255
 // //cout<<"bwimg"<<bwimg<<endl;
 // //imshow("bwimg",bwimg);
  bwimg=abs(I4-I5);
  ////imshow("bwimg1",bwimg);
  bwareaopen(bwimg,60);
  medianBlur(bwimg,bwimg,3);
  bwareaopen(bwimg,60);
 // //imshow("bwimg",bwimg);
  return bwimg;
}

// Is there a coil in the rectangle region of the view
bool ExistCoil(Mat view, Rect rc)
{
	if (countNonZero(view(rc)) > 0)
		return true;
	else
		return false;
}

//绳子右边拓展区域人检测
vector<Rect> GetCoilRegionPeople(Mat img)
{
	    vector<Rect> hogfound;
	     vector<Rect> dpmfound;
		 Mat src=img;
         Mat srcimg=src;//原图
		 resize(src, src, Size(320,240), 0, 0, INTER_NEAREST);//缩放以加速
		 double widthscale=srcimg.cols/320.0;
		 double heightscale=srcimg.rows/240.0;
		 hogfound=HogDetectPeople(src); 
		  #ifdef SHOW
		   for(int i=0; i<hogfound.size(); i++)
         {
             Rect r = hogfound[i];
			 rectangle(src, r.tl(), r.br(), Scalar(255,0,0), 3);
         }
		   //imshow("people1",src);
		   #endif

		 if(hogfound.size())
		 { dpmfound=DpmDetectPeople(src,hogfound);}

		 #ifdef SHOW
		 for(int i=0; i<dpmfound.size(); i++)
         {
             Rect r = dpmfound[i];
			 rectangle(src, r.tl(), r.br(), Scalar(0,255,0), 3);
         }
		 //imshow("people1",src);


		 #endif
		 for(int i=0; i<dpmfound.size(); i++)//转换到原图像尺度
         {
			 dpmfound[i].x*=widthscale;
			 dpmfound[i].y*=heightscale;
             dpmfound[i].width*=widthscale;
			 dpmfound[i].height*=heightscale;
         }
		 #ifdef SHOW
		  for(int i=0; i<dpmfound.size(); i++)
         {
             Rect r = dpmfound[i];
			 rectangle(srcimg, r.tl(), r.br(), Scalar(255,0,0), 3);
         }
		 //imshow("people",srcimg);
         #endif
		 return dpmfound;
}


// 查看rc（操作人员的脚部）的左下或右下是否coil，如果有即表明有人stand on the coil
//已做边界检查
bool StandOncoil(Mat view, Rect rc)
{
	Rect leftRect, rightRect, bottomRect;
	int width = 20;
	int height = 20;
	
	//---------------------------------//
	if (rc.x - width < 0)
	{
		leftRect.x = 0;
		leftRect.width = rc.x;
	}
	else
	{
		leftRect.x = rc.x - width;
		leftRect.width = width;
	}

	leftRect.y = rc.y;	
	leftRect.height = rc.height;
	//---------------------------------//
	rightRect.x = rc.x + rc.width;
	if (rc.x + width > view.cols)
		rightRect.width = view.cols - rc.x;
	else
		rightRect.width = width;

	rightRect.y = rc.y;	
	rightRect.height = rc.height;
	//---------------------------------//
	bottomRect.x = leftRect.x;
	bottomRect.y = rc.y + rc.height;
	bottomRect.width = leftRect.width + rc.width + rightRect.width;
	
	if (bottomRect.y + height > view.rows)
		bottomRect.height = view.rows - bottomRect.y;
	else
		bottomRect.height = height;
	//---------------------------------//
	/*rectangle(view, rc, Scalar(255), 2);
	rectangle(view, leftRect, Scalar(255), 2);
	rectangle(view, rightRect, Scalar(255), 2);
	rectangle(view, bottomRect, Scalar(255), 2);
	ShowImage("left right bottom", view);	*/

	bool br = ExistCoil(view, bottomRect);
	if((ExistCoil(view, leftRect) & br) || (ExistCoil(view, rightRect) & br))
		return true;
	else
		return false;
}

int DetectOperators(vector<Mat> diffs, Mat coil, Rect rectCoil)
{
	int operatingState;

	for (int i = 0; i < diffs.size(); i++)
	{
		Mat diff = diffs[i](rectCoil);
		diff = diff > 10;
		StrengthenFeatures(diff, diff, 5, 20.0, 15);	
		vector<Rect> rcs = FindConnectedDomain(diff);

		Mat view = coil.clone();
		for (int j = 0; j < rcs.size(); j++)
		{
			bool b = StandOncoil(coil, rcs[j]);		
			
			if(b)
				rectangle(view, rcs[j], Scalar(255), 2);
		}
		ShowImage(format("diff rect coil: %d", i), view);
	}
	
	waitKey();
	return operatingState;
}

vector<int> DetectGrinderOperation(vector<Mat> frames)
{
	vector<int> operatingStates;
	Mat view = frames[frames.size() - 1];
	vector<Mat> diffs;

	for (int i = 0; i < frames.size() - 3; i++)
		diffs.push_back(GetFrameDiff(frames[i], frames[i + 2]));
	
	//-----------------------------------------------------------//
	// The number of turns of steel rope which wrapped 
	// around the grinder should be 5 turns.	
	Rect rectGrinder = LocalizeGrinder(diffs);

#ifdef SHOW
	rectangle(view, rectGrinder, Scalar(255), 2);
	ShowImage("rect grinder", view);	
#endif

	if (rectGrinder.width > 0)
	{
		if(DetectTurnsOfSteelRope(frames, rectGrinder) == 5)
			operatingStates.push_back(REGUALR_TURNS);
		else
			operatingStates.push_back(NO_5TURNS);

	Scalar sc = FindPullingRope(diffs, rectGrinder);//帧差结果对绳子检测是否存在影响？

#ifdef SHOW	
	line(view, Point(sc[0], sc[1]), Point(sc[2], sc[3]), Scalar(0, 255, 0), 2);
	ShowImage("pulling rope", view);
#endif
	if((sc[0]>0)&(sc[1]>0)&(sc[2]>0)&(sc[3]>0))
	{//-----------------------------------------------------------//
	// The operators should not stand on the coil of steel rope.
	Rect rcCoil = GetCoilRegion(view, rectGrinder, sc);	//绳子拓展之后的区域
	//在此拓展区域检测
	if((rcCoil.height>0)&(rcCoil.width>0))
	{Mat coilRegion=view(rcCoil);
	#ifdef SHOW	
	ShowImage("regionrope", coilRegion);
    #endif
	//在该区域检测人的站位（绳子有无经过人的区域）
	
	//just for test----------------------------------------
	/*view=imread("127.jpg");
	resize(view, view, Size(1440,1080), 0, 0, INTER_NEAREST);
	coilRegion=view;
	#ifdef SHOW	
	ShowImage("regionrope", coilRegion);
    #endif*/
	//-----------------------------------------------------
	vector<Rect> coilPeople=GetCoilRegionPeople(coilRegion);
	////just for test----------------------------------------
	///*Rect js;
	//js.y=0;
	//js.x=coilRegion.cols*0.4;
	//js.width=coilRegion.cols*0.25;
	//js.height=coilRegion.rows*0.7;
	//#ifdef SHOW
	//rectangle(coilRegion, js, Scalar(255), 2);
	//ShowImage("regionrope",coilRegion);	
 //   #endif
	//coilPeople.push_back(js);*/
	////-----------------------------------------------------
	for(int i=0; i<coilPeople.size(); i++)//转换到原图像尺度
        {
			coilPeople[i].x+=rcCoil.x;
			coilPeople[i].y*=rcCoil.y;
			rectangle(view, coilPeople[i], Scalar(255), 2);
        }
	////just for test-------------------------------------------
	///*int jjdk=view.rows;
	//int jjkd=view.cols;
	//#ifdef SHOW
	//
	//ShowImage("view",view);	
 //   #endif*/
	////--------------------------------------------------------
	for(int i=0; i<coilPeople.size(); i++)
	{
	   vector<Scalar> locations1;
	   for (int j = 0; j < diffs.size(); j++)
	   {
		   Rect nowrect=coilPeople[i];
		   locations1.push_back(FindPullingRope(diffs[j](nowrect)));
	   }
    	Scalar sc1= mean(locations1);//对于一个人，框内找绳索
		if(abs(sc1[0]-sc1[2])<coilPeople[i].width*0.15)
			operatingStates.push_back(PEOPLE_STAND_WRONG);//代表有站位危险
	 }
	////检测脚下有没有绳子的圈
		 for(int i=0; i<coilPeople.size(); i++)
         {
			 Rect roi=coilPeople[i];	
			 roi.y+=roi.height*0.75;//测试过，框选脚区域正确
			 roi.height*=0.25;
			 Rect r;
			 r=roi;//feet area
			 rectangle(view, r, Scalar(0,0,255), 2);
			 if((roi.x-roi.width>0)&(roi.x+2*roi.width<view.cols)&(roi.y+roi.height*2<view.rows))
			 { 
				 roi.x-=roi.width;//测试正确
				 roi.width*=3;
				 roi.height*=2;
				 //just for test---------------------------
				 // rectangle(view, roi, Scalar(0,0,255), 2);
				 //-----------------------------------------
                 Mat coil  = DetectCoil(view(roi));//寻找绳子
				 Rect newroi;
				 newroi.y=r.y-roi.y;//feet area,测试正确
				 newroi.x=r.x-roi.x;
				 newroi.width=r.width;
				 newroi.height=r.height;
				 //just for test------------------------------
				/* Mat xx=view(roi);
				 rectangle(xx, newroi, Scalar(0,255,0), 2);
				 //imshow("coi",xx);
				 //imshow("coi1",coil);*/
				 //--------------------------------------------
				 coil(newroi)=0;
				 //imshow("coi2",coil);
				// //row:newroi.y+newroi.height/2;
				// //col:newroi.x+newroi.width/2;
				 int pointrow=newroi.y+newroi.height;
				 int pointcol=newroi.x+newroi.width/2;//测试正确
				 int colnum=0;
				 int rownum=0;
				 int a,b,c;
				 a=0;b=0;
				 for(int j=0;j<coil.cols;j++)
				 {
				   c=coil.at<uchar>(pointrow, j);
				   if (c>0)
					   a++;
				 }
				  for(int i=pointrow;i<coil.rows;i++)
				 {
				   c=coil.at<uchar>(i, pointcol);
				   if (c>0)
					   b++;
				 }
				  if((a>4)&(b>4))
					  operatingStates.push_back(STAND_ON_COIL);//有圈危险
				
			 }
         }
		}
	 }
	     ////imshow("peop",view);
	}	

	return operatingStates;
}