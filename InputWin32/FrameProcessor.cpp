#include "stdafx.h"
#include <stdlib.h>
#include <windows.h>
#include <iomanip>
#include <iostream>
#include<string.h>
#include <objbase.h>
#include <initguid.h>
#include <Setupapi.h>
#include "FrameTypeDef.h"
#include "QueueSend.h"
#include "FrameData.h"
#include "FrameRecv.h"
#include "FrameSend.h"
#include "FrameProcessor.h"

using namespace std;

static void DistributeReport (void* mFrameProcessor,int Id, int Cmd, int Ack, unsigned char* Data, int DataLen);
static void ResponsionReport (void* mFrameProcessor,int Id, int Cmd, int Ack, unsigned char* Data, int DataLen);
static void SendReportListener(void* Param, int Echo);

FrameProcessor::FrameProcessor(int com, OnEchoReportListener Listener)
{
	mOnEchoReportListener = Listener;
	mParamMaxIndex = FRAME_RECV_DEFAULT;
	mParamNowIndex = 0;
	mNowID = FRAME_ID_NONE;
	if(0 == com)
	{
		mFrameCom.FrameComScan();
	}
	else
	{
		mFrameCom.SetFrameCom(com);
	}
	mFrameCom.FrameComOpen();
	mFrameSend.SetFrameCom(&mFrameCom);
	mFrameSend.SetSendReportListener(SendReportListener, this);
	mFrameRecv.SerialDataReportListener(DistributeReport, ResponsionReport, this);
	mFrameRecv.FrameRecvStart(&mFrameCom);	
}

FrameProcessor::~FrameProcessor()
{
	
}

void FrameProcessor::FrameProcessorIn(int Id, unsigned char* Data, int DataLen)
{
	if (mNowID == FRAME_ID_NONE)
	{
		mNowID = Id;
		mParamNowIndex = 1;
		memcpy(&mParam.mHead, Data, DataLen);
		mParamMaxIndex = mParam.mHead.Num + FRAME_RECV_DEFAULT - 1;
#if _DEBUG_
		mFrameCom.Printf(_T("1"), Data, DataLen);
		cout << "First Recv AllLen 0x" << setw(2) << setfill('0') << hex << (int)mParamMaxIndex<<endl;
		cout << "Send 0x" << setw(2) << setfill('0') << hex << (int)mParam.mHead.Send << endl;
		cout << "Num 0x" << setw(2) << setfill('0') << hex << (int)mParam.mHead.Num << endl;
		cout << "Tan 0x" << setw(2) << setfill('0') << hex << (int)mParam.mHead.Tan << endl;
		cout << "NameLen 0x" << setw(2) << setfill('0') << hex << (int)mParam.mHead.NameLen << endl;
#endif
	}
	else
	{
		if (Id != mNowID)
		{
#if _DEBUG_
			cout << "First ID Error [" << mNowID << "] != [" << Id << "]"<<endl;
#endif
			mNowID = Id;
			mParamNowIndex = 1;
			memcpy(&mParam.mHead, Data, DataLen);
			mParamMaxIndex = mParam.mHead.Num + FRAME_RECV_DEFAULT-1;

		}
		else
		{
			if ((mParamNowIndex+1) != mParamMaxIndex)
			{
				memcpy(&mParam.mData.Param[mParamNowIndex-1], Data, DataLen);
				mParamNowIndex++;
#if _DEBUG_
				cout << "Now ID [" << mNowID << "] Index [" << mParamNowIndex << "]"<<endl;
#endif
			}
			else
			{
				memcpy(&mParam.mData.TParam, Data, DataLen);
				setStateReport(ECHO_READ_SUCCESS, (unsigned char*)&mParam, sizeof(INFO_DATA));
#if _DEBUG_
				cout << " Recv End ID [" << mNowID << "]"<<endl;
#endif
				mNowID = FRAME_ID_NONE;
				mParamNowIndex = 1;
			}

		}
	}
}

static void DistributeReport (void* Param, int Id, int Cmd, int Ack, unsigned char* Data, int DataLen)
{
	FrameProcessor* mFrameProcessor = (FrameProcessor*)Param;
	switch (Cmd)
	{	
		case COMMAND_SENDPACK:
			if (mFrameProcessor != NULL){
				mFrameProcessor->FrameProcessorIn(Id, Data, DataLen);
			}
			break;
			
		default:
			break;
	}
}

static void ResponsionReport (void* Param,int Id, int Cmd, int Ack, unsigned char* Data, int DataLen)
{
	FrameProcessor* mFrameProcessor = (FrameProcessor*)Param;
	
	if(NULL == mFrameProcessor)
	{
		return;	
	}

	switch (Cmd)
	{
		case COMMAND_SENDPACK:
			break;

		default:
			break;
	}
}

static void SendReportListener(void* Param, int Echo)
{
	FrameProcessor* mFrameProcessor = (FrameProcessor*)Param;

	if (NULL == mFrameProcessor)
	{
		return;
	}
	mFrameProcessor->setStateReport(ECHO_COM_ERROR, NULL, 0);
}

void FrameProcessor::setStateReport(int Echo, unsigned char* Data, int DataLen)
{
	if(mOnEchoReportListener != NULL){
		mOnEchoReportListener(Echo, Data, DataLen);
	}
}

void FrameProcessor::FrameProcessorStop()
{	
	mFrameSend.FrameSendStop();
	mFrameRecv.FrameRecvStop();
	mFrameCom.FrameComClose();
}

void FrameProcessor::FrameComOpen(int com)
{
	mFrameCom.SetFrameCom(com);
	mFrameCom.FrameComOpen();
}