#ifndef _FRAMECOM_
#define _FRAMECOM_

#define USB_SERIAL_CH340 "USB-SERIAL CH340"

#define DEFAULE_COM USB_SERIAL_CH340

#define _COM_DEBUG_		1

class FrameCom
{
public:
	FrameCom();
	~FrameCom();
	
	void SetFrameCom(int com);
	void FrameComScan();
	bool FrameComIsOpen(){return mComState;}
	bool FrameComOpen();
	bool FrameComClose();
	bool FrameComSend(unsigned char * buf, int buflen);
	int FrameComRecv(unsigned char * buf, int buflen);
	void Printf(CString Str,unsigned char* buffer, int bufferlen);

private:
	CString mComName;
	HANDLE mComHandle;
	bool mComState;
	bool mComSend;
	int Com;
};

#endif