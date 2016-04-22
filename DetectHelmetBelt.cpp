#include "stdafx.h"


vector<Rect>  DetectHelmetBelt(vector<Rect>face,Mat image)
{

	RNG rng(12345);
	Mat imagecopy=image.clone();
	vector<Rect>chin_rect;
	vector<Rect>optionalchin_rect;
	vector<Rect>nonhelmetbelt_rect;

	Mat hsv;
	vector<Mat> channel;  
	Mat h;
	Mat s;
	Mat v;
	cvtColor(imagecopy, hsv, CV_BGR2HSV);  
	////imshow("hsv",hsv);
	split(hsv, channel);   
	h=channel[0];
	s=channel[1];
	v=channel[2];
	Mat black =v<60;
	medianBlur(black,black,3);
	////imshow("black",black);
	//int face_count=0;

	if (face.size())//下巴区域获取
	{
		for (int i=0;i<face.size();i++)
		{
			if((face[i].width/face[i].height<2.2)&(face[i].height/face[i].width<2.2))
			{

				face[i].y=face[i].y+face[i].height*0.8;
				face[i].x=face[i].x+face[i].width*0.2;
				face[i].width=face[i].width*0.6;
				face[i].height=face[i].height*0.4;
				if ((face[i].y+face[i].height)<imagecopy.rows)//图像越界检查
				{
					//optionalface_rect[face_count]=face_rect[i];
					optionalchin_rect.push_back(face[i]);
					//Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
					//rectangle( image, optionalchin_rect[face_count].tl(), optionalchin_rect[face_count].br(), color, 2, 8, 0 );//其中boundRectneed[i]包含了一个轮廓的矩形框
					//face_count++;
				}
			}
		}
	}
	////imshow("daizi",image);
	if (optionalchin_rect.size())//带子检测
	{
		for (int i=0;i<optionalchin_rect.size();i++)
		{
			int chincount=0;
			Rect roi=optionalchin_rect[i];
			Mat blackroi=black(roi);
			////imshow("blackroi",blackroi);
			for (int x=0;x<blackroi.rows;x++)
				for(int y=0;y<blackroi.cols;y++)
				{

					if (blackroi.at<uchar>(x, y)==255)
					{
						chincount++;
					}
				}
				if (chincount<2)
				{
					nonhelmetbelt_rect.push_back(optionalchin_rect[i]);
					printf("带子没系好");
				}

		}
	}

	return nonhelmetbelt_rect;
}