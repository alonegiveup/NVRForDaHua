/************************************************************************/
/* dhplay   版本:  3.34			                                    */
/************************************************************************/

#ifndef _DHPLAY_H
#define _DHPLAY_H

#if (defined(WIN32) || defined(WIN64))
	#ifdef dhplay_EXPORTS
		#define PLAYSDK_API __declspec(dllexport)
	#else
		#define PLAYSDK_API __declspec(dllimport)
	#endif
	#define CALLMETHOD __stdcall
	#define CALLBACK   __stdcall
#else
	#define CALLMETHOD
	#define CALLBACK
	#define PLAYSDK_API //extern "C"
#endif

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#else
#ifdef __OBJC__
#include "objc/objc.h"
#else
#define BOOL        int
#endif
#define BYTE		unsigned char
#define PBYTE		BYTE*
#define LPBYTE		BYTE*
#ifndef LONG
#define LONG		int
#endif
#ifndef DWORD
#define DWORD		unsigned int
#endif
#define WORD		unsigned short
#define COLORREF	DWORD
#define HDC			void*
#define HWND		void*
#define LPSTR		char*
#define UINT		unsigned int
#define TRUE		1
#define FALSE		0
#define ULONGLONG	unsigned long long
#define LONGLONG	long long

typedef struct _SYSTEMTIME
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

#endif

typedef struct _tagRECT
{
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
}DISPLAYRECT;

