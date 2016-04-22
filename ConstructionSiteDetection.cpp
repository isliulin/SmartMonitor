#include "stdafx.h"
#include "ConstructionSiteDetection.h"
#include "DetectHelmetBelt.h"
#include "HelmetsDetection.h"
#include "SmokeDetection.h"
#include "GrinderDetection.h"
#include "PedestrianClassDetection.h"
#include "VisionLib.h"


Rect GetHeadRect(Rect PedestrianRect)
{	
	Rect rc = PedestrianRect;
	if(rc.y<rc.height*0.1)
	{
		rc.y=0;
	}else{
		rc.y = rc.y-rc.height*0.1;
	}

	rc.height=rc.height*0.5;
	return rc;
}



vector<Rect> GetFaceRect(Mat src,Rect HogHeadRect)
{
	//人脸检测参数
	CascadeClassifier faceDetector;
	faceDetector.load("haarcascade_frontalface_alt2.xml");
	ExpandRectDifSize(src,HogHeadRect,0,0);
	Mat HogHeadImg = src(HogHeadRect);
	//-------------人脸识别--------------
	vector<Rect> faces;
	faceDetector.detectMultiScale(HogHeadImg,faces);
	for(int i=0;i<faces.size();i++)
	{
		faces[i].x+=HogHeadRect.x;
		faces[i].y+=HogHeadRect.y;
	}
	return faces;

}




vector<Rect> GetHelmetRect(Mat src,Rect HogHeadRect)
{
	ExpandRectDifSize(src,HogHeadRect,0,0);
	Mat HogHeadImg = src(HogHeadRect);

//	vector<Rect> helmet =DetectColorWithHSV(HogHeadImg,HelmetColor());
	vector<Rect> helmet = DetectHelmet(HogHeadImg);
	vector<Rect> faces=GetFaceRect(src,HogHeadRect);
	for( int j=0; j<faces.size(); j++ )
	{

		Rect hrc= faces[j];
		hrc.y=faces[j].y-faces[j].height*0.5;
		ExpandRectDifSize(src,hrc,0,0);
		Mat helmelFaceImg = src(hrc);
//		vector<Rect> helmelFaceRect =DetectColorWithHSV(helmelFaceImg,HelmetColor());
		vector<Rect> helmelFaceRect =DetectHelmet(helmelFaceImg);
		for( int k=0; k<helmelFaceRect.size(); k++ )
		{	
			helmelFaceRect[k].x+=hrc.x;
			helmelFaceRect[k].y+=hrc.y;
			helmet.push_back(helmelFaceRect[k]);
		}
	}
	for(int m=0;m<helmet.size();m++)
	{
		helmet[m].x+=HogHeadRect.x;
		helmet[m].y+=HogHeadRect.y;
	}
	return helmet;
	
}




vector<Rect> GetSmokeRect(Mat faceImg,Rect FaceRect)
{
	vector<Rect> smokeRect;
	
	/*ExpandRectDifSize(src,FaceRect,FaceRect.width*0.1,FaceRect.height*0.1);
	Mat faceImg = src(FaceRect);*/
	smokeRect =DetectSmoke(faceImg);
	for(int i=0;i<smokeRect.size();i++)
	{
		smokeRect[i].x+=FaceRect.x;
		smokeRect[i].y+=FaceRect.y;
	}
	return smokeRect;

}




