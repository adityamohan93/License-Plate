#include "Plate.h"

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
Plate_Detect::Plate_Detect(){
    flag=false;
}
void Plate_Detect::show_image(Mat img, string window)
{
	namedWindow(window, CV_WINDOW_AUTOSIZE*10);
	imshow(window,img);
	waitKey(0); 
}
void Plate_Detect::save_image(Mat img, string fname)
{
	/*
	char s1[25];
	string fname;
	sprintf(s1,"img1_%d.jpg",i);
	fname=(string) s1;
	*/
	imwrite(fname,img);
}
bool Plate_Detect::validate(RotatedRect mr){

   // float error=0.40;
    float error=0.45;
    float aspect=4.7272;
    int min= 12*aspect*12;
    //int min= 15*aspect*15;
    int max= 125*aspect*125; 
    float rmin= aspect-aspect*error;
    float rmax= aspect+aspect*error;

    int area= mr.size.height * mr.size.width;
    float r= (float)mr.size.width / (float)mr.size.height;
    if(r<1)
        r= (float)mr.size.height / (float)mr.size.width;

   //cout<<"min area "<<min<<"max area "<<max << "area is " <<area<<"min aspect "<<rmin<<"max aspect"<<rmax<<"aspect is"<< r<<endl; 
   if(( area < min || area > max ) || ( r < rmin || r > rmax )){
        return false;
    }else{
        return true;
    }

}

Mat Plate_Detect::histeq(Mat in)
{
    Mat out(in.size(), in.type());
    if(in.channels()==3){
        Mat hsv;
        vector<Mat> hsvSplit;
        cvtColor(in, hsv, CV_BGR2HSV);
        split(hsv, hsvSplit);
        equalizeHist(hsvSplit[2], hsvSplit[2]);
        merge(hsvSplit, hsv);
        cvtColor(hsv, out, CV_HSV2BGR);
    }else if(in.channels()==1){
        equalizeHist(in, out);
    }

    return out;

}
Plate Plate_Detect::segment(Mat input){
    vector<Plate> output;

    Mat img_gray;
    cvtColor(input, img_gray, CV_BGR2GRAY);
    blur(img_gray, img_gray, Size(5,5));    
    if(flag)
    	show_image(img_gray,"Gray Scale Image");
   
 Mat img_sobel;
    Sobel(img_gray, img_sobel, CV_8U, 1, 0, 3, 1, 0, BORDER_DEFAULT);
    if(flag)
    	show_image(img_sobel,"Sobel filter(vertical edges)");
    
   Mat img_threshold;
    threshold(img_sobel, img_threshold, 0, 255, CV_THRESH_OTSU+CV_THRESH_BINARY);
    if(flag)
    	show_image(img_threshold,"Threshold");
  
   Mat element = getStructuringElement(MORPH_RECT, Size(17, 3) );
    morphologyEx(img_threshold, img_threshold, CV_MOP_CLOSE, element);
   if(flag)
   	show_image(img_threshold,"Morphological");


    vector< vector< Point> > contours;
    findContours(img_threshold,
            contours, 
            CV_RETR_EXTERNAL, 
            CV_CHAIN_APPROX_NONE); 

    vector<vector<Point> >::iterator itc= contours.begin();
    vector<RotatedRect> rects;

    while (itc!=contours.end()) {
        RotatedRect mr= minAreaRect(Mat(*itc));
        if( !validate(mr)){
            itc= contours.erase(itc);
        }else{
            ++itc;
            rects.push_back(mr);
        }
    }

    cv::Mat result;
    input.copyTo(result);
    cv::drawContours(result,contours,-1,cv::Scalar(255,0,0),3); 
    show_image(result,"Plates Contours)");
vector<Point> pointsInterest;
        pointsInterest=(vector<Point> )contours[0];

        RotatedRect minRect = minAreaRect(pointsInterest);

        if(validate(minRect)){
            Point2f rect_points[4]; minRect.points( rect_points );
            for( int j = 0; j < 4; j++ )
                line( result, rect_points[j], rect_points[(j+1)%4], Scalar(0,0,255), 1, 8 );    

            float r= (float)minRect.size.width / (float)minRect.size.height;
            float angle=minRect.angle;    
            float ar= minRect.size.height;
            float br= minRect.size.width;
            if(r<1)
                angle=90+angle;
            Mat rotmat= getRotationMatrix2D(minRect.center, angle,1);
	    Mat img_rotated;
            warpAffine(input, img_rotated, rotmat, input.size(), CV_INTER_CUBIC);
	    Size rect_size=minRect.size;
            if(r < 1)
                swap(rect_size.width, rect_size.height);
            Mat img_crop;
            getRectSubPix(img_rotated, rect_size, minRect.center, img_crop);
            
            Mat resultResized;
            resultResized.create(33,144, CV_8UC3);
            resize(img_crop, resultResized, resultResized.size(), 0, 0, INTER_CUBIC);
            if(flag) 
		show_image(img_crop,"Cropped Image");
            Mat grayResult;
            cvtColor(resultResized, grayResult, CV_BGR2GRAY);
            cout<<"br = "<<br<<endl; 
            if(br>80)
            {
		    blur(grayResult, grayResult, Size(3,3));
		    grayResult=histeq(grayResult);
            }
            if(flag) 
		show_image(grayResult,"Cropped Image with Histogram Equ.");
	    output.push_back(Plate(grayResult,minRect.boundingRect()));
        }

   return output[0];
}

Plate Plate_Detect::return_possible_plates(Mat input){
    Plate tmp=segment(input);
    return tmp;
}
