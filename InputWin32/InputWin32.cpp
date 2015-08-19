// InputWin32.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "InputWin32.h"

FrameProcessor* mFrameProcessor;
OnReportListener mOnEchoReportListener = NULL;
unsigned char * mRecvBuf = NULL;

static void EchoReportListener(int Echo, unsigned char* Data, int DataLen)
{
	if (NULL != mOnEchoReportListener)
	{
		mOnEchoReportListener(Echo, Data, DataLen);
	}
}


void InitFrameProcessor(int com, OnReportListener Proc)
{
	mFrameProcessor = new FrameProcessor(com, EchoReportListener);
	mOnEchoReportListener = Proc;
}

void OpenCom(int com)
{
	mFrameProcessor->FrameComOpen(com);
}

void DeInitFrameProcessor()
{
	mFrameProcessor->FrameProcessorStop();
	if (mFrameProcessor)
	{
		//delete[] mFrameProcessor;
		mFrameProcessor = NULL;
	}
}