Rect GetSmokeResult(Mat &src,Rect PedestrianRect,vector<Rect> helmetRectFirst)
{
	CvRNG rng;
	rng= cvRNG(cvGetTickCount());
	
	Rect HogHeadRect;
	vector<Rect> smokeRect;

	for(int j=0;j<helmetRectFirst.size();j++)
		{
			if(helmetRectFirst[j].width<10||helmetRectFirst[j].height<10)
			{
				continue;
			}else{
				HogHeadRect =GetHeadRect(helmetRectFirst[j]);
				vector<Rect> faces=GetFaceRect(src,HogHeadRect);
				
				for(int k=0;k<faces.size();k++)
				{
				//	smokeRect = GetSmokeRect(src,faces[k]);
					//检测人脸，并识别名字
					ExpandRectDifSize(src,faces[k],0,0);
					Mat faceImg = src(faces[k]);
					smokeRect = GetSmokeRect(faceImg,faces[k]);
					
					
					if(smokeRect.size()>0)
					{						
						rectangle(src,PedestrianRect,CV_RGB(255,0,0),3);
						string smokestr ="someone is smoking";
				//		putText(src, smokestr, Point(faces[k].x+faces[k].width,faces[k].y+faces[k].height), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255), 3);
				//		imwrite("D://Error//"+to_string((long double)rng)+"smokeError.jpg",src);
						return PedestrianRect;
					}
				}
			}
	}
	return Rect(0,0,0,0);

}




//************************************
//* 输入：输入5帧的Mat图像，第一帧和最后一帧相差50
//* 输出：未带安全帽的人形坐标位置:noHelmetPedestrianRect
//*       未系好安全带的人形坐标位子：
//*       吸烟的人形坐标位置:smokePedestrianRect
//*       马甲人员是否到位：ClassDetectionResult（true：到位（不报警）；false:不到位（报警））     
//*************************************/
void DetectConstructionSite(vector<Mat> orgMat,vector<Rect> &noHelmetPedestrianRect,vector<Rect> &smokePedestrianRect,bool &ClassDetectionResult,vector<Rect> &noBeltPedestrianRect)
{

	CvRNG rng;
	rng= cvRNG(cvGetTickCount());
	vector<Rect> PedestrianRect;
	vector<Rect> helmetRect;
	vector<Rect> smokeRect;
	vector<Rect> helmetBeltRect;
	vector<Rect> supervisorRectPerMat; 
	Rect HogHeadRect;
	Mat src = orgMat[0];

	PedestrianRect = GetCoilRegionPeople(src);
	for(int i=0;i<PedestrianRect.size();i++)
	{
		HogHeadRect=GetHeadRect(PedestrianRect[i]);
		helmetRect=GetHelmetRect(src,HogHeadRect);
		vector<Rect> faces=GetFaceRect(src,HogHeadRect);
		//检测到未系安全带
		helmetBeltRect = DetectHelmetBelt(faces,src);
		if(helmetBeltRect.size()>0)
		{
			noBeltPedestrianRect.push_back(PedestrianRect[i]);
		}
		for(int j=0;j<faces.size();j++)
		{
			//检测到吸烟
			smokeRect = GetSmokeRect(src,faces[j]);
			smokePedestrianRect.push_back(PedestrianRect[i]);
		}
		//未检测到安全帽
		if(helmetRect.size()==0)
		{
			noHelmetPedestrianRect.push_back(PedestrianRect[i]);
		}

		//检测是否穿了马甲
		int isOn = DetectSupervisorClass(PedestrianRect[i],src);
		if(isOn==1)
		{
			supervisorRectPerMat.push_back(PedestrianRect[i]);
		}
	}

	vector<Rect> supervisorPedRect;
	for(int k =1;k<orgMat.size();k++)
	{
		supervisorPedRect = GetCoilRegionPeople(orgMat[k]);
		for(int s =0;s<supervisorPedRect.size();s++)
		{
			int isOn1 = DetectSupervisorClass(supervisorPedRect[s],orgMat[k]);//Bug点
			if(isOn1==1)
			{
				for(int w =0;w<supervisorRectPerMat.size();w++)
				{
					if(abs(supervisorRectPerMat[w].x-supervisorPedRect[s].x)>20&&abs(supervisorRectPerMat[w].y-supervisorPedRect[s].y)>20)
					{
						supervisorRectPerMat.push_back(supervisorPedRect[s]);
					}
				
				}
			}
		}
	}

	if(supervisorRectPerMat.size()>=1)
	{
		ClassDetectionResult = true;
	}
	else
	{
		ClassDetectionResult = false;
	
	}
	
}













