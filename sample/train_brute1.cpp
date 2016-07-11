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
#include<string>
#include<cstring>
#include<cstdio>
using namespace std;
using namespace cv;
int main(int argc,char *argv[])
{
	Mat img;
	char c,s[25];
	int ans,z;
	string s1;
	int err,min_e,max,br[37][25][25],cr[25][25];
	for(int i=0;i<=36;i++)
	{
		c=0;
		double ar[25][25]={};
		for(int j=1;j<=10;j++)
		{
			if(i>=0 && i<=9)
				sprintf(s,"%d_%d.jpg",i,j);
			else
				sprintf(s,"%c_%d.jpg",i+'a'-10,j);
			s1=(string) s;
			img=imread(s1);
			if(img.empty())
				continue;
			cvtColor(img,img, CV_RGB2GRAY );
			threshold(img,img,127,255,THRESH_BINARY);c++;
			/*namedWindow(s1, CV_WINDOW_AUTOSIZE*10);
			imshow(s1,img);
			waitKey(0); 
*/
			for(int k=0;k<img.rows;k++) 
			{
			    for (int l=0;l<img.cols;l++)
			      {
			      	 //    	printf("%c " ,img.at<uchar>(k,l)); 
				
			      	if(img.at<uchar>(k,l))
				 	br[i][k][l]=1;
				 else
				 	br[i][k][l]=0;
			      }
			}
		}
		/*
		for(int j=0;j<20;j++) 
		{
		    for (int k=0;k<20;k++)
			{
				if(c)
					br[i][j][k]=ar[j][k]/(double)c;
				else
					br[i][j][k]=0;
			}
		}*/
	}
	for(int i=1;i<=7;i++)
	{
		printf("i=%d-------------------------------------------------\n",i);
		for(int j=0;j<20;j++,printf("\n")) 
			{
			    for (int k=0;k<20;k++)
				{
					printf("%d",br[i][j][k]);
				}
			}
	}

	img=imread(argv[1]);
	cvtColor(img,img, CV_RGB2GRAY );
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
					if(br[i][j][k] &&cr[j][k])
					{
						z++;
					}
					else if(br[i][j][k] || cr[j][k])
						err++;
				}
			}
		if(i<=9)
			c=i+'0';
		else
			c=i-10+'A';
		//cout<<"i= "<<c<<"z= "<<z<<"  error= "<<err<<endl;
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
   	cout<<"ans= "<<c<<endl;
   return 0;
}
