// InputWin32.h : InputWin32 DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
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

// ��ʼ�� Listenter Ϊtypedef void (*OnEchoReportListener)(int Echo, unsigned char* Data, int DataLen); �������ݾ��ɸûص��ش�
// Data ���� DataLen���ݳ���
// Echo Ӧ��ֵ 

/*State ��������״̬
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
	unsigned char Send;                 // 0:����2015 1:�Ǳ�����ʯ 2: �Ǳ�����ʯ 3:�б�2011
	unsigned char Num;                  ������
	unsigned char Tan;                  
	unsigned char NameLen;              ���ֳ���
	ZONE_DATE_TIME Time;                ʱ��
	unsigned char Name[INFO_NAME_NUM];  ����
}INFO_HEAD, *PINFO_HEAD;

typedef struct
{
	unsigned char Param[16 * MAX_INFO_NUM]; ����
	unsigned char TParam[MAX_INFO_NUM];     ̼�� 0xFF = /  TParam/4*0.5 = ̼��ֵ
}INFO_DATA, *PINFO_DATA;

*/
// com ���ں� Listenter�ص�
extern "C" _declspec(dllexport) void InitFrameProcessor(int com, OnReportListener Proc);
extern "C" _declspec(dllexport) void DeInitFrameProcessor();
extern "C" _declspec(dllexport) void OpenCom(int com);