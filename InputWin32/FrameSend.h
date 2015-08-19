#ifndef _FRAMESEND_
#define _FRAMESEND_
 
#include "Mutex.h"
#include "FrameCom.h"

typedef struct 
{
	unsigned char buffer[60];
	int bufferlen;
}SendItem;

typedef void(*OnSendReportListener)(void *param,int Echo);

class FrameSend:public FrameData
{
public:
	FrameSend();
	~FrameSend();

	void FrameSendStop();
	void SendAck(int FrameCmd,unsigned char* buffer, int bufferlen);
	void SendNoAck(int FrameCmd,unsigned char* buffer, int bufferlen);
	void SendEcho(int FrameCmd,unsigned char* buffer, int bufferlen);
	bool IsThreadRun(){return mThreadRun;}
	SendItem* PullSendItem();
	void PushSendItem(SendItem* Item);
	bool SendCom(unsigned char* buffer, int bufferlen);
	void SetFrameCom(FrameCom* p){ mFrameCom = p; }
	void SetSendReportListener(OnSendReportListener proc, void* param){ ProcSend = proc; Param = param; };
	void SendReportListener(int echo);

private :
	Queue<SendItem> mSendQueue;
	FrameCom* mFrameCom;
	HANDLE mThreadSend;
	DWORD mThreadSendID;
	Mutex mMutex;
	bool mThreadRun;
	bool mComSendState;
	OnSendReportListener ProcSend;
	void* Param;
	
};


#endif