#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include "dhnetsdk.h"
#include "dhplay.h"
#include <cstring>
#include <winCon.h>
#include "opencv2/opencv.hpp"
using namespace std;
#define SWITCH 1
#define PLAYPORT 1
typedef struct VideoData
{
	char* data;
	int width;
	int height;
}TVideoData; //视频数据结构体
list<VideoData> videolist;//list存储视频数据

//解码函数 将YUV420解码为IplImage
IplImage* YUV420_To_IplImage_Opencv(char* pYUV420, int width, int height)
{
	if (!pYUV420)
	{
		return NULL;
	}
	IplImage *yuvimage, *rgbimg, *yimg, *uimg, *vimg, *uuimg, *vvimg;
	int nWidth = width;
	int nHeight = height;
	rgbimg = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 3);
	yuvimage = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 3);
	yimg = cvCreateImageHeader(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 1);
	uimg = cvCreateImageHeader(cvSize(nWidth / 2, nHeight / 2), IPL_DEPTH_8U, 1);
	vimg = cvCreateImageHeader(cvSize(nWidth / 2, nHeight / 2), IPL_DEPTH_8U, 1);
	uuimg = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 1);
	vvimg = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 1);
	cvSetData(yimg, pYUV420, nWidth);
	cvSetData(uimg, pYUV420 + nWidth*nHeight, nWidth / 2);
	cvSetData(vimg, pYUV420 + long(nWidth*nHeight*1.25), nWidth / 2);
	cvResize(uimg, uuimg, CV_INTER_LINEAR);
	cvResize(vimg, vvimg, CV_INTER_LINEAR);
	cvMerge(yimg, uuimg, vvimg, NULL, yuvimage);
	cvCvtColor(yuvimage, rgbimg, CV_YCrCb2RGB);
	cvReleaseImage(&uuimg);
	cvReleaseImage(&vvimg);
	cvReleaseImageHeader(&yimg);
	cvReleaseImageHeader(&uimg);
	cvReleaseImageHeader(&vimg);
	cvReleaseImage(&yuvimage);
	if (!rgbimg)
	{
		return NULL;
	}
	CvSize sz;
	IplImage *desc;
	sz.width = rgbimg->width*0.5;
	sz.height = rgbimg->height*0.5;
	desc = cvCreateImage(sz, rgbimg->depth, rgbimg->nChannels);
	cvResize(rgbimg, desc, CV_INTER_CUBIC);
	cvShowImage("test", desc);
	cvWaitKey(1);
	cvReleaseImage(&desc);
	return rgbimg;
}
//设备短线回调函数
void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	printf("设备断线.\n");
	return;
}
//短线重练回调函数
void CALLBACK AutoReConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	printf("自动重连成功.\n");
	return;
}
//实时数据流回调函数 由于使用的NVR所以使用扩展回调函数
void CALLBACK RealDataCallBackEx(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LONG lParam, DWORD dwUser)
{
	if (dwDataType == 0)  //原始视频流送播放库    
	{
		PLAY_InputData(PLAYPORT, pBuffer, dwBufSize);
	}
}
//解码回调函数
void CALLBACK DecCBFun(LONG nPort, char * pBuf, LONG nSize, FRAME_INFO * pFrameInfo, void* pUserData, LONG nReserved2)
{
	// pbuf里的数据是YUV I420格式的数据   
	if (pFrameInfo->nType == 3) //视频数据   
	{
		//将回调获取的YUV420数据放入list列表中
		//这种方式可以保证所有数据不会丢失
		//若做实时显示,可以进行丢帧处理来降低卡顿。
		TVideoData data;
		data.data = (char*)malloc(sizeof(char)*nSize);
		memcpy(data.data,pBuf,nSize);
		data.height = pFrameInfo->nHeight;
		data.width = pFrameInfo->nWidth;
		videolist.push_back(data);
	}
	return;
}
//文件回放下载进度回调函数
void CALLBACK cbDownLoadPos(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser)
{
	//printf("cbDownLoadPos\n");
}
//文件回放数据回调函数
int CALLBACK fDownLoadDataCallBack(LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LDWORD dwUser)
{
	if (dwDataType == 0)  //原始视频流送播放库    
	{
		PLAY_InputData(PLAYPORT, pBuffer, dwBufSize);
	}
	return 1;
}
//数据处理函数
DWORD WINAPI DataDeal(LPVOID lpParameter)
{
	while (1)
	{
		while (videolist.size() == 0);
		TVideoData data = videolist.front();
		double Time = (double)cvGetTickCount();
		YUV420_To_IplImage_Opencv(data.data, data.width, data.height);
		Time = (double)cvGetTickCount() - Time;
		printf("run time = %gms\n", Time / (cvGetTickFrequency() * 1000));
		free(data.data);
		videolist.pop_front();
	}
	return 0;
}
int main(void)
{
	//功能一：实时显示预览
	//功能二：文件下载回放
	PLAY_OpenStream(PLAYPORT, 0, 0, 1024*900);
	PLAY_SetDecCallBackEx(PLAYPORT, DecCBFun, NULL);
	PLAY_Play(PLAYPORT, NULL);
	//以上代码为启用解码
	CLIENT_LogClose();
	NET_DEVICEINFO_Ex info_ex = { 0 };
	int err = 0;
	unsigned long lLogin = 0;
	LLONG lSearch = 0;
	LLONG lRealPlay = 0;
	CLIENT_Init(DisConnectFunc, 0);
	CLIENT_SetAutoReconnect(AutoReConnectFunc, 0);
	lLogin = CLIENT_LoginEx2("192.168.0.101", 37777, "admin", "kz123456", EM_LOGIN_SPEC_CAP_TCP, NULL, &info_ex, &err);
	if (lLogin == 0)
	{
		printf("login error!\r\n");
	}
	else
	{

		printf("login success!\r\n");
		
#if SWITCH //SWITCH 宏定义，可通过修改该开关切换实时数据显示和文件回调显示
		//1.实时取流。
		lRealPlay = CLIENT_RealPlayEx(lLogin, 2, NULL, DH_RType_Realplay);
		//CLIENT_RealPlayEx 第二个参数为NVR 播放通道 此处为单通道显示可改为多通道预览
		if (lRealPlay != 0)
		{
			CLIENT_SetRealDataCallBackEx(lRealPlay, RealDataCallBackEx, 0, 0x0000001f);
		}
		HANDLE hThread1 = CreateThread(NULL, 0, DataDeal, NULL, 0, NULL);
		
#endif
#if !SWITCH
		//2.文件取流 回放
		NET_RECORDFILE_INFO info = { 0 };
		LPNET_TIME time_start = (LPNET_TIME)malloc(sizeof(LPNET_TIME));
		LPNET_TIME time_end = (LPNET_TIME)malloc(sizeof(LPNET_TIME));
		time_start->dwYear = 2017;
		time_start->dwMonth = 11;
		time_start->dwDay = 10;
		time_start->dwHour = 9;
		time_start->dwMinute = 10;
		time_start->dwSecond = 0;
		time_end->dwYear = 2017;
		time_end->dwMonth = 11;
		time_end->dwDay = 10;
		time_end->dwHour = 9;
		time_end->dwMinute = 15;
		time_end->dwSecond = 0;
		//设置回放时间段
		lSearch = CLIENT_FindFile(lLogin, 0, 0, NULL, time_start, time_end, FALSE, 1000);
		//CLIENT_FindFile 第二个参数为通道号，文件回放只允许开启一个通道进行文件回放 第三个参数为文件类型
		int result = CLIENT_FindNextFile(lSearch, &info);
		//查询到符合参数的文件名 存储于info结构体中
		LONG state = CLIENT_PlayBackByRecordFileEx(lLogin,&info,NULL,cbDownLoadPos,NULL,fDownLoadDataCallBack,NULL);
		HANDLE hThread1 = CreateThread(NULL, 0, DataDeal, NULL, 0, NULL);
#endif
	}
	getchar();
	//释放网络库  
	CLIENT_StopRealPlay(lRealPlay);
	CLIENT_Logout(lLogin);
	CLIENT_Cleanup();
	//关闭播放通道，释放资源  
	PLAY_Stop(PLAYPORT);
	PLAY_CloseStream(PLAYPORT);
	return 0;
}





