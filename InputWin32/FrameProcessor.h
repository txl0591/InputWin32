#ifndef _FRAMEPROCESSOR_
#define _FRAMEPROCESSOR_

#define INFO_NAME_NUM           6
#define MAX_INFO_NUM            20

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
	unsigned char Send;           
	unsigned char Num;
	unsigned char Tan;
	unsigned char NameLen;
	ZONE_DATE_TIME Time;
	unsigned char Name[INFO_NAME_NUM];
}INFO_HEAD, *PINFO_HEAD;

typedef struct
{
	unsigned char Param[16 * MAX_INFO_NUM];
	unsigned char TParam[MAX_INFO_NUM];
}INFO_DATA, *PINFO_DATA;

typedef struct
{
	unsigned char NowID;
	unsigned char MaxID;
	INFO_HEAD mHead;
	INFO_DATA mData;
}INFO_PARAM, *PINFO_PARAM;

typedef void(*OnEchoReportListener)(int Echo, unsigned char* Data, int DataLen);

#define FRAME_ID_NONE		-1
#define FRAME_RECV_DEFAULT  3

class FrameProcessor
{
public:
	FrameProcessor(int com, OnEchoReportListener Listener);
	~FrameProcessor();

	void FrameComOpen(int com);

	void FrameProcessorStop();	
	void setStateReport(int Echo, unsigned char* Data, int DataLen);
	void FrameProcessorIn(int Id, unsigned char* Data, int DataLen);

private: 
	OnEchoReportListener mOnEchoReportListener;
	FrameRecv mFrameRecv;
	FrameSend mFrameSend;
	FrameCom mFrameCom;
	unsigned char mWritebuf[1024];
	int mWriteBufLen;
	int mParamMaxIndex;
	int mParamNowIndex;
	int mNowID;
	INFO_PARAM mParam;
};


#endif