#pragma once

#include "CommonHeader.h"

//将Mat绘制到Picture控件上
void DrawToScreen(CStatic& Picture,Mat img);


//获取日期
CString GetDate();


//获取时间
CString GetTime(void);



//获取存储时间
CString GetSaveTime();


//将识别结果写到本地文本文件中
void WriteToRecordLine(CString info);


//将识别结果写入手机报警系统中
BOOL WriteToCommandLine(CString info);


//向Grid控件制定位置中插入字符串
void InsertItemToGrid(CListCtrl& m_grid,CString Time,
	CString Results,int columns);


//在IplImage*所指图片上绘制四边形
void DrawSelectedArea(IplImage *image,vector<CvPoint> PointArray);



//清空手机报警系统的历史信息
void ClearPhoneFile();


//删除文件夹，重新建立文件夹=清空现有文件夹
BOOL DeleteDirectory(char* strDirName);


