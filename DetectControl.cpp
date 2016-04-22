#include "stdafx.h"
#include "DetectControl.h"
#include "WorkDlg.h"
#include "WorkAtHeight.h"//高空作业
#include "GrinderDetection.h"//绞磨机作业
#include "ConstructionSiteDetection.h"//人员作业
#define  SPAN  120//保证同类错误两分钟内不重复发送到手机



//高空检测所需变量
//高空检测设备信息
long HighLoginID;
char* HighIP;
unsigned short  HighPort;
char* HighUser;
char* HighPass;
long HighPlayhandle;


Mat HighMat;
vector<Mat> ImgsForHighTest;
vector<CvPoint> PointArray;//图像坐标


//施工现场视频播放
Mat BehaviorMat;
vector<Mat> ImgsForBehaviorTest;

//施工检测设备信息
long BehaviorLoginID;
char* BehaviorIP;
unsigned short  BehaviorPort;
char* BehaviorUser;
char* BehaviorPass;
long BehaviorPlayhandle;



//绞磨机工作视频播放
//绞磨机检测设备信息
long MillLoginID;
char* MillIP;
unsigned short  MillPort;
char* MillUser;
char* MillPass;
long MillPlayhandle;


Mat MillMat;
vector<Mat> ImgsForMillTest;


//拓展窗口视频播放
//拓展检测设备信息
long MoreLoginID;
char* MoreIP;
unsigned short  MorePort;
char* MoreUser;
char* MorePass;
long MorePlayhandle;

Mat MoreMat;



//检测器控制
BOOL HighWorkDetecting;
BOOL BehaviorDetecting;
BOOL MillDetecting;




//确保一次只检测一种行为，降低CPU占用
CRITICAL_SECTION DetectionCritical;



CStatic BehaviorPic;
CStatic MillPic;
CStatic MorePic;
CListCtrl BehaviorGrid;
CListCtrl MillGrid;


//状态栏
CStatusBarCtrl StatusBar;
CListCtrl HighGrid;
CStatic HighPic;



//定时采集检测线程
DWORD WINAPI HighDetectProc(LPVOID lpParameter)
{
	if (HighWorkDetecting==TRUE)
	{
		return 0;
	}
	HighWorkDetecting = TRUE;

	StatusBar.SetText("正在检测高空作业", 1, 0);

	while (1)
	{
		char *pBuffer = NULL;
		int *pPicLen = NULL;
		H264_DVR_CatchPicInBuffer(HighLoginID, 
					0, pBuffer,1000000,pPicLen);
		//位图转Mat类型
		cvSetData(&HighMat,pBuffer,4);
		imshow("High",HighMat);
		Sleep(1000);
	}

	HighWorkDetecting = FALSE;
	return 0;
}










