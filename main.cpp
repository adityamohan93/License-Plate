/*
	two letters
	two digits
	0,1 or 2 letters
	four digits
*/

#include <iostream>
#include <cstdio>
#include "Plate.h"
#include "character_segment.h"
using namespace std;
using namespace cv;

class Plate_Detect{
    public:
        Plate_Detect();
        bool flag;
        Plate return_possible_plates(Mat input);
        void save_image(Mat, string);
	void show_image(Mat,string);
    private:
        Plate segment(Mat input);
        bool validate(RotatedRect mr);
	Mat histeq(Mat in);
};
int capture(char video_name[])
{
	cvNamedWindow( "DisplayVideo", CV_WINDOW_AUTOSIZE );
	CvCapture* capture = cvCreateFileCapture( video_name );
	IplImage* frame;
	int num=0;
	char s [20];
	while(1)
	{
		num++;
		frame = cvQueryFrame( capture );
		if( !frame )
			break;
		cvShowImage( "DisplayVideo", frame );
		char c = cvWaitKey(33);
		if( c == 27 ) break;
		if (c == 'f')
		{
			sprintf(s,"frame%d.jpg",num);
			cvSaveImage(s,frame);
			cvReleaseCapture( &capture );
			cvDestroyWindow("DisplayVideo" );
			return num;
		}
	}
	cvReleaseCapture( &capture );
	cvDestroyWindow("DisplayVideo" );
}
int main ( int argc, char** argv )
{
	Mat input_image;
	/*char filename[20];
	int num = capture(argv[1]);
	sprintf(filename,"frame%d.jpg",num);

	input_image=imread(filename,1);	
	*/
	char *filename;
	filename= argv[1];
	input_image=imread(filename,1);	
	Plate_Detect Plate_Detect; 
	Plate_Detect.flag=true;
	Plate_Detect.show_image(input_image, "Original Image");   
	Plate plate= Plate_Detect.return_possible_plates( input_image ); 
	character_segment character_segment; 
	character_segment.filename=filename;
	character_segment.saveSegments=true;
	character_segment.DEBUG=false;
	character_segment.return_segmented_characters(&plate);
	string licensePlate=plate.str();
        cout << "License plate number: "<< licensePlate << "\n";
        rectangle(input_image, plate.position, Scalar(0,0,200));
        putText(input_image, licensePlate, Point(plate.position.x, plate.position.y), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,200),2);
        Plate_Detect.show_image(input_image,"Plate Detected");
        
	return 0;
}

