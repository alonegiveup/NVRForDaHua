#include <Windows.h>
#include "dhnetsdk.h"
#include "dhplay.h"
#include "opencv2/opencv.hpp"
using namespace std;
#define SWITCH 1
#define PLAYPORT 1
extern "C"
{
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
#include "libavutil/channel_layout.h"
#include "libavutil/common.h"
#include "libavutil/imgutils.h"
#include "libavutil/mathematics.h"
#include "libavutil/samplefmt.h"
#include "libswscale/swscale.h" 
}
typedef struct VideoData
{
	char* data;
	int width;
	int height;
}TVideoData; //视频数据结构体
bool YUV420ToBGR24_FFmpeg(unsigned char* pYUV, unsigned char* pBGR24, int width, int height)
{
	if (width < 1 || height < 1 || pYUV == NULL || pBGR24 == NULL)
		return false;
	AVPicture pFrameYUV, pFrameBGR;
	avpicture_fill(&pFrameYUV, pYUV, AV_PIX_FMT_YUV420P, width, height);
	avpicture_fill(&pFrameBGR, pBGR24, AV_PIX_FMT_BGR24, width, height);
	struct SwsContext* imgCtx = NULL;
	imgCtx = sws_getContext(width, height, AV_PIX_FMT_YUV420P, width, height, AV_PIX_FMT_BGR24, SWS_BILINEAR, 0, 0, 0);
	if (imgCtx != NULL){
		sws_scale(imgCtx, pFrameYUV.data, pFrameYUV.linesize, 0, height, pFrameBGR.data, pFrameBGR.linesize);
		if (imgCtx){
			sws_freeContext(imgCtx);
			imgCtx = NULL;
		}
		return true;
	}
	else{
		sws_freeContext(imgCtx);
		imgCtx = NULL;
		return false;
	}
}
//设备断线回调函数
void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	printf("设备断线.\n");
	return;
}
//自动重连回调函数
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
		//将回调获取的YUV420数据放入list数据结构中
		double Time = (double)cvGetTickCount();
		unsigned char* rgb = new unsigned char[sizeof(char)*nSize*10];
		YUV420ToBGR24_FFmpeg((unsigned char*)pBuf, rgb, pFrameInfo->nWidth, pFrameInfo->nHeight);
		cv::Mat img(pFrameInfo->nHeight, pFrameInfo->nWidth,CV_8UC3,rgb);
		resize(img,img,cv::Size(640,480));
		imshow("test",img);
		cvWaitKey(1);
		img.release();
		delete[] rgb;
		Time = (double)cvGetTickCount() - Time;
		printf("run time = %gms\n", Time / (cvGetTickFrequency() * 1000));
	}
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
		lRealPlay = CLIENT_RealPlayEx(lLogin, 0, NULL, DH_RType_Realplay);
		//CLIENT_RealPlayEx 第二个参数为NVR 播放通道 此处为单通道显示可改为多通道预览
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
		//设置回放时间段
		lSearch = CLIENT_FindFile(lLogin, 0, 0, NULL, time_start, time_end, FALSE, 1000);
		//CLIENT_FindFile 第二个参数为通道号，文件回放只允许开启一个通道进行文件回放 第三个参数为文件类型
		int result = CLIENT_FindNextFile(lSearch, &info);
		//查询到符合参数的文件名 存储于info结构体中
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