//施工行为检测
DWORD WINAPI BeheviorDetectProc(LPVOID lpParameter)
{
	if (BehaviorDetecting==TRUE)
	{
		return 0;
	}
	BehaviorDetecting = TRUE;
	vector<Mat> ImgsForBehaviorTest; 
	StatusBar.SetText("正在检测工人行为", 2, 0);
	while(1)
	{

		if (BehaviorDetecting==FALSE)
		{
			StatusBar.SetText("就绪", 2, 0);
			return 0;
		}

		if (!BehaviorMat.empty())
		{
			if (ImgsForBehaviorTest.size()< 5)
			{
				ImgsForBehaviorTest.push_back(BehaviorMat.clone());

				Sleep(100);
				continue;
			}
		}
		else
		{
			Sleep(1000);
			continue;
		}

		if (ImgsForBehaviorTest.size()==5)
		{
			vector<Rect> noHelmetPedestrianRect;
			vector<Rect> smokePedestrianRect;
			bool ClassDetectionResult = true;
			vector<Rect> noBeltPedestrianRect;


	       EnterCriticalSection(&DetectionCritical);
		   try
		   {
			   DetectConstructionSite(ImgsForBehaviorTest,noHelmetPedestrianRect,
				   smokePedestrianRect,ClassDetectionResult,
				   noBeltPedestrianRect);
		   }
		   catch (...)
		   {
		   	    //AfxMessageBox("施工检测出错");
		   }
		   LeaveCriticalSection(&DetectionCritical);



			if (noHelmetPedestrianRect.size()>0)
			{

				//手机报警系统
				static time_t LastTime = 0;
				static time_t PresentTime = 0;
				static time_t Span = 0;
				PresentTime = time(0);
				Span = PresentTime - LastTime;

				if (Span>SPAN)
				{
					LastTime = PresentTime;
					cvSaveImage("D:\\施工现场监控信息\\notificationPhoto\\"
						+GetSaveTime()+"安全帽.jpg",
						&(IplImage)ImgsForBehaviorTest[0]);
					WriteToCommandLine("2");
				}


				//显示所有检测信息
				InsertItemToGrid(BehaviorGrid,GetTime(),
					"有人未戴安全帽",1);
				cvSaveImage("D:\\施工现场监控信息\\报警图片\\"
					+GetSaveTime()+"安全帽.jpg",
					&(IplImage)ImgsForBehaviorTest[0]);
				WriteToRecordLine(GetTime()+ "       "+"有人未戴安全帽");


			}


			if (smokePedestrianRect.size()>0)
			{

				//手机报警系统
				static time_t LastTime = 0;
				static time_t PresentTime = 0;
				static time_t Span = 0;
				PresentTime = time(0);
				Span = PresentTime - LastTime;

				if (Span>SPAN)
				{
					LastTime = PresentTime;
					cvSaveImage("D:\\施工现场监控信息\\notificationPhoto\\"
						+GetSaveTime()+"吸烟.jpg",
						&(IplImage)ImgsForBehaviorTest[0]);
					WriteToCommandLine("3");
				}


				//显示所有检测信息
				InsertItemToGrid(BehaviorGrid,GetTime(),
					"有人吸烟",1);
				cvSaveImage("D:\\施工现场监控信息\\报警图片\\"
					+GetSaveTime()+"吸烟.jpg",
					&(IplImage)ImgsForBehaviorTest[0]);
				

				WriteToRecordLine(GetTime()+ "       "+"有人吸烟");

			}


			if (ClassDetectionResult==FALSE)
			{
				    
				//手机报警系统
				static time_t LastTime = 0;
				static time_t PresentTime = 0;
				static time_t Span = 0;
				PresentTime = time(0);
				Span = PresentTime - LastTime;

				if (Span>SPAN)
				{
					LastTime = PresentTime;
					WriteToCommandLine("4");
					cvSaveImage("D:\\施工现场监控信息\\notificationPhoto\\"
						+GetSaveTime()+"马甲.jpg",
						&(IplImage)ImgsForBehaviorTest[4]);
				}


				//显示所有检测信息
				InsertItemToGrid(BehaviorGrid,GetTime(),
						"监督人员未到位",1);
				cvSaveImage("D:\\施工现场监控信息\\报警图片\\"
						+GetSaveTime()+"人员未到位.jpg",
						&(IplImage)ImgsForBehaviorTest[4]);
				WriteToRecordLine(GetTime()+ "       "+"监督人员未到位");


			}


			if (noBeltPedestrianRect.size()>0)
			{
				//手机报警系统
				static time_t LastTime = 0;
				static time_t PresentTime = 0;
				static time_t Span = 0;
				PresentTime = time(0);
				Span = PresentTime - LastTime;

				if (Span>SPAN)
				{
					LastTime = PresentTime;
					cvSaveImage("D:\\施工现场监控信息\\notificationPhoto\\"
						+GetSaveTime()+"带子.jpg",
						&(IplImage)ImgsForBehaviorTest[0]);
					WriteToCommandLine("5");
				}


				//显示所有检测信息
				InsertItemToGrid(BehaviorGrid,
					GetTime(),"有人安全帽带子未系好",1);
				cvSaveImage("D:\\施工现场监控信息\\报警图片\\"
					+GetSaveTime()+"带子.jpg",
					&(IplImage)ImgsForBehaviorTest[0]);
				WriteToRecordLine(GetTime()+ "       "+"有人安全帽带子未系好");


			}

		}
		Sleep(5000);
		ImgsForBehaviorTest.clear();
	}

	BehaviorDetecting = FALSE;
	return 0;

}









