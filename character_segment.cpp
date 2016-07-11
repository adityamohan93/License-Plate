#include "character_segment.h"
#include<cstdio>
Segment::Segment(){}
Segment::Segment(Mat i, Rect p){
    img=i;
    pos=p;
}
class Plate_Detect{
    public:
        Plate_Detect();
        bool flag;
        Plate return_segmented_characters(Mat input);
        void save_image(Mat, string);
	void show_image(Mat,string);
    private:
        Plate segment(Mat input);
        bool validate(RotatedRect mr);
	Mat histeq(Mat in);
};
character_segment::character_segment(){
    DEBUG=true;
    saveSegments=false;
    charSize=20;
}
Mat character_segment::preprocessChar(Mat in){
    int h=in.rows;
    int w=in.cols;
    Mat transformMat=Mat::eye(2,3,CV_32F);
    int m=max(w,h);
    transformMat.at<float>(0,2)=m/2 - w/2;
    transformMat.at<float>(1,2)=m/2 - h/2;

    Mat warpImage(m,m, in.type());
    warpAffine(in, warpImage, transformMat, warpImage.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0) );

    Mat out;
    resize(warpImage, out, Size(charSize, charSize) ); 

    return out;
}
bool character_segment::validate(Mat r){
    float aspect=45.0f/77.0f;
    float charAspect= (float)r.cols/(float)r.rows;
    float error=0.50;
    float minHeight=7;
    //float minHeight=15;
    float maxHeight=28;
    float minAspect=0.2;
    float maxAspect=1;//aspect+aspect*error;
    float area=countNonZero(r);
    float bbArea=r.cols*r.rows;
    float percPixels=area/bbArea;
//cout<<"min height "<<minHeight<<"max height "<<maxHeight << " height is " <<r.rows<<" min aspect "<<minAspect<<" max aspect "<<maxAspect<<" aspect is "<< charAspect<<endl;
   if(percPixels < 0.95 && charAspect > minAspect && charAspect < maxAspect && r.rows >= minHeight && r.rows < maxHeight)
        return true;
    else
        return false;

}
void character_segment::train()
{
	Mat img;
	char s[25];
	int c;
	string s1;
	for(int i=0;i<=36;i++)
	{
		c=0;
		double ar[25][25]={};
		for(int j=1;j<=16;j++)
		{
			if(i>=0 && i<=9)
				sprintf(s,"sample/%d_%d.jpg",i,j);
			else
				sprintf(s,"sample/%c_%d.jpg",i+'a'-10,j);
			s1=(string) s;
			img=imread(s1);
			if(img.empty())
				continue;
			cvtColor(img,img, CV_RGB2GRAY );
			threshold(img,img,127,255,THRESH_BINARY);
			c++;
			/*namedWindow(s1, CV_WINDOW_AUTOSIZE*10);
			imshow(s1,img);
			waitKey(0); 
			*/
			for(int k=0;k<img.rows;k++) 
			{
				for (int l=0;l<img.cols;l++)
				{
					if(img.at<uchar>(k,l))
					ar[k][l]++;
				}
			}
		}
		for(int j=0;j<20;j++) 
		{
			for (int k=0;k<20;k++)
			{
				if(c)
					br[i][j][k]=ar[j][k]/(double)c;
				else
					br[i][j][k]=0;
			}
		}
	}
	/*for(int i=1;i<=4;i++)
	{
		printf("i=%d-------------------------------------------------\n",i);
		for(int j=0;j<20;j++,printf("\n")) 
		{
			for (int k=0;k<20;k++)
			{
				printf("%lf ",br[i][j][k]);
			}
		}
	}*/
}
char character_segment::classify(Mat img)
{
	char c,s[25];
	int ans,z,x;
	string s1;
	int err,min_e,max,cr[25][25];
	threshold(img,img,127,255,THRESH_BINARY);c++;
	for(int k=0;k<img.rows;k++)
	{
		for (int l=0;l<img.cols;l++)
		{
			if(img.at<uchar>(k,l))
				cr[k][l]=1;
			else
				cr[k][l]=0;
		}
	}
	min_e=500000000;
	max=-1;
	for(int i=1;i<=36;i++)
	{
		z=0;
		err=0;
		for(int j=0;j<20;j++)
		{
			for (int k=0;k<20;k++)
			{
				x=br[i][j][k]>.34 ? 1:0;
				if(x &&cr[j][k])
				{
					z++;
				}
				else if(x || cr[j][k])
					err++;
			}
		}
		if(i<=9)
			c=i+'0';
		else
			c=i-10+'A';
		z=z-err;
		if(z>max)
		{
			max=z;
			min_e=err;
			ans=i;
		}
		else if(z==max && err<min_e)
		{
			max=z;
			min_e=err;
			ans=i;
		}
	}
	if(ans>=0 && ans<=9)
		c=ans+'0';
	else if(ans>=10 && ans<=36)
		c=ans-10+'A';
	else
		c='\0';
	return c;
}
vector<Segment> character_segment::segment(Plate plate){
	Plate_Detect detect;
	Mat input=plate.plateImg;
	vector<Segment> output;
	Mat img_threshold;
	threshold(input, img_threshold,60, 255, CV_THRESH_BINARY_INV);
	detect.show_image(img_threshold,"inside segment character_segment.cpp"); 
	Mat img_contours;
	img_threshold.copyTo(img_contours);
	vector< vector< Point> > contours;
	findContours(img_contours,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	cv::Mat result;
	img_threshold.copyTo(result);
	cvtColor(result, result, CV_GRAY2RGB);
	cv::drawContours(result,contours,-1,
	cv::Scalar(255,0,0), // in blue
	1);
	detect.show_image(result,"draw contours ");
	vector<vector<Point> >::iterator itc= contours.begin();
	while (itc!=contours.end()) {    
		Rect mr= boundingRect(Mat(*itc));
		rectangle(result, mr, Scalar(0,255,0));
		Mat crp(img_threshold, mr);
		if(validate(crp)){
			output.push_back(Segment(crp, mr));
			//detect.show_image(crp,"verified characters");
			rectangle(result, mr, Scalar(0,125,255));
		}
	++itc;
	}
	detect.show_image(result,"segmented characters");
	return output;
}


string character_segment::return_segmented_characters(Plate *input){
	char s1[15];
	string s;
	vector<Segment> segments=segment(*input);
	train();
	for(int i=0; i<segments.size(); i++){
		Mat ch=preprocessChar(segments[i].img);
		sprintf(s1,"image_%d.jpg",i);
		s=(string) s1;
		imwrite(s,ch);
		Plate_Detect detect;
		//detect.show_image(ch,"character");
		char character=classify(ch);
		//cout<<character<<endl;
		input->chars.push_back(character);
		input->charsPos.push_back(segments[i].pos);
	}
	return "-";
}
