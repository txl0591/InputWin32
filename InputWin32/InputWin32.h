// InputWin32.h : InputWin32 DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "FrameTypeDef.h"
#include "QueueSend.h"
#include "FrameData.h"
#include "FrameRecv.h"
#include "FrameSend.h"
#include "FrameProcessor.h"

typedef void(__stdcall *OnReportListener)(int Echo, unsigned char* Data, int DataLen);

// 初始化 Listenter 为typedef void (*OnEchoReportListener)(int Echo, unsigned char* Data, int DataLen); 所有数据均由该回调回传
// Data 数据 DataLen数据长度
// Echo 应答值 

/*State 操作返回状态
#define INPUT_COM_ERROR      0x00
#define INPUT_READ_ERROR	 0x01

typedef struct _ZONE_DATE_TIME_
{
	unsigned int year;
	unsigned char  month;
	unsigned char  day;
	unsigned char  hour;
	unsigned char  min;
	unsigned char  sec;
	unsigned char  week;
}ZONE_DATE_TIME, *PZONE_DATE_TIME;

typedef struct
{
	unsigned char Send;                 // 0:福建2015 1:非泵送碎石 2: 非泵送卵石 3:行标2011
	unsigned char Num;                  测区数
	unsigned char Tan;                  
	unsigned char NameLen;              名字长度
	ZONE_DATE_TIME Time;                时间
	unsigned char Name[INFO_NAME_NUM];  名字
}INFO_HEAD, *PINFO_HEAD;

typedef struct
{
	unsigned char Param[16 * MAX_INFO_NUM]; 测区
	unsigned char TParam[MAX_INFO_NUM];     碳化 0xFF = /  TParam/4*0.5 = 碳化值
}INFO_DATA, *PINFO_DATA;

*/
// com 串口号 Listenter回调
extern "C" _declspec(dllexport) void InitFrameProcessor(int com, OnReportListener Proc);
extern "C" _declspec(dllexport) void DeInitFrameProcessor();
extern "C" _declspec(dllexport) void OpenCom(int com);