//绞磨机检测
DWORD WINAPI MillDetectProc(LPVOID lpParameter)
{
	if (MillDetecting==TRUE)
	{
		return 0;
	}
	MillDetecting = TRUE;
	vector<Mat> ImgsForHighTest; 
	StatusBar.SetText("正在检测绞磨机工作", 3, 0);
	while (1)
	{

		if (MillMat.empty()==FALSE)
		{
			if (ImgsForMillTest.size()< 20)
			{
				ImgsForMillTest.push_back(MillMat.clone());
				Sleep(100);
				continue;
			}
		}
		else
		{
			Sleep(1000);
			continue;
		}


		if (MillDetecting == FALSE)
		{
			StatusBar.SetText("就绪", 3, 0);
			return 0;
		}



		if (ImgsForMillTest.size()==20)
		{
			vector<int> results;

			EnterCriticalSection(&DetectionCritical);
			try
			{
				results = DetectGrinderOperation(ImgsForMillTest);
			}
			catch (...)
			{
				//AfxMessageBox("绞磨机检测出错");
			}
			LeaveCriticalSection(&DetectionCritical);




			for (unsigned int i=0;i<results.size();i++)
			{
				switch (results[i])
				{
				case NO_5TURNS:
					{
						//手机报警系统
						static time_t LastTime = 0;
						static time_t PresentTime = 0;
						static time_t Span = 0;
						PresentTime = time(0);
						Span = PresentTime - LastTime;

						if (Span>SPAN)
						{
							LastTime = PresentTime;
							WriteToCommandLine("6");
							cvSaveImage("D:\\施工现场监控信息\\notificationPhoto\\"
								+GetSaveTime()+"绞磨机.jpg",
								&(IplImage)ImgsForMillTest[19]);
						}


						//显示所有检测信息
						InsertItemToGrid(MillGrid,GetTime(),
							"未缠够5圈",1);
						cvSaveImage("D:\\施工现场监控信息\\报警图片\\"
							+GetSaveTime()+"绞磨机未缠够绳子.jpg",
							&(IplImage)ImgsForMillTest[19]);
						WriteToRecordLine(GetTime()+ "       "+"未缠够5圈");

						break;
					}
				case REGUALR_TURNS :
					{
						InsertItemToGrid(MillGrid,GetTime(),
							"缠够5圈",1);
						break;
					}

				case STAND_ON_COIL:
					{

						//手机报警系统
						static time_t LastTime = 0;
						static time_t PresentTime = 0;
						static time_t Span = 0;
						PresentTime = time(0);
						Span = PresentTime - LastTime;

						if (Span>SPAN)
						{
							LastTime = PresentTime;
							WriteToCommandLine("9");
							cvSaveImage("D:\\施工现场监控信息\\notificationPhoto\\"
								+GetSaveTime()+"绞磨机.jpg",
								&(IplImage)ImgsForMillTest[19]);
						}


						//显示所有检测信息
						InsertItemToGrid(MillGrid,GetTime(),
							"有人踩在绳子上",1);
						cvSaveImage("D:\\施工现场监控信息\\报警图片\\"
							+GetSaveTime()+"绞磨机绳子.jpg",
							&(IplImage)ImgsForMillTest[19]);
						WriteToRecordLine(GetTime()+ "       "+"有人踩在绳子上");
						break;

					}


				case PEOPLE_STAND_WRONG:
					{

						//手机报警系统
						static time_t LastTime = 0;
						static time_t PresentTime = 0;
						static time_t Span = 0;
						PresentTime = time(0);
						Span = PresentTime - LastTime;

						if (Span>SPAN)
						{
							LastTime = PresentTime;
							WriteToCommandLine("10");
							cvSaveImage("D:\\施工现场监控信息\\notificationPhoto\\"
								+GetSaveTime()+"绞磨机.jpg",
								&(IplImage)ImgsForMillTest[19]);
						}


						//显示所有检测信息
						InsertItemToGrid(MillGrid,GetTime(),
							"绞磨机工作人员站位错误",1);
						cvSaveImage("D:\\施工现场监控信息\\报警图片\\"
							+GetSaveTime()+"绞磨机人员站位.jpg",
							&(IplImage)ImgsForMillTest[19]);
						WriteToRecordLine(GetTime()+ "       "+"人员站位错误");

						break;
					}
				}
			}
		}


		Sleep(6000);
		ImgsForMillTest.clear();
	}

	MillDetecting = FALSE;
	return 0;

}



