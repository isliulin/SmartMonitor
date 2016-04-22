
#include "stdafx.h"

vector<Rect> HogDetectPeople(Mat img)
{	
	
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

        fflush(stdout);
        vector<Rect> found, found_filtered;
        double t = (double)getTickCount();
        // run the detector with default parameters. to get a higher hit-rate
        // (and more false alarms, respectively), decrease the hitThreshold and
        // groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
        hog.detectMultiScale(img, found, 0, Size(8,8), Size(32,32), 1.05, 2);
        t = (double)getTickCount() - t;
        printf("tdetection time = %gms\n", t*1000./cv::getTickFrequency());
        size_t i, j;
        for( i = 0; i < found.size(); i++ )
        {
            Rect r = found[i];
            for( j = 0; j < found.size(); j++ )
                if( j != i && (r & found[j]) == r)
                    break;
            if( j == found.size() )
                found_filtered.push_back(r);
        }
        for( i = 0; i < found_filtered.size(); i++ )
        {
            Rect r = found_filtered[i];
            // the HOG detector returns slightly larger rectangles than the real objects.
            // so we slightly shrink the rectangles to get a nicer output.
		    r.x += cvRound(r.width*0.1);
            r.width = cvRound(r.width*0.8);
            r.y += cvRound(r.height*0.07);
            r.height = cvRound(r.height*0.8);
			if(r.x+r.width>img.cols-1)
			{	r.x=img.cols-1-r.width;}
			if(r.x<0)
				r.x=0;
			if(r.y+r.height>img.rows-1)
			    r.y=img.rows-1-r.height;
			if(r.y<0)
			    r.y=0;
			found_filtered[i].x=r.x;
			found_filtered[i].y=r.y;
			found_filtered[i].width=r.width;
			found_filtered[i].height=r.height;

           // rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
        }
		return found_filtered;
}