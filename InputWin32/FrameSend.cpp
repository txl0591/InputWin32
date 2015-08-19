#include "stdafx.h"
#include <stdlib.h>
#include <windows.h>
#include <iomanip>
#include <iostream>
#include "FrameTypeDef.h"
#include "QueueSend.h"
#include "FrameData.h"
#include "FrameSend.h"

using namespace std;

static DWORD WINAPI SendThread(LPVOID Param);

FrameSend::FrameSend()
{
	mThreadRun = true;	
	mFrameCom = NULL;
	mThreadSend = CreateThread(NULL, 0, SendThread, this, 0, &mThreadSendID); 
}

FrameSend::~FrameSend()
{
	mThreadRun = false;	
	if(mThreadSend != NULL)
	{
		CloseHandle(mThreadSend);
		mThreadSend = NULL;
	}

	if(!mSendQueue.empty())
	{
		mSendQueue.pop();
	}
}

void FrameSend::FrameSendStop()
{
	mThreadRun = false;	
	if(mThreadSend != NULL)
	{
		CloseHandle(mThreadSend);
		mThreadSend = NULL;
	}
	
	if(!mSendQueue.empty())
	{
		mSendQueue.pop();
	}
}

void FrameSend::SendAck(int FrameCmd,unsigned char* buffer, int bufferlen)
{
	SendItem Item;
	memset(&Item,0,sizeof(SendItem));
	Item.bufferlen = SerialDataPack(Item.buffer, FrameCmd, FRAME_SEND, ACK_NEED, buffer,bufferlen);
	PushSendItem(&Item);
}

void FrameSend::SendNoAck(int FrameCmd,unsigned char* buffer, int bufferlen)
{
	SendItem Item;
	memset(&Item,0,sizeof(SendItem));
	Item.bufferlen = SerialDataPack(Item.buffer, FrameCmd, FRAME_SEND, ACK_NONEED, buffer,bufferlen);
	PushSendItem(&Item);
}

void FrameSend::SendEcho(int FrameCmd,unsigned char* buffer, int bufferlen)
{
	SendItem Item;
	memset(&Item,0,sizeof(SendItem));
	Item.bufferlen = SerialDataPack(Item.buffer, FrameCmd, FRAME_ACK, ACK_NONEED, buffer,bufferlen);
	PushSendItem(&Item);
}


SendItem* FrameSend::PullSendItem()
{
	SendItem *mItem = NULL;
	SendItem *nItem = NULL;
	mMutex.Lock();
	if(!mSendQueue.empty())
	{
		mItem = &mSendQueue.front();
		nItem = (SendItem*)malloc(sizeof(SendItem));
		memcpy(nItem,mItem,sizeof(SendItem));
		mSendQueue.pop();
	}
	mMutex.Unlock();
	return nItem;
}

void FrameSend::PushSendItem(SendItem* Item)
{
	mMutex.Lock();
	mSendQueue.push(*Item);
	mMutex.Unlock();
}

bool FrameSend::SendCom(unsigned char* buffer, int bufferlen)
{
	if(mFrameCom != NULL)
	{
		return mFrameCom->FrameComSend(buffer,bufferlen);
	}

	return FALSE;
}

void FrameSend::SendReportListener(int echo)
{
	if (ProcSend != NULL){
		ProcSend(Param, FALSE);
	}
}


static DWORD WINAPI SendThread(LPVOID Param)
{
	FrameSend* nFrameSend = (FrameSend*)Param;
	while(nFrameSend->IsThreadRun())
	{
		SendItem* Item = nFrameSend->PullSendItem();
		if(Item)
		{
			if (FALSE == nFrameSend->SendCom(Item->buffer, Item->bufferlen))
			{
				nFrameSend->SendReportListener(FALSE);
			}
			free(Item);
			Item = NULL;
		}
		Sleep(20);
	}
	return 0;
}