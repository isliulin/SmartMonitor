#pragma once
#include "CommonHeader.h"


//高空检测
//高空检测设备信息
extern long HighLoginID;
extern char* HighIP;
extern unsigned short  HighPort;
extern char* HighUser;
extern char* HighPass;
extern long HighPlayhandle;


extern CStatic HighPic;//高空检测视频播放窗口
extern Mat HighMat;//实时视频帧
extern vector<CvPoint> PointArray;//四边形定位坐标数组
extern vector<Mat> ImgsForHighTest;//高空检测受检测的图片序列
extern BOOL HighWorkDetecting;//高空检测状态
extern CListCtrl HighGrid;//高空检测检测结果显示


//高空检测检测线程
DWORD WINAPI HighDetectProc(LPVOID lpParameter);






//施工现场检测
//施工检测设备信息
extern long BehaviorLoginID;
extern char* BehaviorIP;
extern unsigned short  BehaviorPort;
extern char* BehaviorUser;
extern char* BehaviorPass;
extern long BehaviorPlayhandle;


extern CStatic BehaviorPic;//行为检测视频播放窗口
extern Mat BehaviorMat;//行为检测实时视频帧
extern vector<Mat> ImgsForBehaviorTest;//行为检测受检测图片序列
extern BOOL BehaviorDetecting;//行为检测窗口检测状态
extern CListCtrl BehaviorGrid;//行为检测结果输出窗口

//行为检测线程
DWORD WINAPI BeheviorDetectProc(LPVOID lpParameter);





//绞磨机工作视频播放
//绞磨机检测设备信息
extern long MillLoginID;
extern char* MillIP;
extern unsigned short  MillPort;
extern char* MillUser;
extern char* MillPass;
extern long MillPlayhandle;


extern CStatic MillPic;//绞磨机视频播放窗口
extern Mat MillMat;//绞磨机检测实时视频帧
extern vector<Mat> ImgsForMillTest;//绞磨机检测受检视频帧序列集
extern BOOL MillDetecting;//绞磨机检测状态
extern CListCtrl MillGrid;//绞磨机检测结果输出窗口


//绞磨机作业检测线程
DWORD WINAPI MillDetectProc(LPVOID lpParameter);




//拓展窗口视频播放
//拓展检测设备信息
extern long MoreLoginID;
extern char* MoreIP;
extern unsigned short  MorePort;
extern char* MoreUser;
extern char* MorePass;
extern long MorePlayhandle;


extern Mat MoreMat;//拓展窗口实时视频帧
extern CStatic MorePic;//拓展视频播放窗口




//使用临界区保持监测线程的独占性
extern CRITICAL_SECTION DetectionCritical;


//系统状态栏，显示系统运行状态信息
extern CStatusBarCtrl StatusBar;







