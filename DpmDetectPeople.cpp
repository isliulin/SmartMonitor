#include "stdafx.h"

 vector<Rect> detectAndDrawObjects( Mat& image, LatentSvmDetector& detector, const vector<Scalar>& colors, float overlapThreshold, int numThreads );
 void readmodelDirectory( const string& directoryName, vector<string>& filenames, bool addDirectoryName=true );


vector<Rect> DpmDetectPeople(Mat img,vector<Rect> found)
{	
	
	vector<Rect> Dpmfound;
	vector<Rect> newfound;
	string models_folder;
    float overlapThreshold = 0.2f;
    int numThreads = -1;

    models_folder = "peoplexml";

    vector<string> models_filenames;

    readmodelDirectory( models_folder, models_filenames );

    LatentSvmDetector detector( models_filenames );
    if( detector.empty() )
    {
        //////cout << "Models cann't be loaded" << endl;
        exit(-1);
    }

  /*  const vector<string>& classNames = detector.getClassNames();
    ////cout << "Loaded " << classNames.size() << " models:" << endl;
    for( size_t i = 0; i < classNames.size(); i++ )
    {
        ////cout << i << ") " << classNames[i] << "; ";
    }
    ////cout << endl;

    ////cout << "overlapThreshold = " << overlapThreshold << endl;
*/
    vector<Scalar> colors;
    generateColors( colors, detector.getClassNames().size() );

	for(int i=0;i<found.size();i++)
	{
		Rect roi=found[i];
	    Mat image1=img(roi);
		if(image1.data)
		{
		  newfound=detectAndDrawObjects( image1, detector, colors, overlapThreshold, numThreads);
		  for(int j=0;j<newfound.size();j++)
		  {
			newfound[j].x+=roi.x;
			newfound[j].y+=roi.y;
			Dpmfound.push_back(newfound[j]);
		  }
		}
	}
    
	return Dpmfound;
	//resize(image, image, Size(480,320), 0, 0, INTER_NEAREST);
}



vector<Rect> detectAndDrawObjects( Mat& image, LatentSvmDetector& detector, const vector<Scalar>& colors, float overlapThreshold, int numThreads )
{
	vector<Rect> found;
    vector<LatentSvmDetector::ObjectDetection> detections;

    TickMeter tm;
    tm.start();
    detector.detect( image, detections, overlapThreshold, numThreads);
    tm.stop();

    //////cout << "Detection time = " << tm.getTimeSec() << " sec" << endl;

    const vector<string> classNames = detector.getClassNames();
    CV_Assert( colors.size() == classNames.size() );

    for( size_t i = 0; i < detections.size(); i++ )
    {     
        const LatentSvmDetector::ObjectDetection& od = detections[i];
        rectangle( image, od.rect, colors[od.classID], 3 );
		found.push_back(od.rect);
    }
    // put text over the all rectangles
    for( size_t i = 0; i < detections.size(); i++ )
    {
        const LatentSvmDetector::ObjectDetection& od = detections[i];
        putText( image, classNames[od.classID], Point(od.rect.x+4,od.rect.y+13), FONT_HERSHEY_SIMPLEX, 0.55, colors[od.classID], 2 );
    }
	return found;
}
 void readmodelDirectory( const string& directoryName, vector<string>& filenames, bool addDirectoryName )
{
    filenames.clear();

    #ifdef WIN32
    struct _finddata_t s_file;
    string str = directoryName + "\\*.*";

    intptr_t h_file = _findfirst( str.c_str(), &s_file );
    if( h_file != static_cast<intptr_t>(-1.0) )
    {
        do
        {
            if( addDirectoryName )
                filenames.push_back(directoryName + "\\" + s_file.name);
            else
                filenames.push_back((string)s_file.name);
        }
        while( _findnext( h_file, &s_file ) == 0 );
    }
    _findclose( h_file );
#else
    DIR* dir = opendir( directoryName.c_str() );
    if( dir != NULL )
    {
        struct dirent* dent;
        while( (dent = readdir(dir)) != NULL )
        {
            if( addDirectoryName )
                filenames.push_back( directoryName + "/" + string(dent->d_name) );
            else
                filenames.push_back( string(dent->d_name) );
        }
    }
#endif

    sort( filenames.begin(), filenames.end() );
}

