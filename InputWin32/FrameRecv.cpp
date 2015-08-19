#include "stdafx.h"
#include <iostream>
#include "FrameTypeDef.h"
#include "QueueSend.h"
#include "FrameData.h"
#include "FrameRecv.h"

using namespace std;

static DWORD WINAPI RecvThread(LPVOID Param);
static DWORD WINAPI RecvPacketThread(LPVOID Param);
FrameRecv::FrameRecv()
{
	mBuffer = new unsigned char[RECVBUFFER_LEN];
	mBufferReadLen = 0;
    mBufferFrameLen = BUFFERFRAMELEN;
    mReadState = COM_RECV_HEAD;
}

FrameRecv::~FrameRecv()
{
	mThreadRun = false;
	if(mThreadRecv != NULL)
	{
		CloseHandle(mThreadRecv);
		mThreadRecv = NULL;
	}
	
	mThreadPacketRun = false;	
	if(mThreadPacket != NULL)
	{
		CloseHandle(mThreadPacket);
		mThreadPacket = NULL;
	}
	
	if(!mRecvQueue.empty())
	{
		mRecvQueue.pop();
	}

	delete[] mBuffer;
}

void FrameRecv::FrameRecvStart(FrameCom* p)
{
	mFrameCom = p;

	mThreadRun = true;	
	mThreadRecv = CreateThread(NULL, 0, RecvThread, this, 0, &mThreadRecvID); 

	mThreadPacketRun = true;	
	mThreadPacket = CreateThread(NULL, 0, RecvPacketThread, this, 0, &mThreadPacketID); 
}

void FrameRecv::FrameRecvStop()
{
	mThreadRun = false;
	if(mThreadRecv != NULL)
	{
		CloseHandle(mThreadRecv);
		mThreadRecv = NULL;
	}

	mThreadPacketRun = false;	
	if(mThreadPacket != NULL)
	{
		CloseHandle(mThreadPacket);
		mThreadPacket = NULL;
	}
	
	if(!mRecvQueue.empty())
	{
		mRecvQueue.pop();
	}
}

RecvItem* FrameRecv::PullRecvItem()
{
	RecvItem *mItem = NULL;
	RecvItem *nItem = NULL;
	mMutex.Lock();
	if(!mRecvQueue.empty())
	{
		mItem = &mRecvQueue.front();
		nItem = (RecvItem*)malloc(sizeof(RecvItem));
		memcpy(nItem,mItem,sizeof(RecvItem));
		mRecvQueue.pop();
	}
	mMutex.Unlock();
	return nItem;
}

void FrameRecv::PushRecvItem(unsigned char* Buf, unsigned int BufLen)
{
	RecvItem Item;
	memcpy(Item.buffer,Buf, BufLen);
	Item.bufferlen = BufLen;
	mMutex.Lock();
	mRecvQueue.push(Item);
	mMutex.Unlock();
}

bool FrameRecv::ReadFrame()
{
    int i,j,aiindex = 0;
    bool mbool = false;
    unsigned char buffer[1024];

    if (mReadState == COM_RECV_HEAD){
        if(mBufferReadLen >= BUFFERFRAMELEN){
        	for (i = 0; i < mBufferReadLen; i++){
                if ((mBuffer[i]&0xFF) == 0xaa){
                	aiindex = i;
                	break;
                }
            }
        	
        	if (aiindex > 0){
            	mBufferReadLen -= aiindex;
            	if(mBufferReadLen > 0){
            		for (j = 0; j < mBufferReadLen; j++)
                    {
                        mBuffer[j] = mBuffer[aiindex];
                        aiindex++;
                    }
            	}
            }
        	
        	if(mBufferReadLen > BUFFERFRAMELEN){
        		if(mBuffer[BUFFERFRAMELEN-1] > 70){
        			mBufferReadLen = 0;
        			mBufferFrameLen = BUFFERFRAMELEN;
        		}
        		else{
        			mBufferFrameLen = BUFFERFRAMELEN+mBuffer[BUFFERFRAMELEN-1];
        			mReadState = COM_RECV_DATA;
        		}
        	}
        }
    } 
    
    if (mReadState == COM_RECV_DATA){
    	if (mBufferReadLen >= mBufferFrameLen){
			PushRecvItem(mBuffer, mBufferFrameLen);
    		mBufferReadLen -= mBufferFrameLen;
    		if(mBufferReadLen > 0){
    			for (j = 0; j < mBufferReadLen; j++){
    				buffer[j] = mBuffer[mBufferFrameLen+j];
    			}
    			for(j = 0; j < RECVBUFFER_LEN;j++){
    				mBuffer[j] = 0;
    			}
    			for (j = 0; j < mBufferReadLen; j++){
    				mBuffer[j] = buffer[j];
    			}
    			mbool = true;
    		}
    		mReadState = COM_RECV_HEAD;
    		mBufferFrameLen = BUFFERFRAMELEN;
    	}
    }

    return mbool;
}

void FrameRecv::FrameRecvData(unsigned char* data, int datalen)
{
	bool mrunning = true;
	if(RECVBUFFER_LEN <= (mBufferReadLen+datalen))
	{
		while(mrunning){
        	mrunning = ReadFrame();
        }
	}
	
	if(datalen > 0){
		for (int i = 0; i < datalen; i++){
            mBuffer[mBufferReadLen+i] = data[i];
        }
		mBufferReadLen += datalen;
		mrunning = true;
        while(mrunning){
        	mrunning = ReadFrame();
        }
	}
}

void FrameRecv::RevcCom()
{
	if (mFrameCom != NULL)
	{
		unsigned char buffer[200];
		int nlen = 1;
		int len = mFrameCom->FrameComRecv(buffer,nlen);
		
		if(len > 0)
		{
			FrameRecvData(buffer,len);
		}
	}
}

static DWORD WINAPI RecvThread(LPVOID Param)
{
	FrameRecv* nFrameRecv = (FrameRecv*)Param;
	while(nFrameRecv->IsThreadRun())
	{
		nFrameRecv->RevcCom();
		Sleep(1);
	}
	return 0;

}

static DWORD WINAPI RecvPacketThread(LPVOID Param)
{
	int delay = 1;
	RecvItem* Item;
	FrameRecv* nFrameRecv = (FrameRecv*)Param;
	while(nFrameRecv->IsThreadPacketRun())
	{	
		Item = nFrameRecv->PullRecvItem();
		if(Item != NULL)
		{
			nFrameRecv->SerialDataUnPack(Item->buffer, Item->bufferlen);
			delay = 1;
		}
		else
		{
			if(delay < 100)
			{
				delay++;
			}
		}
		Sleep(delay);
	}
	return 0;

}