
#ifndef _FRAMEDATA_
#define _FRAMEDATA_

#define FRAME_MIN  9

typedef void(*OnCommDataReportListener)(void* mFrameProcessor,int Id, int Cmd, int Ack, unsigned char* Data, int DataLen);

class FrameData
{
public:
	FrameData();
	~FrameData();
	
	void SerialDataUnPack(unsigned char* data, int dataLen);
	int SerialDataPack(unsigned char* data, int FrameCmd, int FrameType, int FrameAck, unsigned char* buffer, int bufferlen);
	void SerialDataReportListener(OnCommDataReportListener DistributeReport, OnCommDataReportListener ResponsionReport, void* param);

private:
	unsigned char SerialDataID();

private:
	OnCommDataReportListener OnResponsionReport;
	OnCommDataReportListener OnDistributeReport;
	void* Param;
	unsigned char ID;
};

#endif