//********************************************************************
//*  FaceDetection.h
//*  Copyright (c) 2014 CSIS Research Center. All rights reserved.
//* 
//*  version: 1.0
//*  author:  Zhu DaYong 
//*  Created: 2014.03.17
//*  mailto:  zdytaurus@gmail.com
//*********************************************************************/
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <Windows.h>
#include <direct.h>
#include "opencv\cv.h"
#include "stdafx.h"


map<string,int> imgtxt(string path);
int GetSubPath(string& folder, vector<string>& subFolders);
int GetSubFolders(string& folder, vector<string>& subFolders ,map<string,int>&sub_folder_name);
void GetSubfolder_picname(FILE *fp,string dir_path,int subi);
void GetSubfolder_picname_non_lable(FILE *fp,string dir_path);

void prepareORL(string path);
void replaceImg(string path);

Mat Process_face(Mat origin_img);
void detectObjectsCustom(const Mat &img, CascadeClassifier &cascade, vector<Rect> &objects, int scaledWidth, int flags, Size minFeatureSize, float searchScaleFactor, int minNeighbors);
void detectLargestObject(const Mat &img, CascadeClassifier &cascade, Rect &largestObject, int scaledWidth);

void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels,map<string,int>lbptest, char separator =';');
Mat labelchoice(int lab_num,string path,map<string,int>name,int num_name_use);

void SSR(IplImage* src,int sigma,int scale);
Mat retinex(Mat src);

void TrainFaceRecognizer();
int GetMin(int a,int b,int c);
vector<string> RecognizeFace(Mat src);
void TestFindFace();
vector<String> GetAttendance(vector<Mat> vecImg,int ConfThreshold);