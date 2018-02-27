#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sstream>
#include "opencv2/imgproc/imgproc.hpp"
#include <fstream>
#include <iomanip>
#include <math.h>
#include <stdbool.h>
using namespace cv;
using namespace std;

struct boxes
{
	float x,y,w,h,s;

}typedef box;

template<typename T>
string tranform2str(T i)
{
	stringstream ss;
	ss << i;
	string st;
	ss >> st;
	return st;
}

float IOUcalc(box b1, box b2)
{
	float ai = (float)(b1.w + 1)*(b1.h + 1);
	float aj = (float)(b2.w + 1)*(b2.h + 1);
	float x_inter, x2_inter, y_inter, y2_inter;

	x_inter = max(b1.x,b1.x);
	y_inter = max(b1.y,b2.y);
	
	x2_inter = min((b1.x + b1.w),(b2.x + b2.w));
	y2_inter = min((b1.y + b1.h),(b2.y + b2.h));
	
	float w = (float)max((float)0, x2_inter - x_inter);  
	float h = (float)max((float)0, y2_inter - y_inter);  
	
	float inter = ((w*h)/(ai + aj - w*h));
	return inter;
}


void nms_best(vector<box> b, int count, vector<bool> &res, float theta)
{

    for(int i=0; i<count; i++)
    {
    	res[i] = true;
    }

    for(int i=0; i<count; i++)
    {
    	 
    	for(int j=0; j<count; j++)
    	{

    		if(b[i].s > b[j].s)
	 		{
	 		
				auto iou = IOUcalc(b[i], b[j]);
				printf("box %d and box %d IOU is %.3f\n", i, j, iou);
				
				if(iou > theta)
	 			{ 
	 				res[j] = false; 
	 			}

			}
    			
    	}
    	
    }
    
}

box rnd_box(int ref_x, int ref_y)
{
	box b;
	b.x =ref_x + (rand()%100) - 50 ;
	b.y =ref_y + (rand()%150) - 75;
	b.w = rand()%180+30;
	b.h = b.w*2;
	b.s = (float)rand() / (float)RAND_MAX; 
	return b;
}

int main()
{
	string text;
	int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
	double fontScale = 1;
	int thickness = 2;
	float theta = 0.15;   // if two box IOU smaller than _theta_  ,it will be keep high score box.

	int baseline=0;
	Size textSize = getTextSize(text, fontFace,
                            fontScale, thickness, &baseline);
	baseline += thickness;


	srand (time(NULL));
	Mat temp(960,1080, CV_8UC3, Scalar(255, 255, 255));

	int count=4;
	vector<box> vecb;

	int ref_x = rand()%(temp.cols/2 );
	int ref_y = rand()%(temp.rows/2) ;

	vector<bool> res1;
	vector<bool> res2;

	for(int i = 0; i<count; i++)
	{
		box b_i = rnd_box(ref_x ,ref_y );
		vecb.push_back(b_i);
		res1.push_back(false);
		res2.push_back(false);
	}
		
	nms_best(vecb,count,res2, theta);
	
	Point textOrg((temp.cols - textSize.width)/2,
              (temp.rows + textSize.height)/2);


	for(int i =0; i<count ; i++)
	{
		string text = tranform2str(vecb[i].s);
		string ind = tranform2str(i);
		text = ind +"_"+ text;       // show num is meaning:
		Size textSize = getTextSize(text, fontFace,fontScale, thickness, &baseline);
		if(res2[i])
		{
			rectangle(temp,Point(vecb[i].x, vecb[i].y),Point(vecb[i].x + vecb[i].w, vecb[i].y + vecb[i].h),Scalar(196, 64, 16),2,8,0);
			putText(temp, text,  Point(vecb[i].x, vecb[i].y), fontFace, fontScale,Scalar(255,0,0), thickness, 8);

		}
		else
		{
			rectangle(temp,Point(vecb[i].x, vecb[i].y),Point(vecb[i].x + vecb[i].w, vecb[i].y + vecb[i].h),Scalar(0,0,255),2,8,0);
			putText(temp, text,  Point(vecb[i].x, vecb[i].y), fontFace, fontScale,Scalar(0,0,255), thickness, 8);
		}
		
	}

	imshow("nms_bool",temp);
	waitKey(0);
	return 0;
}