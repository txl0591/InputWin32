#ifndef _FRAMERECV_
#define _FRAMERECV_


#include "Mutex.h"
#include "FrameCom.h"

#define COM_RECV_HEAD	0x00
#define COM_RECV_DATA	0x01
#define RECVBUFFER_LEN  (1024*5)
#define BUFFERFRAMELEN  6

typedef struct 
{
	unsigned char buffer[60];
	int bufferlen;
}RecvItem;


class FrameRecv:public FrameData
{
public:
	FrameRecv();
	~FrameRecv();
	
	void FrameRecvStart(FrameCom* p);
	void FrameRecvStop();
	void FrameRecvData(unsigned char* data, int datalen);
	bool IsThreadRun(){return mThreadRun;}
    bool IsThreadPacketRun(){return mThreadPacketRun;}
	void RevcCom();
    RecvItem* PullRecvItem();
	void PushRecvItem(unsigned char * Buf, unsigned int BufLen);
private:
	bool ReadFrame();

public:
	FrameCom* mFrameCom;

private :
	Queue<RecvItem> mRecvQueue;
	unsigned char* mBuffer;
	int mBufferReadLen;
    int mBufferFrameLen;
    int mReadState;
    
	HANDLE mThreadRecv;
	DWORD mThreadRecvID;
	bool mThreadRun;
	
	HANDLE mThreadPacket;
	DWORD mThreadPacketID;
	Mutex mMutex;
	bool mThreadPacketRun;
    
};


#endif
