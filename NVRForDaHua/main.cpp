#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include "dhnetsdk.h"
#include "dhplay.h"
#include <winCon.h>
#include "opencv2/opencv.hpp"
using namespace std;
#define SWITCH 1
#define PLAYPORT 1 
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
void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	printf("设备断线.\n");
	return;
}

void CALLBACK AutoReConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	printf("自动重连成功.\n");
	return;
}
void CALLBACK RealDataCallBackEx(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LONG lParam, DWORD dwUser)
{
	if (dwDataType == 0)  //原始视频流送播放库    
	{
		PLAY_InputData(PLAYPORT, pBuffer, dwBufSize);
	}
}
void CALLBACK DecCBFun(LONG nPort, char * pBuf, LONG nSize, FRAME_INFO * pFrameInfo, void* pUserData, LONG nReserved2)
{
	//移走数据后，快速返回,即开始解码回调下一帧数据;不要在回调中运行长事务，否则阻塞解码下一帧数据  
	// pbuf里的数据是YUV I420格式的数据   

	if (pFrameInfo->nType == 3) //视频数据   
	{
		YUV420_To_IplImage_Opencv(pBuf,pFrameInfo->nWidth,pFrameInfo->nHeight);
		printf("video CallBack\n");
	}
	else if (pFrameInfo->nType == T_AUDIO16)
	{
		//cout<<"Audio CallBack"<<endl;  
	}
	else
	{
		printf("nType = %d\n", pFrameInfo->nType);
	}
	return;
}

void CALLBACK cbDownLoadPos(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser)
{
	//printf("cbDownLoadPos\n");
}
int CALLBACK fDownLoadDataCallBack(LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LDWORD dwUser)
{
	//printf("fDownLoadDataCallBack\n");
	if (dwDataType == 0)  //原始视频流送播放库    
	{
		PLAY_InputData(PLAYPORT, pBuffer, dwBufSize);
	}
	return 1;
}
int main(void)
{
	//功能一：实时显示预览
	//功能二：文件下载回放
	PLAY_OpenStream(PLAYPORT, 0, 0, 1024*1024);
	PLAY_SetDecCallBackEx(PLAYPORT, DecCBFun, NULL);
	PLAY_Play(PLAYPORT, NULL);
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
		//1.实时取流 已完成。
		//TODO速度待优化，目前有卡顿。
#if SWITCH
		lRealPlay = CLIENT_RealPlayEx(lLogin, 2, NULL, DH_RType_Realplay);
		if (lRealPlay != 0)
		{
			CLIENT_SetRealDataCallBackEx(lRealPlay, RealDataCallBackEx, 0, 0x0000001f);
		}
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
		lSearch = CLIENT_FindFile(lLogin, 0, 0, NULL, time_start, time_end, FALSE, 1000);
		int result = CLIENT_FindNextFile(lSearch, &info);
		LONG state = CLIENT_PlayBackByRecordFileEx(lLogin,&info,NULL,cbDownLoadPos,NULL,fDownLoadDataCallBack,NULL);
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

