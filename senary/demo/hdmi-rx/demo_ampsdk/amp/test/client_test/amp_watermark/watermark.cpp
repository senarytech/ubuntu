#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <algorithm>
#include "watermark.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

void addwatermark(unsigned char *pbuf_in, unsigned char *pbuf_out, int size)
{
	/*
	int T1_value = 40;
	cv::Mat img_org(1920, 1080, CV_8UC3,*pbuf_inout);
	cv::Mat img_dst;
	Canny(img_org, img_dst, T1_value, 2 * T1_value, 3, false);
	pbuf_inout = img_dst.data;
	*/
	//cv::Mat img_org(1920, 1080, CV_8UC3, pbuf_inout);
	cv::Mat img_org(1080*3/2, 1920, CV_8UC1, pbuf_in);
	cv::putText(img_org,"Hello,Senary ",Point(200,500),FONT_HERSHEY_SIMPLEX,10,Scalar(255,255,255),4,8);
	//memcpy(pbuf_out,img_org.data,1920*1080*2);
	//cv::putText(img_org, "Nice to see you!", (200, 100), cv::FONT_HERSHEY_COMPLEX, 2.0, (100, 200, 200), 5)


}

int get_bmp_buf(unsigned char *pbuf, int size)
{
	FILE *fp = NULL;
	fp = fopen("/root/1.bmp","rb");
	if(fp)
	{
		fseek(fp,54,SEEK_SET);
		fread(pbuf,1,1920*1080*2,fp);
		fclose(fp);
	}
	else{

		return -1;
	}


	return 0;

}

