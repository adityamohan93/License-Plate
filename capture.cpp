#include<cstdio>
#include <iostream>
#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"


int main( int argc, char** argv ) 
{
	cvNamedWindow( "DisplayVideo", CV_WINDOW_AUTOSIZE );
	CvCapture* capture = cvCreateFileCapture( argv[1] );
	IplImage* frame;
	int num=0;
	char s [20];
	while(1) {
	num++;
	frame = cvQueryFrame( capture );
	if( !frame ) break;
	cvShowImage( "DisplayVideo", frame );
	char c = cvWaitKey(33);
	if( c == 27 ) break;
	if (c == 'f')
	{
	sprintf(s,"frame%d.jpg",num);
	cvSaveImage(s,frame);

	}
	}
	cvReleaseCapture( &capture );
	cvDestroyWindow("DisplayVideo" );
}
