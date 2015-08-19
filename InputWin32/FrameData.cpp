#include "stdafx.h"
#include <iostream>
#include "FrameTypeDef.h"
#include "FrameData.h"

using namespace std;

FrameData::FrameData()
{
	OnResponsionReport = NULL;
	OnDistributeReport = NULL;
	ID = 0;
}

FrameData::~FrameData()
{
	OnResponsionReport = NULL;
	OnDistributeReport = NULL;
}

void FrameData::SerialDataReportListener(OnCommDataReportListener DistributeReport, OnCommDataReportListener ResponsionReport, void* param)
{
	OnDistributeReport = DistributeReport;
	OnResponsionReport = ResponsionReport;
	Param = param;
}

unsigned char FrameData::SerialDataID()
{
	ID += 1;
    if (ID == 0)
    {
        ID = 1;
    }
    return ID;
}

int FrameData::SerialDataPack(unsigned char *data, int FrameCmd, int FrameType, int FrameAck, unsigned char *buffer, int bufferlen)
{
	if (NULL == data || buffer == NULL)
	{
		return 0;
	}

	int dataLen = FRAME_MIN;
    int checkindex;
    unsigned char check = 0;
    data[FRAME_START] = 0xAA;
    data[FRAME_VERSION] = 0x00;
    data[FRAME_CTRL] = (FrameType+FrameAck+0x78);
    check += data[FRAME_CTRL];
    data[FRAME_SRC] = (ADDR_ANDROID);
    check += data[FRAME_SRC];
    data[FRAME_DST] = (ADDR_RFID);
    check += data[FRAME_DST];
    data[FRAME_CMD] = FrameCmd;
    check += data[FRAME_CMD];
    data[FRAME_ID] = SerialDataID();
    check += data[FRAME_ID];
    data[FRAME_LEN] = (3+bufferlen);

    checkindex = FRAME_DATA;

    if (bufferlen > 0){
        for (int i = 0; i < bufferlen; i++){
            data[FRAME_DATA+i] = buffer[i];
            checkindex++;
            check += buffer[i];
        }
    }
    data[checkindex] = check;
    dataLen += bufferlen;
    return dataLen;
}

void FrameData::SerialDataUnPack(unsigned char *data, int dataLen)
{
	int mFrameType = FRAME_SEND;
	int mFrameAck = ACK_NEED;
	int FrameCmd = 0;
	int len = 0;
	int Id = -1;
	unsigned char buffer[50];

	if (data != NULL && dataLen >= FRAME_MIN)
	{
		if ((data[FRAME_START] & 0XFF) != 0xaa){
#if _DEBUG_
			cout <<"Frame Start Error"<<endl;
#endif
			return;
		}

		if ((data[FRAME_CTRL] & 0X06) != 0x00){
#if _DEBUG_
			cout << "Frame Ctrl Error" << endl;
#endif
			return;
		}
	    
		unsigned char Addr = (data[FRAME_DST] & 0XFF);     
		if (Addr != ADDR_NONE &&  Addr != ADDR_PC){
#if _DEBUG_
			cout << "Frame Addr Error" << endl;
#endif
			return;
		}

		len = (data[FRAME_LEN]& 0XFF)-3;
		if (len+FRAME_MIN != dataLen){
#if _DEBUG_
			cout << "Frame Len Error" << endl;
#endif			
			return;
		}

		if (((data[FRAME_CTRL] &0xFF) & 0x80) > 0){
			mFrameType = FRAME_ACK;
		}

		if (((data[FRAME_CTRL] & 0xFF) & 0x01) > 0){
			mFrameAck = ACK_NONEED;
		}

		Id = data[FRAME_ID] & 0xFF;
		FrameCmd = data[FRAME_CMD] & 0xFF;

		if (len > 0){
			for (int i = 0; i < len; i++){
				buffer[i] =  data[FRAME_DATA+i];
			}
		}
		
		if (mFrameType == FRAME_ACK)
		{
			if(OnResponsionReport != NULL)
			{
				OnResponsionReport(Param, Id, FrameCmd, mFrameAck, buffer, len);
			}
		}
		else 
		{
			if(OnDistributeReport != NULL)
			{
				OnDistributeReport(Param, Id, FrameCmd, mFrameAck, buffer, len);
			}	
		}
	}
}