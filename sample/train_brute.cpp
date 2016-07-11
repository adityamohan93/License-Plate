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
	char s[25];
	int ans,c;
	string s1;
	double err,min,br[37][25][25],cr[25][25];
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
			c++;
			/*namedWindow(s1, CV_WINDOW_AUTOSIZE*10);
			imshow(s1,img);
			waitKey(0); 
*/
			for(int k=0;k<img.rows;k++) 
			{
			    for (int l=0;l<img.cols;l++)
			      {
			      	 //    	printf("%c " ,img.at<uchar>(k,l)); 
				
			      	if(img.at<uchar>(k,l)!='\0')
				 ar[k][l]+=1;
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
	img=imread(argv[1]);
	for(int k=0;k<img.rows;k++) 
	{
	    for (int l=0;l<img.cols;l++)
	      {
	      	 //    	printf("%c " ,img.at<uchar>(k,l)); 
		
	      	if(img.at<uchar>(k,l)!='\0')
			cr[k][l]=1;
		else
			cr[k][l]=0;
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
	min=500000;
	for(int i=1;i<=36;i++)
	{
		err=0;
		for(int j=0;j<20;j++) 
			{
			    for (int k=0;k<20;k++)
				{
					err+=abs(br[i][j][k]-cr[j][k]);
				}
			}
		if(err<min)
		{
			min=err;
			ans=i;
		}
	}
	if(ans>=0 && ans<=9)
		printf("ans=%d\n",ans);		
 	else if(ans>=10 && ans<=36)
		printf("ans=%c\n",ans-10+'a');		
   return 0;
}