#ifdef	__cplusplus
extern "C" {
#endif

/************************************************************************/
/* 宏定义			                                                    */
/************************************************************************/

#define  FUNC_MAX_PORT					501		//最大通道号

#define MIN_WAVE_COEF					-100	//PLAY_AdjustWaveAudio使用
#define MAX_WAVE_COEF					100		//PLAY_AdjustWaveAudio使用

#define TIMER_1							1		//多媒体定时器
#define TIMER_2							2		//线程定时器

#define BUF_VIDEO_SRC					1		//视频源缓冲
#define BUF_AUDIO_SRC					2		//音频源缓冲
#define BUF_VIDEO_RENDER				3		//解码后视频数据缓冲
#define BUF_AUDIO_RENDER				4		//解码后音频数据缓冲

#define MAX_DISPLAY_WND					4

#define DISPLAY_NORMAL					1		//正常分辨率显示
#define DISPLAY_QUARTER					2		//1/4分辨率显示

#define MAX_DIS_FRAMES					50		//播放缓冲区最大缓冲帧数
#define MIN_DIS_FRAMES					6		//播放缓冲区最小缓冲帧数

#define BY_FRAMENUM						1		//按帧号方式(PLAY_GetKeyFramePos使用)
#define BY_FRAMETIME					2		//按时间方式(PLAY_GetKeyFramePos使用)

#define SOURCE_BUF_MAX					1024*100000	//数据流缓冲区最大长度
#define SOURCE_BUF_MIN					1024*50		//数据流缓冲区最小长度

#define STREAME_REALTIME				0		//实时流模式
#define STREAME_FILE					1		//文件流模式

#define T_AUDIO16						101		//16位音频数据类型
#define T_AUDIO8						100		//8位音频数据类型

#define T_UYVY							1		//UYVY类型的YUV数据,现在不支持.
#define T_IYUV							3		//IYUV(I420)类型YUV数据
#define T_RGB32							7		//RGB32类型

#define SUPPORT_DDRAW					1		//支持DIRECTDRAW,如果不支持,则播放器不能工作
#define SUPPORT_BLT						2		//显卡支持BLT操作,如果不支持,则播放器不能工作
#define SUPPORT_BLTFOURCC				4		//显卡BLT支持颜色转换,如果不支持,播放器会使用软件方式作RGB转换
#define SUPPORT_BLTSHRINKX				8		//显卡BLT支持X轴缩小,如果不支持,系统使用软件方式转换
#define SUPPORT_BLTSHRINKY				16		//显卡BLT支持Y轴缩小,如果不支持,系统使用软件方式转换
#define SUPPORT_BLTSTRETCHX				32		//显卡BLT支持X轴放大,如果不支持,系统使用软件方式转换
#define SUPPORT_BLTSTRETCHY				64		//显卡BLT支持Y轴放大,如果不支持,系统使用软件方式转换
#define SUPPORT_SSE						128		//CPU支持SSE指令,Intel Pentium3以上支持SSE指令
#define SUPPORT_MMX						256		//CPU支持MMX指令集

#define PLAY_CMD_GetTime				1		//编码中时间信息
#define PLAY_CMD_GetFileRate			2		//帧率信息
#define PLAY_CMD_GetMediaInfo			3		//媒体信息
#define PLAY_CMD_GetRenderNum			4		//当前要渲染的帧号
#define PLAY_CMD_GetRenderTime			5		//当前要渲染的时间
#define PLAY_CMD_GetSrcTime				6
#define PLAY_CMD_GetCurRenderNum		7       //当前帧号。4是上一帧的帧号

#define AVI_MEDIACHANGE_FRAMERATE		1		//帧率改变
#define AVI_MEDIACHANGE_RESOLUTION		2		//分辨率改变

#define WATERMARK_DATA_TEXT				0		//文字
#define WATERMARK_DATA_JPEG_BMP			1		//JPEG或者BMP图片
#define WATERMARK_DATA_FRAMEDATA		3		//帧数据

#define	DH_PLAY_NOERROR					0		//没有错误
#define DH_PLAY_PARA_OVER				1		//输入参数非法
#define DH_PLAY_ORDER_ERROR				2		//调用顺序不对
#define DH_PLAY_TIMER_ERROR				3		//多媒体时钟设置失败
#define DH_PLAY_DEC_VIDEO_ERROR			4		//视频解码失败
#define DH_PLAY_DEC_AUDIO_ERROR			5		//音频解码失败
#define DH_PLAY_ALLOC_MEMORY_ERROR		6		//分配内存失败
#define DH_PLAY_OPEN_FILE_ERROR			7		//文件操作失败
#define DH_PLAY_CREATE_OBJ_ERROR		8		//创建线程事件等失败
#define DH_PLAY_CREATE_DDRAW_ERROR		9		//创建directDraw失败
#define DH_PLAY_CREATE_OFFSCREEN_ERROR	10		//创建后端缓存失败
#define DH_PLAY_BUF_OVER				11		//缓冲区满,输入流失败
#define DH_PLAY_CREATE_SOUND_ERROR		12		//创建音频设备失败
#define DH_PLAY_SET_VOLUME_ERROR		13		//设置音量失败
#define DH_PLAY_SUPPORT_FILE_ONLY		14		//只能在播放文件时才能使用
#define DH_PLAY_SUPPORT_STREAM_ONLY		15		//只能在播放流时才能使用
#define DH_PLAY_SYS_NOT_SUPPORT			16		//系统不支持,解码器只能工作在Pentium 3以上
#define DH_PLAY_FILEHEADER_UNKNOWN		17		//没有文件头
#define DH_PLAY_VERSION_INCORRECT		18		//解码器和编码器版本不对应
#define DH_PLAY_INIT_DECODER_ERROR		19		//初始化解码器失败
#define DH_PLAY_CHECK_FILE_ERROR		20		//文件太短或码流无法识别
#define DH_PLAY_INIT_TIMER_ERROR		21		//初始化多媒体时钟失败
#define DH_PLAY_BLT_ERROR				22		//位拷贝失败
#define DH_PLAY_UPDATE_ERROR			23		//显示overlay失败
#define DH_PLAY_MEMORY_TOOSMALL			24		//缓冲太小


// 大华SVAC解码库宏定义
//SVC  flags
#define SVC_LAYER_BASE    0x0001
#define SVC_LAYER_ENHANCE 0x0002

#define MAX_CUSTOM_EXT_COUNT            8
#define MAX_EXT_ROI_REGION_NUM          32  //最大ROI区域扩展的个数    //标准协议里没规定最大值是多少，这里我们暂定为32应该够用
#define MAX_EXT_EVENT_REGION_NUM        32  //最大监控事件扩展的个数
#define MAX_EXT_ALERT_NUM               32  //最大监控报警扩展的个数

//扩展数据的类型
#define EXT_TYPE_ROI        0x0001      //ROI区域扩展
#define EXT_TYPE_TIME       0x0002      //绝对时间扩展
#define EXT_TYPE_ALERT      0x0004      //报警扩展
#define EXT_TYPE_EVENT      0x0008      //监控事件扩展
#define EXT_TYPE_AUTH       0x0010      //认证数据
#define EXT_TYPE_CUSTOM     0x1000      //自定义扩展


/************************************************************************/
/* 结构体	                                                            */
/************************************************************************/


//时间结构
#ifndef _DHTIME_
#define _DHTIME_
typedef struct _DHTIME								
{
	DWORD second		:6;					//	秒	0-59		
	DWORD minute		:6;					//	分	0-59		
	DWORD hour		:5;					//	时	0-23		
	DWORD day		:5;					//	日	1-31		
	DWORD month		:4;					//	月	1-12		
	DWORD year		:6;					//	年	2000-2063	
} DHTIME,*pDHTIME;
#endif

//文件信息结构,本地使用
typedef struct _FILE_INFO
{
	UINT		channel;				//通道号
	BYTE		alarm;					//报警类型
	BYTE		lock_flag;				//锁定类型(0未锁定 1手动 2自动)
	BYTE		video_audio;			//视频或音频
	BYTE		image;					//图像质量(bit4~6保留, bit7 表示夏令时)
	BYTE		hint;					//查找文件时使用的暗示参数
	DHTIME	start_time ;			//开始时间
	DHTIME	end_time ;				//结束时间
	UINT		file_length ;			//文件长度，单位为KB
	UINT		disk_no;				//所在的硬盘号
	UINT		disk_part;				//所在的分区
	UINT		first_clus_no ;			//文件首簇号
	UINT		ud_no;					//用户数据号
	BYTE		base_name[24];			//文件名
	BYTE		ext_name[4];			//扩展名
	UINT		checked;				//是否选中，用于文件备份
}FILE_INFO,*pFILE_INFO;


typedef enum __tPicFormats
{
	PicFormat_BMP = 0,				        //BMP32类型
    PicFormat_JPEG,							//JPEG类型
	PicFormat_JPEG_70,						//70%质量的JPEG类型
	PicFormat_JPEG_50,						//50%质量的JPEG类型
	PicFormat_JPEG_30,						//30%质量的JPEG类型
	PicFormat_JPEG_10,						//10%质量的JPEG类型
	PicFormat_BMP24,                        //BMP24类型
	PicFormat_TIFF							//TIFF类型
} tPicFormats;

typedef struct
{
	LONGLONG		nFilePos;				//指定帧在文件中的偏移位置
	LONG			nFrameLen;				//帧长度
	LONG			nFrameNum;				//帧序号
	LONG			nFrameTime;				//帧时间
	LONG			nErrorFrameNum;			//保留,暂无使用
	SYSTEMTIME*		pErrorTime;				//保留,暂无使用
	LONG			nErrorLostFrameNum;		//保留,暂无使用
	LONG			nErrorFrameSize;		//保留,暂无使用
}FRAME_POS,*PFRAME_POS;

typedef struct
{
	LONG			nWidth;					//画面宽,单位像素.如果是音频数据则为0.
	LONG			nHeight;				//画面高,如果是音频数据则为0
	LONG			nStamp;					//时标信息,单位毫秒
	LONG			nType;					//视频帧类型,T_AUDIO16,T_RGB32,T_IYUV
	LONG			nFrameRate;				//编码时产生的图像帧率
}FRAME_INFO;

typedef struct
{
	#define FRAME_TYPE_VIDEO 0				//视频帧
	#define FRAME_TYPE_AUDIO 1				//音频帧
	int				nFrameType;				//视频帧类型,见上面定义
	int				nFrameSeq;				//帧序号
	int				nStamp;					//时标信息,单位毫秒
	int				nWidth;					//画面宽,单位像素.如果是音频数据则为0.
	int 			nHeight;				//画面高,如果是音频数据则为0
	int				nFrameRate;				//编码时产生的图像帧率
	int				nChannels;				//音频通道数
	int				nBitPerSample;			//音频采样位数
	int				nSamplesPerSec;			//音频采样频率
	int				nRemainData;			//缓冲剩余数据量
	SYSTEMTIME		nDataTime;				//时间
	int				nReserved[59];			//保留字段
}FRAME_INFO_EX;

typedef struct 
{
	int				nFrameType;					//帧类型,定义见FRAME_INFO_EX里nFrameType字段
	
	void*			pAudioData;				//音频数据,如果是音频帧
	int				nAudioDataLen;			//音频数据长度

	void*			pVideoData[3];			//分别表示视频的YUV三个分量
	int				nStride[3];				//分别表示YUV三个分量的跨距
	int				nWidth[3];				//分别表示YUV三个分量的宽度
	int				nHeight[3];				//分别表示YUV三个分量的高度
	int				nReserved[64];			//保留字段
}FRAME_DECODE_INFO;

typedef struct 
{
	int			lWidth;					//画面宽,单位像素.
	int			lHeight;				//画面高
	int			lFrameRate;				//帧率
	int			lChannel;				//音频通道数
	int			lBitPerSample;			//音频采样位数
	int			lSamplesPerSec;			//音频采样频率
}MEDIA_INFO;

typedef struct 
{
	char*			pDataBuf;				//帧数据
	LONG			nSize;					//帧数据长度
	LONG			nFrameNum;				//帧序号
	BOOL			bIsAudio;				//是否音频帧
	LONG			nReserved;				//保留
}FRAME_TYPE;



// 解码附加信息
typedef struct
{
    LONG			nVideoEncodeType;   // 码流类型
    LONG	        nDataLen;           // 解码数据长度
    char*           pUserData;              // 解码信息
    LONG	        nReserved[2];
}FRAME_DEC_EXT_INFO;

// 大华SVAC解码库解码附加信息自定义字段
typedef struct
{
    unsigned char type;             // 信息类型
    unsigned char length;           // 信息长度
    unsigned char *pbuf;            // 信息指针
    unsigned char reserved[2];      // 保留
}DEC_EXT_INFO_CUSTOM;

#define ZXW_OSD_MAX_LEN 243     //++OSD_DATA的最长字符数
#define ZXW_OSD_TYPE_NUM 3      //++OSD的种类，目前支持三种：时间、通道、地点标注
typedef struct
{
	unsigned char subtype;   //subtype为32,33,34时，表示按照本文档的描述进行解析。同一个subtype的扩展信息不会同时出现两个；当同类信息的扩展信息出现时，后一个扩展信息总是覆盖前一个扩展信息。
	unsigned char codeType;  //编码格式，当前仅0在使用，表示使用UTF-8编码
	unsigned char alignType; //对齐格式，0为左对齐，1为右对齐
	unsigned char charSize;  //字体大小，用像素表示
	unsigned char charType;  //字符格式，0为白底黑边，1为黑底白边，2为白色字体，3为黑色字体
	unsigned short top_pixel;      //位置，按像素表示 
	unsigned short left_pixel;     //位置，按像素表示 
	unsigned char len;       //字符在 OSD_DATA中占用的字节数 
	unsigned char res[3];    //3个保留字节 
	unsigned char OSD_DATA[ZXW_OSD_MAX_LEN];  //OSD 字符数据，其长度由 Len 确定。支持
	//换行，定义'\n'为换行，'\0'为结束符（同时也受到字符长度的约束）。OSD_DATA的最长
	//长度为 243字节。
}DEC_EXT_INFO_ZXW_OSD;

typedef struct
{
    int flags;                              //扩展数据类型标记

    int time_year;                          //绝对时间扩展
    int time_month;
    int time_day;
    int time_hour;
    int time_minute;
    int time_second;
    int time_sec_fractional;
    int time_frame_num;

    int roi_region_num;                             // ROI区域扩展
    int roi_top_left_x[MAX_EXT_ROI_REGION_NUM];     // ROI区域左上角像素x坐标
    int roi_top_left_y[MAX_EXT_ROI_REGION_NUM];     // ROI区域左上角像素y坐标
    int roi_width[MAX_EXT_ROI_REGION_NUM];          // ROI区域宽度
    int roi_height[MAX_EXT_ROI_REGION_NUM];         // ROI区域高度

    int event_region_num;                           // 监控事件扩展
    int event_num[MAX_EXT_EVENT_REGION_NUM];
    int event_region_event_id[MAX_EXT_EVENT_REGION_NUM][16];

    int alert_num;                                  // 监控报警扩展
    int alert_region_id[MAX_EXT_ALERT_NUM];
    int alert_event_id[MAX_EXT_ALERT_NUM];
    int alert_frame_num[MAX_EXT_ALERT_NUM];

    //认证数据相关参数
    int *auth_buf[2];                               // 认证数据buf, auth_buf[0]是基本层认证数据, auth_buf[1]是增强层认证数据
    int auth_len[2];                                // 认证数据长度, 不存在认证数据时，长度应该设置为 0
    int auth_signature_type[2];
    int auth_hash_hierarchy_flag[2];

    DEC_EXT_INFO_CUSTOM svac_ext_info_custom[MAX_CUSTOM_EXT_COUNT];   // 扩展
	DEC_EXT_INFO_ZXW_OSD svac_ext_info_zxw_osd[ZXW_OSD_TYPE_NUM];     //存放中星微的OSD信息

}DEC_EXT_PARAM_SVAC;

/***begin鱼眼定义***/
typedef enum
{
	FISHEYEMOUNT_MODE_INVALID = 0,
	FISHEYEMOUNT_MODE_CEIL = 1,						/*顶装*/
	FISHEYEMOUNT_MODE_WALL,							/*壁装*/
	FISHEYEMOUNT_MODE_FLOOR,						/*地装*/
	FISHEYEMOUNT_MODE_NUM
}FISHEYE_MOUNTMODE;

typedef enum
{
	FISHEYECALIBRATE_MODE_INVALID = 0,
	FISHEYECALIBRATE_MODE_OFF = 1,								/*关闭鱼眼算法库，外部关闭*/
	FISHEYECALIBRATE_MODE_ORIGINAL,								/*原始模式(正方形),带缩放比例*/
	FISHEYECALIBRATE_MODE_PANORAMA,								/*1p*/
	FISHEYECALIBRATE_MODE_PANORAMA_PLUS_ONE_EPTZ,				/*1p+1*/
	FISHEYECALIBRATE_MODE_DOUBLE_PANORAMA,						/*2p*/
	FISHEYECALIBRATE_MODE_ORIGINAL_PLUS_DOUBLE_PANORAMA,		/*1+2p*/
	FISHEYECALIBRATE_MODE_ORIGINAL_PLUS_THREE_EPTZ_REGION,		/*1+3*/
	FISHEYECALIBRATE_MODE_PANORAMA_PLUS_THREE_EPTZ_REGION,		/*1p+3*/
	FISHEYECALIBRATE_MODE_ORIGINAL_PLUS_TWO_EPTZ_REGION,		/*1+2*/	
	FISHEYECALIBRATE_MODE_ORIGINAL_PLUS_FOUR_EPTZ_REGION,		/*1+4*/
	FISHEYECALIBRATE_MODE_PANORAMA_PLUS_FOUR_EPTZ_REGION,		/*1p+4*/
	FISHEYECALIBRATE_MODE_PANORAMA_PLUS_SIX_EPTZ_REGION,		/*1p+6*/
	FISHEYECALIBRATE_MODE_ORIGINAL_PLUS_EIGHT_EPTZ_REGION,		/*1+8*/
	FISHEYECALIBRATE_MODE_PANORAMA_PLUS_EIGHT_EPTZ_REGION,		/*1p+8*/
	FISHEYECALIBRATE_MODE_TWO_EPTZ_REGION_WITH_ORIGINAL,		/*1F+2*/	
	FISHEYECALIBRATE_MODE_FOUR_EPTZ_REGION_WITH_ORIGINAL,		/*1F+4*/	
	FISHEYECALIBRATE_MODE_DOUBLE_PANORAMA_WITH_ORIGINAL,		/*1F+2p*/
	FISHEYECALIBRATE_MODE_FOUR_EPTZ_REGION_WITH_PANORAMA,		/*1p(F)+4*/
	FISHEYECALIBRATE_MODE_TWO_EPTZ_REGION,		                /*2画面*/
	FISHEYECALIBRATE_MODE_SINGLE,								/*单画面*/
	FISHEYECALIBRATE_MODE_FOUR_EPTZ_REGION,						/*4画面*/
	FISHEYECALIBRATE_MODE_USER_DEFINED,				    		/*用户自定义*/
	FISHEYECALIBRATE_MODE_PHONE,								/*手机模式*/
	FISHEYECALIBRATE_MODE_ORIGINAL_PLUS_ONE_EPTZ_REGION,		/*1+1*/
	FISHEYECALIBRATE_MODE_ONE_EPTZ_REGION,						/*1画面*/

	FISHEYECALIBRATE_MODE_NUM
}FISHEYE_CALIBRATMODE;

typedef enum
{
	FISHEYEEPTZ_CMD_INVALID = 0,	
	FISHEYEEPTZ_CMD_ZOOM_IN = 1,						/*放大*/
	FISHEYEEPTZ_CMD_ZOOM_OUT,							/*缩小*/
	FISHEYEEPTZ_CMD_UP,									/*向上移动*/
	FISHEYEEPTZ_CMD_DOWN,								/*向下移动*/
	FISHEYEEPTZ_CMD_LEFT,								/*向左移动*/
	FISHEYEEPTZ_CMD_RIGHT,								/*向右移动*/
	FISHEYEEPTZ_CMD_ROTATE_CLOCKWISE_AUTO,				/*自动顺时针旋转*/
	FISHEYEEPTZ_CMD_ROTATE_ANTICLOCKWISE_AUTO,			/*自动逆时针旋转*/
	FISHEYEEPTZ_CMD_STOP,								/*停止*/
	FISHEYEEPTZ_CMD_SHOW_REGION,						/*框选放大*/
	FISHEYEEPTZ_CMD_EXIT_SHOW_REGION,					/*退出框选放大*/
	FISHEYEEPTZ_CMD_DEFAULT,						    /*恢复默认*/
	FISHEYEEPTZ_CMD_ORIGIN_ROTATE,						/*圆旋转*/

	FISHEYEEPTZ_CMD_SET_CUR_REGION = 0x20,             /*配置指定窗口的位置信息*/
	FISHEYEEPTZ_CMD_GET_CUR_REGION,                    /*获取指定窗口的位置信息*/
	FISHEYEEPTZ_CMD_IS_IN_PANORAMA_REGION,             /*输入点是否在当前全景点链区域内*/
	FISHEYEEPTZ_CMD_TAP_VIEW,							/*显示指定位置,即点即看*/
	FISHEYEEPTZ_CMD_SET_FOCUS,	 				        /*设置窗口位置信息*/
	FISHEYEEPTZ_CMD_GET_FOCUS,							/*获取窗口位置信息*/

	FISHEYEEPTZ_CMD_PTZ_CALI,							/*鱼球联动标定*/
	FISHEYEEPTZ_CMD_GET_PTZ_RLT,						/*获取鱼球联动定位信息*/
	FISHEYEEPTZ_CMD_NUM
}FISHEYE_EPTZCMD;

typedef struct
{
	int w;
	int h;
}FISHEYE_SIZE;

typedef struct
{
	short x;
	short y;
}FISHEYE_POINT2D;

typedef struct
{
	FISHEYE_MOUNTMODE     subMountMode;			    /*子图像安装模式, 仅当图像主校正模式为用户自定义模式时, 该值有效*/
	FISHEYE_CALIBRATMODE  subCalibrateMode;	        /*子图像校正模式, 仅当图像主校正模式为用户自定义模式时, 该值有效*/
	FISHEYE_SIZE          imgOutput;                /*子图像输出分辨率*/
	FISHEYE_POINT2D       upperLeft;                /*子图像偏移*/
	int					  reserved[3];				/*保留字节*/
}FISHEYE_SUBMODE;

typedef struct
{
	FISHEYE_SIZE          mainShowSize;		        /*暂不启用, 主显示比例, 4:3, 16:9等, 算法据此输出适合的最优结果（不变形情况下尽量达到该比例）*/
	FISHEYE_SIZE          floatMainShowSize;		/*输出双buffer时使用，目前暂时还是用老的操作方法，浮动窗口的主显示窗口分辨率, 浮动圆的宽高比需要为1:1，浮动壁装全景的宽高比需要为16:9*/
	FISHEYE_SIZE          imgOutput;                /*输出图像分辨率(缩放前), 图像主校正模式为用户自定义模式时为外部输入, 其他模式为内部返回*/
	FISHEYE_SUBMODE*	  subMode;                  /*子模式信息, 图像主校正模式为用户自定义模式时为外部输入, 其他模式为内部返回*/
	int		              subModeNum;               /*子模式数, 图像主校正模式为用户自定义模式时为外部输入, 其他模式为内部返回*/
	int                   outputSizeRatio;		    /*暂不启用, 校正输出图像的缩放比,Q8格式,范围0-256, 256为保持最大输出分辨率*/
	int                   reserved[1];				/*保留字节*/
}FISHEYE_OUTPUTFORMAT;

typedef struct
{
	int x;
	int y;
	int hAngle;
	int vAngle;
	int available;
	int reserved[3];
}FISHEYE_REGIONPARAM;

typedef struct
{
	FISHEYE_REGIONPARAM   regionParam[9];
	int              circularOffset;
	int              panoramaOffset;
	int              useRegionParam;           /*为1时表明有效，使用该值进行初始化；没有保存信息时请置为0*/
	int              reserved[1];
}FISHEYE_MODEINITPARAM;

/*枪机类型*/
typedef enum CAM_TYPE
{
	//枪机类型
	IPCTYPE_200WN				= 0,	//
	IPCTYPE_130WN				= 1,
	IPCTYPE_D1WN				= 2,
	IPCTYPE_100WN				= 3,
	IPCTYPE_FE					= 4,	//鱼眼

	//球机类型
	SPCTYPE_D6501				= 100,	//sony机芯65球机
	HSPCTYPE_D6A2030E			= 101,	//大华机芯2030E，6A球机
	HSPCTYPE_D65A2030E			= 102	//大华机芯2030E，65A球机
}CAM_TYPE;

/*镜头类型*/
typedef enum LEN_TYPE
{
	LENTYPE_NORM				= 0,	//无畸变镜头
	LENTYPE_Lens0361 			= 1,	//3.6毫米枪机镜头
	LENTYPE_Lens2880			= 2,	//130度广角枪机镜头
	LENTYPE_Lens0362 			= 3,	//3.6毫米枪机镜头
	LENTYPE_Lens0401 			= 4,	//4.0毫米枪机镜头

	LENTYPE_TEST1				= 100	//调试用参数
}LEN_TYPE;

typedef struct
{	
	/*必设参数*/
	int zoom_type;							//倍数控制模式----期望自适应变倍和根据框选目标大小变倍两种模式
	int hcam_wax;							//期望倍数对应球机角度x（水平）
	int hcam_way;							//期望倍数对应球机角度y（垂直）
	int hcam_wmul;							//期望倍数设置（基准倍数）
	int cfg_type;							//配置方式，默认为1：使用配置参数方式,1：使用参数配置方式，0：使用设备类型配置方式

	//主相机参数							         
	//镜头参数
	int prm_re;								//投影半径
	int prm_mul;							//投影倍率
	int prm_dx;								//x方向偏移
	int prm_dy;								//y方向偏移
	int prm_cw;								//CMOS宽（实际使用宽）
	int prm_ch;								//CMOS高（实际使用高）

	//主相机和从相机类型配置（cfg_type为0时设置该参数才有效）            //默认130度、130万枪机和200W65球机
	LEN_TYPE mlen_type;						//主相机镜头类型
	CAM_TYPE mcam_type;						//主相机类型
	CAM_TYPE hcam_type;						//从相机类型

	//球机参数
	int himg_width;                         //从相机图像宽
	int himg_height;                        //从相机图像高
	int prm_fax;                            //球机水平视场角

	/*可默认的参数*/
	//主相机参数
	int mcam_fc;							//相机等效焦距
	int mcam_cw;							//镜头靶面高
	int mcam_ch;							//镜头靶面宽 
	int cam_height;                         //相机架设高度（米），（暂时未使用）
	int prm_ma;								//焦距参数

	//从相机参数
	//球机参数
	int prm_hw;								//CMOS宽
	int prm_hh;								//CMOS高
	int prm_fo;								//等效焦距
	int prm_ca;								//视野参数
	int prm_mmul;							//最大倍率
}MHFPTZ_CONFIGPARAM;

typedef struct
{
	FISHEYE_SIZE     mainStreamSize;		    /*对应主码流原始宽高，当传入分辨率与之不等时表明为辅码流是此分辨率缩放而来*/
	int              originX;					/*输入图像中鱼眼圆的圆心横坐标, 归一化至0-8192坐标系*/
	int              originY;					/*输入图像中鱼眼圆的圆心纵坐标, 归一化至0-8192坐标系*/
	int              radius;					/*输入图像中鱼眼圆的半径, 归一化至0-8192坐标系*/ 
	int              lensDirection;		        /*旋转角度, Q7格式, 范围0-360*128, 一般配为0*/
	FISHEYE_MOUNTMODE     mainMountMode;		/*主安装模式*/
	FISHEYE_CALIBRATMODE  mainCalibrateMode;	/*图像主校正模式*/
	FISHEYE_MODEINITPARAM modeInitParam;        /*外部传入模式初始化各画面信息，适用于模式切换恢复到上一次的状态,*/
	FISHEYE_OUTPUTFORMAT *outputFormat;         /*输出图像信息*/
	MHFPTZ_CONFIGPARAM   *configParam;          /*鱼球联动配置参数*/
	int              enableAutoContrast;       /*开启自动对比度, 0关闭, 1开启, 该功能会增加算法耗时, 需要性能好的机器才建议开启*/
	int              alphaHistogram;           /*直方图IIR强度0-255, 默认128, 越大越参考当前帧*/
	int              alphaGray;                /*灰度拉伸强度0-255, 默认245, 越大越对比度弱*/
	FISHEYE_SIZE     captureSize;		       /*对应当前模式下的抓图分辨率*/
	int              mhfptzIndex;               //IN 鱼球联动球机序号0,1,2....
	int              reserved[1];				/*保留字节*/
}FISHEYE_OPTPARAM;

typedef struct
{
	FISHEYE_EPTZCMD   ePtzCmd;			/*云台操作，说明见FISHEYE_EPtzCmd定义*/
	int          winId;				    /*要进行eptz的窗口编号，左上角winId为0，从左到右递增*/							
	int          arg1;
	int          arg2;
	int          arg3;
	int          arg4;
	int          arg5;
	int          arg6;
	int          reserved0[6];			   /*保留字节*/
	void*        pParam;                   /*鱼球联动*/
	void*        pResult;
	void*        pArg;   

	int          reserved1[7];			   /*保留字节*/

}FISHEYE_EPTZPARAM;

typedef enum
{
	FISHEYE_SETPARAM,
	FISHEYE_GETPARAM

}FISHEYE_OPERATETYPE;
/***end鱼眼定义***/

/************************************************************************/
/* 接口		                                                            */
/************************************************************************/

//------------------------------------------------------------------------
// 函数: PLAY_OpenFile
// 描述: 打开播放文件，用于在播放文件之前调用. 文件不能超过4G或小于4K
// 参数: nPort,通道号
//		 sFileName,文件名
// 返回: BOOL,成功返回TRUE,失败返回FALSE
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_OpenFile(LONG nPort,LPSTR sFileName);

//------------------------------------------------------------------------
// 函数: PLAY_CloseFile
// 描述: 关闭文件, 在PLAY_Stop后调用
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_CloseFile(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_OpenStream
// 描述: 打开流播放
// 参数: nPort,通道号
//		 pFileHeadBuf,目前不使用,填NULL.
//		 nSize,目前不适用,填0.
//		 nBufPoolSize,设置播放器中存放数据流的缓冲区大小.
//			范围是[SOURCE_BUF_MIN,SOURCE_BUF_MAX].一般设为900*1024，如果数
//			据送过来相对均匀,可调小该值,如果数据传输不均匀,可增大该值.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_OpenStream(LONG nPort,PBYTE pFileHeadBuf,DWORD nSize,DWORD nBufPoolSize);

//------------------------------------------------------------------------
// 函数: PLAY_CloseStream
// 描述: 关闭流
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_CloseStream(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_Play
// 描述: 开始播放.如果已经播放,改变当前播放状态为正常速度播放.
// 参数: nPort,通道号
//		 hWnd,播放窗口句柄
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_Play(LONG nPort, HWND hWnd);

//------------------------------------------------------------------------
// 函数: PLAY_Stop
// 描述: 停止播放
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_Stop(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_InputData
// 描述: 输入数据流,PLAY_Play之后使用
// 参数: nPort,通道号
//		 pBuf,缓冲区地址
//		 nSize,缓冲区大小
// 返回: BOOL,成功返回TRUE,失败返回FALSE.如失败,一般是缓冲区已满,用户可暂停
//			输入，一段时间之后再输入流，确保播放库不丢失数据。
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_InputData(LONG nPort,PBYTE pBuf,DWORD nSize);

//------------------------------------------------------------------------
// 函数: PLAY_Pause
// 描述: 暂停或者恢复播放
// 参数: nPort,通道号
//		 nPause,TRUE暂停,FLASE恢复
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_Pause(LONG nPort,DWORD nPause);

//------------------------------------------------------------------------
// 函数: PLAY_Fast
// 描述: 播放速度范围为[1/64, 64]
//		 每次调用播放速度提升一级(*2),要恢复正常播放调用
//		 PLAY_Play,从当前位置开始正常播放.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_Fast(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_Slow
// 描述: 播放速度范围为[1/64, 64]
//		 每次调用播放速度降一级(*0.5),要恢复正常播放调用PLAY_Play.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_Slow(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_OneByOne
// 描述: 单桢播放,要恢复正常播放调用PLAY_ Play.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_OneByOne(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetPlayPos
// 描述: 设置文件播放指针的相对位置(百分比)
// 参数: nPort,通道号
//		 fRelativePos,文件长度的百分比
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetPlayPos(LONG nPort,float fRelativePos);

//------------------------------------------------------------------------
// 函数: PLAY_SetPlayDirection
// 描述: 设置播放方向
// 参数: nPort,通道号
//		 emDirection,播放方向：0，向前，1，向后
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetPlayDirection(LONG nPort, DWORD emDirection);

//------------------------------------------------------------------------
// 函数: PLAY_GetPlayPos
// 描述: 获取文件播放指针的相对位置(百分比)
// 参数: nPort,通道号
// 返回: float,文件长度的相对位置,范围0-100%
//------------------------------------------------------------------------
PLAYSDK_API float CALLMETHOD PLAY_GetPlayPos(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetFileEndMsg
// 描述: 设置文件结束时要发送的消息
// 参数: nPort,端口号
//       hWnd,消息发送窗口
//       nMsg,用户自定义的输入的消息，当播放到文件结束时用户在hWnd窗口过程中
//			收到这个消息。此消息中的wParam或者lParam都可以获得nPort的值.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetFileEndMsg(LONG nPort,HWND hWnd,UINT nMsg);

//------------------------------------------------------------------------
// 函数: PLAY_SetVolume
// 描述: 设置音量,设置的音量是指声卡输出的音量,会影响其他的声音应用.
// 参数: nPort,通道号
//		 nVolume,音量的值,范围[0,0xFFFF]
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetVolume(LONG nPort,WORD nVolume);

//------------------------------------------------------------------------
// 函数: PLAY_StopSound
// 描述: 关闭独占方式声音
// 参数: 
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_StopSound();

//------------------------------------------------------------------------
// 函数: PLAY_PlaySound
// 描述: 独占方式打开声音,同一时刻只能有一路声音,如果现在已经有声音打开，则
//		 自动关闭原来已经打开的声音.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_PlaySound(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetFileTime
// 描述: 获取文件总时间
// 参数: nPort,通道号
// 返回: 文件总的时间长度值,单位秒.
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetFileTime(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetPlayedTime
// 描述: 获取当前文件播放时间
// 参数: nPort,通道号
// 返回: 文件当前播放的时间,单位秒
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetPlayedTime(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetPlayedFrames
// 描述: 获取已播放的桢数
// 参数: nPort,通道号
// 返回: 已经解码的视频帧数
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetPlayedFrames(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetDecodeCallBack
// 描述: 设置解码回调,替换播放器中的显示部分,由用户自己控制显示,该函数在
//			PLAY_Play之前调用,在PLAY_Stop时自动失效,下次调用PLAY_Play之前
//			需要重新设置.解码部分不控制速度,只要用户从回调函数中返回,解码器
//			就会解码下一部分数据.适用于只解码不显示的情形。
// 参数: nPort,通道号
//		 cbDec,解码回调函数指针,不能为NULL.回调函数参数含义如下:
//			nPort,通道号
//			pFrameDecodeInfo,解码后的音视频数据
//			pFrameInfo,图像和声音信息,请参见FRAME_INFO结构体
//			pUser,用户自定义参数
//		 pUser,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK* fCBDecode)(LONG nPort, FRAME_DECODE_INFO* pFrameDecodeInfo, FRAME_INFO_EX* pFrameInfo, void* pUser);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetDecodeCallBack(LONG nPort, fCBDecode cbDec, void* pUser);

//------------------------------------------------------------------------
// 函数: PLAY_SetDisplayCallBack
// 描述: 设置视频抓图回调函数.如果要停止回调,可以把回调函数指针设为NULL,该
//			函数可以在任何时候调用
// 参数: nPort,通道号
//		 DisplayCBFun,抓图回调函数,可以为NULL,其参数含义如下
//			nPort,通道号
//			pBuf,返回图像数据
//			nSize,返回图像数据大小
//			nWidth,画面宽,单位像素
//			nHeight,画面高
//			nStamp,时标信息，单位毫秒
//			nType,数据类型,T_RGB32,T_UYVY,详见宏定义说明
//			nReceaved,对应用户自定义参数
//		 pUserData,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK* fDisplayCBFun)(LONG nPort,char * pBuf,LONG nSize,LONG nWidth,LONG nHeight,LONG nStamp,LONG nType, void* pReserved);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetDisplayCallBack(LONG nPort, fDisplayCBFun DisplayCBFun, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_ConvertToBmpFile
// 描述: 图像格式转为BMP格式.
// 参数: pBuf,图像数据指针
//		 nSize,图像数据大小
//		 nWidth,图像宽度
//		 nHeight,图像高度
//		 nType,数据类型.T_RGB32,T_UYVY等.
//		 sFileName,要保存的文件名.最好以BMP作为文件扩展名.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_ConvertToBmpFile(char * pBuf,LONG nSize,LONG nWidth,LONG nHeight,LONG nType, char *sFileName);

//------------------------------------------------------------------------
// 函数: PLAY_ConvertToJpegFile
// 描述: 图像格式转为JPEG格式.
// 参数: pYUVBuf,图像数据指针
//		 nWidth,图像宽度
//		 nHeight,图像高度
//		 YUVtype,YUV数据类型,如T_YV12,T_UYVY
//		 quality,图片压缩质量,范围(0, 100].
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_ConvertToJpegFile(char *pYUVBuf, LONG nWidth, LONG nHeight, int YUVtype, int quality, char *sFileName);

//------------------------------------------------------------------------
// 函数: PLAY_GetFileTotalFrames
// 描述: 获取文件总帧数
// 参数: nPort,通道号
// 返回: DWORD,文件中的总帧数
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetFileTotalFrames(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetCurrentFrameRate
// 描述: 获取当前帧率
// 参数: nPort,通道号
// 返回: DWORD,当前码流中编码时的帧率值
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetCurrentFrameRate(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetPlayedTimeEx
// 描述: 获取文件当前播放时间
// 参数: nPort,通道号
// 返回: DWORD,文件当前播放的时间,单位毫秒.
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetPlayedTimeEx(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetPlayedTimeEx
// 描述: 设置文件当前播放时间.
// 参数: nPort,通道号
//		 nTime,设置文件播放位置到指定时间,单位毫秒.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetPlayedTimeEx(LONG nPort,DWORD nTime);

//------------------------------------------------------------------------
// 函数: PLAY_GetCurrentFrameNum
// 描述: 获取当前播放的帧序号,PLAY_GetPlayedFrames是总共解码的帧数.如果文件
//			播放位置不被改变，那么这两个函数的返回值应该非常接近，除非码流丢
//			失数据.
// 参数: nPort,通道号
// 返回: 当前播放的帧序号.
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetCurrentFrameNum(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetStreamOpenMode
// 描述: 设置流播放模式,必须在播放之前设置.
// 参数: nPort,通道号
//		 nMode,STREAME_REALTIME实时模式(默认);STREAME_FILE文件模式.实时模
//			式,适合播放网络实时数据;文件模式,适合用户把文件数据用流方式输入.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetStreamOpenMode(LONG nPort,DWORD nMode);

//------------------------------------------------------------------------
// 函数: PLAY_GetSdkVersion
// 描述: 获取播放库SDK版本号,次版本号和补丁号
// 参数: 
// 返回: DWORD,高16位表示当前的主版本号;9~16位表示次版本号;1~8位表示次补丁号;
//			如:返回值0x00030107表示:主版本号是3,次版本号是1,补丁号是7.
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetSdkVersion();

//------------------------------------------------------------------------
// 函数: PLAY_GetLastError
// 描述: 获取错误码
// 参数: nPort,通道号
// 返回: DWORD,获得当前错误的错误码.请参见错误码宏定义.
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetLastError(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_RefreshPlay
// 描述: 刷新显示.当用户暂停时如果刷新了窗口,则窗口中的图像因为刷新而消失,
//			此时调用这个接口可以重新把图像显示出来.只有在暂停和单帧播放时才
//			执行,其它情况会直接返回.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_RefreshPlay(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetPictureSize
// 描述: 获取原始图像大小,根据此大小来设置显示窗口的区域,可以不用显卡做缩放
//			工作,对于那些不支持硬件缩放的显卡来说非常有用.
// 参数: nPort,通道号
//		 pWidth,输出参数,返回图片宽度
//		 pHeight,输出参数,返回图片高度
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetPictureSize(LONG nPort,LONG *pWidth,LONG *pHeight);

//------------------------------------------------------------------------
// 函数: PLAY_SetPicQuality
// 描述: 设置图像质量,当设置成高质量时画面效果好,但CPU利用率高.在支持多路播放时，
//			可以设为低质量,以降低CPU利用率.当某路放大播放时将该路设置成高质量，
//			以达到好的画面效果.
// 参数: nPort,通道号
//		 bHighQuality,为TRUE(1)时图像高质量,为FALSE(0)时低质量(默认值).
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetPicQuality(LONG nPort,BOOL bHighQuality);

//------------------------------------------------------------------------
// 函数: PLAY_PlaySoundShare
// 描述: 以共享方式播放声音,播放本路声音而不去关闭其他路的声音.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_PlaySoundShare(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_StopSoundShare
// 描述: 关闭声音(共享方式).在同一个进程中,所有通道必须使用相同的方式播放或关闭声音.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_StopSoundShare(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetStreamOpenMode
// 描述: 获取流播放模式
// 参数: nPort,通道号
// 返回: LONG,STREAME_REALTIME或STREAME_FILE.
//------------------------------------------------------------------------
PLAYSDK_API LONG CALLMETHOD PLAY_GetStreamOpenMode(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetVolume
// 描述: 获取音量,这里的音量是指声卡输出的音量.
// 参数: nPort,通道号
// 返回: WORD,音量值
//------------------------------------------------------------------------
PLAYSDK_API WORD CALLMETHOD PLAY_GetVolume(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetPictureQuality
// 描述: 获取图像质量
// 参数: nPort,通道号
//		 bHighQuality,输出参数,TRUE表示高质量,FALSE表示低质量.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetPictureQuality(LONG nPort,BOOL *bHighQuality);

//------------------------------------------------------------------------
// 函数: PLAY_GetSourceBufferRemain
// 描述: 获取流播放模式下源缓冲区剩余数据大小
// 参数: nPort,通道号
// 返回: DWORD,当前源缓冲的大小单位BYTE(字节).
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetSourceBufferRemain(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_ResetSourceBuffer
// 描述: 清空流播放模式下源缓冲区的剩余数据
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_ResetSourceBuffer(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_OneByOneBack
// 描述: 单帧回退,此接口只支持文件播放,必须在文件索引生成之后才能调用.每调
//			用一次倒退一帧
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_OneByOneBack(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetFileRefCallBack
// 描述: 设置建立索引回调,在文件打开时生成文件索引.这个过程耗时比较长,大约
//			每秒处理40M左右的数据,因为从硬盘读数据比较慢,建立索引的过程是在
//			后台完成,需要使用索引的函数要等待这个过程结束,其他接口不受影响.
// 参数: nPort,通道号
//		 pFileRefDone,回调函数指针,其参数含义如下:
//			nPort,通道号
//			pUserData,用户自定义参数
//		 pUserData,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void(CALLBACK *fpFileRefDoneCBFun)(DWORD nPort, void* pUserData);									
PLAYSDK_API BOOL CALLMETHOD PLAY_SetFileRefCallBack(LONG nPort,
													fpFileRefDoneCBFun pFileRefDoneCBFunc,
													void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_SetCurrentFrameNum
// 描述: 设置文件当前帧播放帧号,此函数必须在文件索引生成之后才能调用.
// 参数: nPort,通道号
//		 nFrameNum,帧序号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetCurrentFrameNum(LONG nPort,DWORD nFrameNum);

//------------------------------------------------------------------------
// 函数: PLAY_GetKeyFramePos
// 描述: 查找指定位置之前的关键帧位置,图像解码必须从关键帧开始，如果用户保存
//			的文件不是从关键帧开始的,那么倒下一个关键帧之前的数据会被忽略.如
//			果用户要截取文件中的一段数据,则应该考虑从关键帧开始截取.结束位置
//			则关系不大,最多丢失3帧数据.在文件索引建立完全的前提下,与
//			PLAY_GetNextKeyFramePos联合使用,可以用来实现剪辑录像文件,剪辑精度
//			与关键帧间隔有关.
// 参数: nPort,通道号
//		 nValue,当前位置,可以是时间或帧号,类型由nType指定.
//		 nType,指定nValue的类型.如果nType是BY_FRAMENUM,则nValue表示帧号;
//			如果nType是BY_FRAMTIME,则nValue表示时间,单位ms.
//		 pFramePos,查找到的关键帧的文件位置信息结构指针,详见PFRAME_POS.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetKeyFramePos(LONG nPort,DWORD nValue, DWORD nType, PFRAME_POS pFramePos);

//------------------------------------------------------------------------
// 函数: PLAY_GetNextKeyFramePos
// 描述: 查找指定位置之后的关键帧位置
// 参数: nPort,通道号
//		 nValue,当前位置,可以是时间或帧号,类型由nType指定.
//		 nType,指定nValue的类型.如果nType是BY_FRAMENUM,则nValue表示帧号;
//			如果nType是BY_FRAMTIME,则nValue表示时间,单位ms.
//		 pFramePos,查找到的关键帧的文件位置信息结构指针,详见PFRAME_POS.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetNextKeyFramePos(LONG nPort,DWORD nValue, DWORD nType, PFRAME_POS pFramePos);

//------------------------------------------------------------------------
// 函数: PLAY_SetDecCBStream
// 描述: 设置解码回调流类型,在PLAY_Play之前调用.
// 参数: nPort,通道号
//		 nStream,1 视频流;2 音频流;3 复合流.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetDecCBStream(LONG nPort,DWORD nStream);

//------------------------------------------------------------------------
// 函数: PLAY_SetDisplayRegion
// 描述: 设置或增加显示区域,可以做局部放大显示.
// 参数: nPort,通道号
//		 nRegionNum,显示区域序号,0~(MAX_DISPLAY_WND-1),如果为0,则将设置的区域显示在主窗口中.
//		 pSrcRect,局部显示区域
//		 hDestWnd,显示窗口句柄
//		 bEnable,打开(设置)或关闭显示区域
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetDisplayRegion(LONG nPort,DWORD nRegionNum, DISPLAYRECT *pSrcRect, HWND hDestWnd, BOOL bEnable);


//------------------------------------------------------------------------
// 函数: PLAY_GetRefValue
// 描述: 获取文件索引,以便下次打开同一个文件时直接使用这个信息.必须在索引建成后才能获得信息.
// 参数: nPort,通道号
//		 pBuffer,索引信息
//		 pSize,输入pBuffer的大小,输出索引信息大小.可以在第一次指定pSize=0,pBuffer=NULL,从
//			pSize的返回值获得需要的缓冲区大小.然后分配足够的缓冲,再调用一次.	 
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetRefValue(LONG nPort,BYTE *pBuffer, DWORD *pSize);

//------------------------------------------------------------------------
// 函数: PLAY_SetRefValue
// 描述: 设置文件索引,索引信息及其长度必须准确.如果已经有了文件索引信息,可以
//			不再调用生成索引的回调函数PLAY_SetFileRefCallBackEx,直接输入索引信息.
// 参数: nPort,通道号
//		 pBuffer,索引信息
//		 nSize,索引信息的长度
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetRefValue(LONG nPort,BYTE *pBuffer, DWORD nSize);

//------------------------------------------------------------------------
// 函数: PLAY_RigisterDrawFun
// 描述: 注册一个回调函数,获得当前表面的device context,你可以在这个DC上画图(或文字),
//			就好像在窗口的客户区DC上绘图,但这个DC不是窗口客户区的DC,而是DirectDraw里
//			的Off-Screen表面的DC.注意,如果是使用overlay表面,这个接口无效,可以直接在
//			窗口上绘图,只要不是透明色就不会被覆盖.
// 参数: nPort,通道号
//		 DrawFun,画图回调函数,其参数含义如下:
//			nPort,通道号
//			hDc,OffScreen表面设备上下文.
//			pUserData,用户数据,就是上面输入的用户数据
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK* fDrawCBFun)(LONG nPort,HDC hDc, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_RigisterDrawFun(LONG nPort, fDrawCBFun DrawCBFun, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_ResetBuffer
// 描述: 清空指定缓冲区的剩余数据
// 参数: nPort,通道号
//		 nBufType,如下:
//			BUF_VIDEO_SRC 1 视频源缓冲
//			BUF_AUDIO_SRC 2 音频源缓冲
//			BUF_VIDEO_RENDER 3 解码后视频数据缓冲
//			BUF_AUDIO_RENDER 4 解码后音频数据缓冲
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_ResetBuffer(LONG nPort,DWORD nBufType);

//------------------------------------------------------------------------
// 函数: PLAY_GetBufferValue
// 描述: 获取指定缓冲区的大小
// 参数: nPort,通道号
//		 nBufType,如下:
//			BUF_VIDEO_SRC 1 视频源缓冲
//			BUF_AUDIO_SRC 2 音频源缓冲
//			BUF_VIDEO_RENDER 3 解码后视频数据缓冲
//			BUF_AUDIO_RENDER 4 解码后音频数据缓冲
// 返回: DWORD,根据不同参数返回缓冲区值,源缓冲区返回byte,解码后缓冲区返回帧数.
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetBufferValue(LONG nPort,DWORD nBufType);

//------------------------------------------------------------------------
// 函数: PLAY_AdjustWaveAudio
// 描述: 调整WAVE波形,可以改变声音的大小.它和PLAY_SetVolume的不同在于,它是调整
//			声音数据,只对该路其作用,而PLAY_SetVolume是调整声卡音量,对整个系统起作用.
// 参数: nPort,通道号
//		 nCoefficient,调整的参数,范围从MIN_WAVE_COEF到MAX_WAVE_COEF,0是不调整
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_AdjustWaveAudio(LONG nPort,LONG nCoefficient);

//------------------------------------------------------------------------
// 函数: PLAY_SetAudioCallBack
// 描述: 音频解码后的WAVE数据回调.
// 参数: nPort,通道号
//		 funAudio,音频解码回调函数,其参数含义如下
//			nPort,通道号
//			pAudioBuf,wave格式音频数据
//			nSize,音频数据长度
//			nStamp,时标(ms)
//			nType,音频类型T_AUDIO16, 采样率8000，单声道，每个采样点16位表示
//			pUserData,用户自定义数据
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK * fAudioCBFun)(LONG nPort, char * pAudioBuf, LONG nSize, LONG nStamp, LONG nType, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetAudioCallBack(LONG nPort, fAudioCBFun AudioCBFun, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_SetEncTypeChangeCallBack
// 描述: 设置图像分辨率改变通知回调,打开文件前使用.
// 参数: nPort,通道号
//		 funEncChange,回调函数
//			nPort,通道号
//			pUserData,用户自定义数据
//		 pUserData,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void(CALLBACK *fEncChangeCBFun)(LONG nPort, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetEncTypeChangeCallBack(LONG nPort, fEncChangeCBFun EncChangeCBFun, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_SetEncTypeChangeCallBackEx
// 描述: 设置图像分辨率改变通知回调,打开文件前使用.
// 参数: nPort,通道号
//		 funEncChange,回调函数
//			nPort,通道号
//			pUserData,用户自定义数据
//		 pUserData,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void(CALLBACK *fEncChangeCBFunEx)(LONG nPort, void* pUserData,LONG nWidth, LONG nHeight);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetEncTypeChangeCallBackEx(LONG nPort, fEncChangeCBFunEx EncChangeCBFun, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_SetColor
// 描述: 设置视频参数
// 参数: nPort,通道号
//		 nRegionNum,显示区域,参考PLAY_SetDisplayRegion;如果只有一个显示区域(通常情况)设为0.
//		 nBrightness,亮度,默认64;范围0-128.
//		 nContrast,对比度,默认64;范围0-128.
//		 nSaturation,饱和度,默认64;范围0-128.
//		 nHue,色调,默认64;范围0-128.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetColor(LONG nPort, DWORD nRegionNum, int nBrightness, int nContrast, int nSaturation, int nHue);

//------------------------------------------------------------------------
// 函数: PLAY_GetColor
// 描述: 获取视频参数
// 参数: nPort,通道号
//		 nRegionNum,显示区域,参考PLAY_SetDisplayRegion;如果只有一个显示区域(通常情况)设为0.
//		 pBrightness,亮度,默认64;范围0-128.
//		 pContrast,对比度,默认64;范围0-128.
//		 pSaturation,饱和度,默认64;范围0-128.
//		 pHue,色调,默认64;范围0-128.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetColor(LONG nPort, DWORD nRegionNum, int *pBrightness, int *pContrast, int *pSaturation, int *pHue);

//------------------------------------------------------------------------
// 函数: PLAY_SetEncChangeMsg
// 描述: 设置分辨率改变通知消息
// 参数: nPort,通道号
//		 hWnd,消息发送的窗口
//		 nMsg,用户输入的消息,当解码时编码格式发生改变时用户在hWnd窗口过程中收到这个消息.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetEncChangeMsg(LONG nPort,HWND hWnd,UINT nMsg);

//------------------------------------------------------------------------
// 函数: PLAY_CatchPic
// 描述: 抓图,将BMP图片保存为指定的文件.PLAY_SetDisplayCallBack设置的视频数
//			据回调函数,只有在有视频数据解码出来时才调用,并由用户处理视频数
//			据(如抓图),如果不断有解码的数据,就不断调用这个回调函数.而PLAY_
//			CatchPic一次只抓一幅图,并能在暂停和单帧播放时实现抓图.建议:如果
//			用户想实现抓图(一次抓一幅图),调用PLAY_CatchPic,而如果想得到一段
//			时间内的视频数据,调用PLAY_SetDisplayCallBack.
// 参数: nPort,通道号
//		 sFileName,文件名称
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_CatchPic(LONG nPort,char* sFileName) ;

//------------------------------------------------------------------------
// 函数: PLAY_CatchPicEx
// 描述: PLAY_CatchPic的扩展接口,用法请参详PLAY_CatchPic.可指定图片格式
// 参数: nPort,通道号
//		 sFileName,文件名称
//		 ePicfomat,图片格式,见tPicFormats.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_CatchPicEx(LONG nPort,char* sFileName,tPicFormats ePicfomat);

//------------------------------------------------------------------------
// 函数: PLAY_SetFileEndCallBack
// 描述: 设置文件结束回调
// 参数: nPort,通道号
//		 pFileEnd,回调函数指针,其参数含义如下:
//			nPort,通道号
//			pUserData,用户自定义参数
//		 pUserData,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK *fpFileEndCBFun)(DWORD nPort, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetFileEndCallBack(LONG nPort, fpFileEndCBFun pFileEndCBFun, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_StartDataRecord
// 描述: 开始流数据录像,只对流模式有用,在PLAY_Play之后调用.
// 参数: nPort,通道号
//		 sFileName,录像文件名,如果文件名中有不存在的文件夹,就创建该文件夹.
//		 idataType,0表示原始视频流;1表示转换成AVI格式;2表示转换成ASF格式;3分段保存文件
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_StartDataRecord(LONG nPort, char *sFileName, int idataType);

//------------------------------------------------------------------------
// 函数: PLAY_WriteData
// 描述: 保存原始码流
// 参数: nPort,通道号
//		 pBuf,流数据
//		 nSize,大小
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_WriteData(LONG nPort, PBYTE pBuf,DWORD nSize);

//------------------------------------------------------------------------
// 函数: PLAY_StopDataRecord
// 描述: 停止流数据录像
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_StopDataRecord(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetPlaySpeed
// 描述: 改变图像播放的速度,
// 参数: nPort,通道号
//		 fCoff,播放速度,范围[1/64~64.0],小于1表示慢放，大于1表示正放.
//			   当播放速度较快时可能会抽帧播放.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetPlaySpeed(LONG nPort, float fCoff);

//------------------------------------------------------------------------
// 函数: PLAY_QueryInfo
// 描述: 目前实现了对当前时间和帧率信息的查询.
// 参数: nPort,通道号
//		 cmdType,指定状态查询指令,如下
//			PLAY_CMD_GetTime   获取编码中时间信息,单位ms.
//			PLAY_CMD_GetFileRate  获取帧率信息
//			PLAY_CMD_GetMediaInfo  获取媒体信息,信息结构为 MEDIA_INFO
//			PLAY_CMD_GetRenderTime  信息结构为tm，在<time.h>中定义
//		 buf,存放信息的缓冲
//		 buflen,缓冲长度
//		 returnlen,获取的信息的有效数据长度
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_QueryInfo(LONG nPort , int cmdType, char* buf, int buflen, int* returnlen);

//------------------------------------------------------------------------
// 函数: PLAY_OpenAudioRecord
// 描述: 打开音频采集功能.
// 参数: pProc,音频采集数据回调指针,其参数含义如下:
//			pDataBuffer,回调数据指针
//			DataLength,回调数据长度
//			pUserData,用户数据
//		 nBitsPerSample,表示每个采样所需要的位数
//		 nSamplesPerSec,采样率
//		 nLength,数据缓冲的长度
//		 nReserved,保留
//		 pUserData,用户自定义数据
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK *pCallFunction)(LPBYTE pDataBuffer, DWORD DataLength, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_OpenAudioRecord(pCallFunction pProc, LONG nBitsPerSample, LONG nSamplesPerSec, LONG nLength, LONG nReserved, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_CloseAudioRecord
// 描述: 关闭音频采集功能
// 参数: 
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_CloseAudioRecord();

//------------------------------------------------------------------------
// 函数: PLAY_SetWaterMarkCallBack
// 描述: 设置水印数据回调。注意：水印校验回调设置后将不会进行解码显示
// 参数: nPort,通道号
//		 pFunc,水印信息获取回调函数,其参数含义如下:
//			buf,水印数据buffer指针
//			key,区分不同水印信息
//			len,缓冲的最大长度
//			reallen,缓冲的实际长度
//			type,数值范围[0,3],含义如下
//				0  I帧帧数据水印信息
//				1  帧水印
//				2  水印校验 
//				3  智能分析帧
//		 pUserData,用户自定义参数 
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef int (CALLBACK* GetWaterMarkInfoCallbackFunc)(char* buf, LONG key, LONG len, LONG reallen, LONG type, void* pUserData);	
PLAYSDK_API BOOL CALLMETHOD PLAY_SetWaterMarkCallBack(LONG nPort, GetWaterMarkInfoCallbackFunc pFunc, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_CreateFile
// 描述: 打开文件并自动分配通道号
// 参数: sFileName,文件名
// 返回: DWORD,失败返回0,否则返回通道号.
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_CreateFile(LPSTR sFileName);

//------------------------------------------------------------------------
// 函数: PLAY_DestroyFile
// 描述: 关闭播放文件并释放自动分配的通道号.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_DestroyFile(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_CreateStream
// 描述: 打开流接口并自动分配通道号
// 参数: nBufPoolSize,置播放器中存放数据流的缓冲区大小,范围是[SOURCE_BUF_MIN,SOURCE_BUF_MAX].
// 返回: DWORD,失败返回0,否则返回通道号.
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_CreateStream(DWORD nBufPoolSize);

//------------------------------------------------------------------------
// 函数: PLAY_DestroyStream
// 描述: 关闭数据流,并释放自动分配的通道号.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_DestroyStream(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetFreePort
// 描述: 获取空闲通道号,上限为501.与PLAY_ReleasePort匹对使用.
// 参数: plPort,输出参数,返回获取的通道号.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetFreePort(LONG *plPort);

//------------------------------------------------------------------------
// 函数: PLAY_ReleasePort
// 描述: 释放通道号,与PLAY_RealsePort匹对使用.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_ReleasePort(LONG lPort);

//------------------------------------------------------------------------
// 函数: PLAY_VerticalSyncEnable
// 描述: 垂直同步显示开关.此接口需在PLAY_Play之后调用,重新播放时需重新设置.
//			在播放动态图像出现断层时,可以使用此接口打开垂直同步功能.
// 参数: nPort,通道号
//		 bEnable,TRUE打开垂直同步;FALSE关闭垂直同步.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API	BOOL CALLMETHOD PLAY_VerticalSyncEnable(LONG nPort, BOOL bEnable);

//------------------------------------------------------------------------
// 函数: PLAY_GetPicBMP
// 描述: 抓取BMP图像
// 参数: nPort,通道号
//		 pBmpBuf,存放BMP图像数据的缓冲地址,由用户分配,不得小于bmp 图像大小,
//			推荐大小:sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+w*h*4,
//			其中w和h分别为图像宽高.
//		 dwBufSize,缓冲区大小
//		 pBmpSize,获取到的实际bmp 图像大小
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetPicBMP(LONG nPort, PBYTE pBmpBuf, DWORD dwBufSize, DWORD* pBmpSize);

//------------------------------------------------------------------------
// 函数: PLAY_GetPicBMPEx
// 描述: 抓取BMP图像
// 参数: nPort,通道号
//		 pBmpBuf,存放BMP图像数据的缓冲地址,由用户分配,不得小于bmp 图像大小,
//			推荐大小:sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+w*h*4,
//			其中w和h分别为图像宽高.
//		 dwBufSize,缓冲区大小
//		 pBmpSize,获取到的实际bmp 图像大小
//		 nWidth,指定的bmp的宽
//		 nHeight,指定的bmp的高
//		 nRgbType,指定RGB格式 0：RGB32;1：RGB24;
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
PLAYSDK_API BOOL CALLMETHOD PLAY_GetPicBMPEx(LONG nPort, PBYTE pBmpBuf, DWORD dwBufSize, DWORD* pBmpSize, LONG nWidth, LONG nHeight, int nRgbType);

//------------------------------------------------------------------------
// 函数: PLAY_GetPicJPEG
// 描述: 抓取JPEG图像.
// 参数: nPort,通道号
//		 pJpegBuf,存放JPEG图像数据的缓冲地址,由用户分配,不得小于JPEG图像大小,
//			推荐大小:w*h*3/2,其中w和h分别为图像宽高.
//		 dwBufSize,缓冲区大小.
//		 pJpegSize,获取到的实际JPEG图像大小.
//		 quality,JPEG图像的压缩质量,取值范围为(0,100].
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetPicJPEG(LONG nPort, PBYTE pJpegBuf, DWORD dwBufSize, DWORD* pJpegSize, int quality);

//------------------------------------------------------------------------
// 函数: PLAY_GetPicTIFF
// 描述: 抓取TIFF图像
// 参数: nPort,通道号
//		 pTiffBuf,存放TIFF图像数据的缓冲地址,由用户分配,不得小于tiff图像大小,
//			推荐大小:w*h*3+1024,
//			其中w和h分别为图像宽高.
//		 dwBufSize,缓冲区大小
//		 pTiffSize,获取到的实际TIFF图像大小
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetPicTIFF(LONG nPort, PBYTE pTiffBuf, DWORD dwBufSize, DWORD* pTiffSize);

//------------------------------------------------------------------------
// 函数: PLAY_SetVisibleDecodeCallBack
// 描述: 解码回调.与PLAY_SetDecodeCallBack基本相同,不同的是解码回调的同时可以
//			显示视频,建议不要在回调函数里面做长时间的逻辑处理,以免增加显示的延时.
// 参数: nPort,通道号
//		 cbDec,解码回调函数指针,不能为NULL.回调函数参数含义如下:
//			nPort,通道号
//			pFrameDecodeInfo,解码后的音视频数据
//			pFrameInfo,图像和声音信息,请参见FRAME_INFO结构体
//			pUser,用户自定义参数
//		 pUser,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
PLAYSDK_API BOOL CALLMETHOD PLAY_SetVisibleDecodeCallBack(LONG nPort, fCBDecode cbDec, void* pUser);

//------------------------------------------------------------------------
// 函数: PLAY_RigisterDrawFunEx
// 描述: 注册一个回调函数,获得当前表面的上下文(HDC),这个DC不是窗口客户区的DC,
//			而是DirectDraw里的Off-Screen表面的DC.注意.如果是使用overlay表面,
//			这个接口无效,overlay方式可以直接在窗口上绘图,只要不是透明色就不会被覆盖.
// 参数: nPort,通道号
//		 nReginNum,显示区域序号,范围[0,(MAX_DISPLAY_WND-1)].如果nRegionNum为0,
//			则将设置的区域显示在主窗口中.
//		 DrawFunEx,画图回调函数,其参数含义如下:
//			nPort,通道号
//			hDc,Off-Screen表面的设备上下文(DC)
//			pUserData,用户自定义参数
//		 pUserData,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK* fDrawCBFunEx)(LONG nPort,LONG nReginNum,HDC hDc, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_RigisterDrawFunEx(LONG nPort, LONG nReginNum, fDrawCBFunEx DrawFunEx, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_CatchResizePic
// 描述: 抓图,可选择格式并指定宽高.
// 参数: nPort,通道号
//		 sFileName,文件名称
//		 lTargetWidth,指定的图像宽度
//		 lTargetHeight,指定的图像高度
//		 ePicfomat,图片格式,详见tPicFomats
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_CatchResizePic(LONG nPort, char* sFileName, LONG lTargetWidth, LONG lTargetHeight, tPicFormats ePicfomat);

//------------------------------------------------------------------------
// 函数: PLAY_GetRealFrameBitRate
// 描述: 获取视频实时码率
// 参数: nPort,通道号
//		 pBitRate,输出参数,返回视频码率.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetRealFrameBitRate(LONG nPort, double* pBitRate);


//------------------------------------------------------------------------
// 函数: PLAY_SetFileRefCallBackEx
// 描述: 设置建立索引回调,以返回索引创建情况.
// 参数: nPort,通道号
//		 pFileRefDoneEx,回调函数指针,其参数含义如下:
//			nPort,通道号
//			bIndexCreated,索引创建标志,TRUE索引创建成功;FALSE失败.
//			pUserData,用户自定义参数
//		 pUserData,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK *fpFileRefDoneCBFunEx)(DWORD nPort, BOOL bIndexCreated, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetFileRefCallBackEx(LONG nPort, fpFileRefDoneCBFunEx pFileRefDoneCBFunEx, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_StartAVIConvert
// 描述: 开始AVI转换并设置AVI转换状态回调.
// 参数: nPort,通道号
//		 pAVIFunc,回调函数,其参数含义如下:
//			nPort,通道号
//			lMediaChangeType,AVI_MEDIACHANGE_FRAMERATE表示帧率改变;AVI_MEDIACHANGE_RESOLUTION表示分辨率改变
//			lUser,用户自定义参数
//			pIsNewFile,TRUE 分段转换;FALSE 不分段；默认不分段
//			sNewFileName,如果分段转换,新的录像文件名
//		 lUser,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK* AVIConvertCallback)(LONG nPort, LONG lMediaChangeType, void* pUserData, BOOL *pIsNewFile, char *sNewFileName);
PLAYSDK_API BOOL CALLMETHOD PLAY_StartAVIConvert(LONG nPort, char *sFileName, AVIConvertCallback pAVIFunc, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_StopAVIConvert
// 描述: 停止AVI转换.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_StopAVIConvert(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetWaterMarkCallBackEx
// 描述: 设置水印数据回调。注意：水印校验回调设置后将不会进行解码显示
// 参数: nPort,通道号
//		 pFunc,函数指针,其参数含义如下:
//			nPort,通道号
//			buf,水印数据缓冲
//			lTimeStamp,水印的时间戳
//			lInfoType,水印信息类型,有三种类型,WATERMARK_DATA_TEXT,
//				WATERMARK_DATA_JPEG_BMP,WATERMARK_DATA_FRAMEDATA
//		 len,缓冲的最大长度
//		 reallen,缓冲的实际长度
//		 lCheckResult,1没有错误;2水印错误;3帧数据错误;4帧号错误
//		 pUserData,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef int (CALLBACK* GetWaterMarkInfoCallbackFuncEx)(LONG nPort, char* buf, LONG lTimeStamp, LONG lInfoType, LONG len, LONG reallen, LONG lCheckResult, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetWaterMarkCallBackEx(LONG nPort, GetWaterMarkInfoCallbackFuncEx pFunc, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_SetAudioRecScaling
// 描述: 设置音频采集数据缩放比例
// 参数: fRatio,缩放比例.大于0小于1为音频缩小;1为原始音频;大于1为音频放大.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetAudioRecScaling(float fRatio);

//------------------------------------------------------------------------
// 函数: PLAY_GetAudioRecScaling
// 描述: 获取音频采集数据缩放比例
// 参数: pfRatio,缩放比例.大于0小于1为音频缩小;1为原始音频;大于1为音频放大.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetAudioRecScaling(float* pfRatio);

//------------------------------------------------------------------------
// 函数: PLAY_SetAudioRenderScaling
// 描述: 设置音频解码缩放比例
// 参数: nPort,通道号
//		 fRatio,缩放比例.大于0小于1为音频缩小;1为原始音频;大于1为音频放大.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetAudioRenderScaling(LONG nPort, float fRatio);

//------------------------------------------------------------------------
// 函数: PLAY_GetAudioRenderScaling
// 描述: 获取音频解码缩放比例
// 参数: nPort,通道号
//		 pfRatio,缩放比例.大于0小于1为音频缩小;1为原始音频;大于1为音频放大.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetAudioRenderScaling(LONG nPort, float* pfRatio);

//------------------------------------------------------------------------
// 函数: PLAY_SetRotateAngle
// 描述: 图像翻转
// 参数: nrotateType,翻转类型,范围[0,3]. 0不旋转;1旋转90度;2旋转180度;3旋转270度.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetRotateAngle(LONG nPort , int nrotateType);

//------------------------------------------------------------------------
// 函数: PLAY_SetDelayTime
// 描述: 设置延迟时间
// 参数: nDelay(ms),延迟时间，缓冲多少时间开始播放，缓冲小于此值开始稍微慢放
//       nThreshold(ms),阀值时间，到达阀值开始稍微快放
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetDelayTime(LONG nPort, int nDelay, int nThreshold);

//------------------------------------------------------------------------
// 函数: PLAY_SetPlayMethod
// 描述: 设置播放策略参数，只对实时流有效
// 参数: nStartTime,开始播放的时间(ms)
//       nSlowTime,开始慢放的时间(ms)
//		 nFastTime,开始快放的时间(ms)
//		 nFailedTime,超过此时间投递数据失败(ms)
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetPlayMethod(LONG nPort, int nStartTime, int nSlowTime, int nFastTime, int nFailedTime);

//------------------------------------------------------------------------
// 函数: PLAY_BackOne
// 描述: 与PLAY_OneByOneBack重复，请使用PLAY_OneByOneBack
// 参数: 
// 返回: 
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_BackOne(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetDecCallBack(建议使用PLAY_SetDecodeCallBack)
// 描述: 设置解码回调,替换播放器中的显示部分,由用户自己控制显示,该函数在
//			PLAY_Play之前调用,在PLAY_Stop时自动失效,下次调用PLAY_Play之前
//			需要重新设置.解码部分不控制速度,只要用户从回调函数中返回,解码器
//			就会解码下一部分数据.适用于只解码不显示的情形。
// 参数: nPort,通道号
//		 DecCBFun,解码回调函数指针,不能为NULL.回调函数参数含义如下:
//			nPort,通道号
//			pBuf,解码后的音视频数据
//			nSize,解码后的音视频数据pBuf的长度
//			pFrameInfo,图像和声音信息,请参见FRAME_INFO结构体
//			nReserved1,保留参数
//			nReserved2,保留参数	
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK* fDecCBFun)(LONG nPort,char * pBuf,LONG nSize,FRAME_INFO * pFrameInfo, void* pUserData, LONG nReserved2);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetDecCallBack(LONG nPort, fDecCBFun DecCBFun);

//------------------------------------------------------------------------
// 函数: PLAY_SetDecCallBackEx(建议使用PLAY_SetDecodeCallBack)
// 描述: 设置解码回调,替换播放器中的显示部分,由用户自己控制显示,该函数在
//			PLAY_Play之前调用,在PLAY_Stop时自动失效,下次调用PLAY_Play之前
//			需要重新设置.解码部分不控制速度,只要用户从回调函数中返回,解码器
//			就会解码下一部分数据.适用于只解码不显示的情形。
// 参数: nPort,通道号
//		 DecCBFun,解码回调函数指针,不能为NULL.回调函数参数含义如下:
//			nPort,通道号
//			pBuf,解码后的音视频数据
//			nSize,解码后的音视频数据pBuf的长度
//			pFrameInfo,图像和声音信息,请参见FRAME_INFO结构体
//			nReserved1,用户自定义参数
//			nReserved2,保留参数	
//		 pUserData,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetDecCallBackEx(LONG nPort, fDecCBFun DecCBFun, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_SetVisibleDecCallBack(建议使用PLAY_SetVisibleDecodeCallBack)
// 描述: 解码回调.与PLAY_SetDecCallBackEx基本相同,不同的是解码回调的同时可以
//			显示视频,建议不要在回调函数里面做长时间的逻辑处理,以免增加显示的延时.
// 参数: nPort,通道号
//		 DecCBFun,回调函数指针,不能为NULL,其参数含义如下:
//			nPort,通道号
//			pBuf,解码后的音视频数据
//			nSize,数据长度
//			pFrameInfo,帧信息,详见FRAME_INFO结构体.
//			pUserData,用户自定义参数
//			pReserved1,保留参数.
//		 pUserData,用户自定义参数.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK* fVisibleDecCBFun)(LONG nPort,char * pBuf,LONG nSize,FRAME_INFO * pFrameInfo, void* pUserData, LONG nReserved1);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetVisibleDecCallBack(LONG nPort, fVisibleDecCBFun DecCBFun, void* pUserData);

//------------------------------------------------------------------------
//	函数名: PLAY_SetSecurityKey
//	描述: 设置aes解密密钥
//  输入参数:   nPort:		解码通道。
//				szKey:		密钥的指针
//				nKeylen:	密钥的长度       
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetSecurityKey(LONG nPort,const char* szKey,DWORD nKeylen);

//------------------------------------------------------------------------
// 函数: PLAY_StartFisheye
// 描述: 开启视频鱼眼算法功能，需要包含fisheye.dll库
// 参数: [in]nPort : 通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_StartFisheye(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_OperateFisheyeParams
// 描述: 设置/获取鱼眼参数
// 参数: [in]nPort					:  通道号
//		 [in]operatetype			:  操作类型
//       [in]pOptParam	    		:  鱼眼参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//-----------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_OptFisheyeParams(LONG nPort, FISHEYE_OPERATETYPE operatetype, FISHEYE_OPTPARAM* pOptParam);

//------------------------------------------------------------------------
// 函数: PLAY_FisheyeSecondRegion
// 描述: 用于浮动模式下开启或关闭第二个鱼眼窗口
// 参数: [in]nPort			: 通道号
//		 [in]hDestWnd		: 显示窗口句柄
//		 [in]pOptParam		: 第二个窗口对应的鱼眼参数
//		 [in]bEnable		: 打开或关闭显示区域
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_FisheyeSecondRegion(LONG nPort, HWND hDestWnd, FISHEYE_OPTPARAM* pOptParam, BOOL bEnable);

//------------------------------------------------------------------------
// 函数: PLAY_FisheyeEptzUpdate
// 描述: 开启eptz(电子云台），进行缩放移动
// 参数: [in]nPort           :  通道号
//       [in/out]pEptzParam  :  调节参数
//		 [in]bSecondRegion	 ：	是否为浮动模式下的第二个窗口，1为真，默认填0
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_FisheyeEptzUpdate(LONG nPort, FISHEYE_EPTZPARAM* pEptzParam, BOOL bSecondRegion);

//------------------------------------------------------------------------
// 函数: PLAY_StopFisheye
// 描述: 停止视频鱼眼算法功能
// 参数: [in]nPort : 通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_StopFisheye(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetFishEyeInfoCallBack
// 描述: 设置获取鱼眼回调
// 参数: nPort,通道号
//		 pFishEyeInfoFun,回调函数指针,不能为NULL,其参数含义如下:
//			nPort,通道号
//          byCorrectMode,矫正模式
//          wRadius,半径
//          wCircleX,圆心横坐标
//          wCircleY,圆心纵坐标
//			widthRatio, 宽比率
//			heightRatio,高比率
//			gain, 增益
//			denoiseLevel, 降噪等级
//			InstallStyle, 鱼眼安装方式
//		 pUserData,用户自定义参数.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK* fCBFishEyeInfoFun)( 
	LONG nPort,
	BYTE byCorrectMode,
	WORD wRadius,
	WORD wCircleX,
	WORD wCircleY,
	UINT widthRatio,
	UINT heightRatio,
	BYTE gain,
	BYTE denoiseLevel,
	BYTE installStyle,
	void* pUserData );
PLAYSDK_API BOOL CALLMETHOD PLAY_SetFishEyeInfoCallBack(
	LONG nPort, 
	fCBFishEyeInfoFun pFishEyeInfoFun, 
	void* pUserData );

typedef enum
{
	RENDER_NOTSET = 0,
	RENDER_GDI,
	RENDER_X11 = RENDER_GDI,
	RENDER_D3D,
	RENDER_OPENGL = RENDER_D3D,
	RENDER_DDRAW
}RenderType;

typedef enum
{
	DECODE_NOTSET = 0,
	DECODE_SW,
	DECODE_HW,		//拷贝模式
	DECODE_HW_FAST, //直接显示模式
	DECODE_MSDK
}DecodeType;
/************************************************************************/
/* 函数: PLAY_SetEngine
   描述: 指定解码器(Windows平台), PLAY_Play之前调用有效
   参数: nPort,通道号
	     decodeType, 解码模式（仅限于H264)
	     renderType, 渲染模式
   返回: BOOL,成功返回TRUE,失败返回FALSE.   
   备注：如果单一设置其中一个Engine，可以将另一个传入NONE,
         例如:PLAY_SetEngine(0, DECODE_MSDK, RENDER_NONE);         */
/************************************************************************/
PLAYSDK_API BOOL CALLMETHOD PLAY_SetEngine(LONG nPort,
										   DecodeType decodeType, 
										   RenderType renderType);

/************************************************************************/
/* 函数: PLAY_InitDisk
   描述: 初始化硬盘(Windows32平台)
   参数:
   返回: UINT,成功返回硬盘数量， 失败返回0。   
 */
/************************************************************************/
PLAYSDK_API UINT CALLMETHOD PLAY_InitDisk();

/************************************************************************/
/* 函数: PLAY_QueryFileList
   描述: 文件查询与定位(Windows32平台)
 */
/************************************************************************/
PLAYSDK_API UINT CALLMETHOD PLAY_QueryFileList(UINT nChannel,DHTIME start_time,DHTIME end_time,UINT *pmax_ret_num,pFILE_INFO pfile_info,UINT type, BYTE hint, UINT driver_type);

/************************************************************************/
/* 函数: PLAY_FormatDisk
   描述: 格式化硬盘(Windows32平台)
   参数: disk_no,要读取分区信息的磁盘号
   返回: BOOL,成功返回TRUE,失败返回FALSE.
*/
/************************************************************************/
typedef enum
{
	FORMAT_ALL_DATA, //清除所有数据
	FORMAT_KEY_DATA  //清除关键区数据
}FormatType;
PLAYSDK_API BOOL CALLMETHOD PLAY_FormatDisk(UINT disk_no, FormatType type);

/***********************************************************************/
/* 函数: PLAY_SetPercentCallBack
   描述: 进度回调, 可以设定文件开始结束时间
   参数: [nStartTime,nEndTime]: 文件开始结束时间, 单位为从1970/1/1开始经过的秒数,
		 如果指定, 需要PLAY_SetFileRefCallBack回调后才有效, 
		 [0,0] 表示整个文件大小,不需要建立索引,在PLAY_OpenFile之后调用
		 nPercent: 进度百分比, -1表示异常
*/
/************************************************************************/
typedef void (CALLBACK *fPercentCallbackFunc)(LONG nPort, int nPercent, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetPercentCallBack(LONG nPort, LONG nStartTime, LONG nEndTime, fPercentCallbackFunc pFunCallback, void* pUserData);

/************************************************************************/
/* 函数: PLAY_SetGPSCallBack
   描述: GPS信息获取函数
   参数: 
*/
/************************************************************************/
typedef int (CALLBACK *fGPSInfoCallbackFunc)(char* pBuf, LONG nLen, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetGPSCallBack(LONG nPort, fGPSInfoCallbackFunc pFunc, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_SetDemuxCallBack
// 描述: 源数据分析完的数据回调
// 参数: nPort,通道号
//		 DecCBFun,分析数据回调指针
//			nPort,通道号
//			pBuf,数据指针
//			nSize,数据长度
//			pMutexInfo, 帧信息,指向DEMUX_INFO结构
//			pMutexInfoEx, 帧数据或裸数据，指向DemuInfoEx结构
//			pUserData,用户自定义数据
//		 pUserData,用户自定义数据
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK* fDemuxDecCBFun)(LONG nPort,char * pBuf,	LONG nSize,void * pMutexInfo,void* pMutexInfoEx, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetDemuxCallBack(LONG nPort, fDemuxDecCBFun DecCBFun, void* pUserData);

#ifdef __cplusplus
}
#endif

#endif
