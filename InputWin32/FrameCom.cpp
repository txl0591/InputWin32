#include "stdafx.h"
#include <stdlib.h>
#include <windows.h>
#include <iomanip>
#include <iostream>
#include<string.h>
#include <objbase.h>
#include <initguid.h>
#include <Setupapi.h>
#include "FrameCom.h"

using namespace std;

FrameCom::FrameCom()
{
	mComHandle = INVALID_HANDLE_VALUE;
	mComState = FALSE;
	Com = 0;
}

void FrameCom::SetFrameCom(int com)
{
	mComHandle = INVALID_HANDLE_VALUE;
	mComState = FALSE;
	char name[6];
	memset(name, 0,sizeof(name));
	sprintf_s(name,"COM%d",com);
	mComName = name;
}

FrameCom::~FrameCom()
{
	CloseHandle(mComHandle);
}

void FrameCom::FrameComScan()
{
	/*
	int m_nSerialPortNum(0);
	CString strSerialList[256]; 
	CArray<SSerInfo,SSerInfo&> asi;
	EnumSerialPorts(asi,TRUE);
	m_nSerialPortNum = (int)asi.GetSize();
	for (int i = 0; i < asi.GetSize(); i++)
	{
		CString str = asi[i].strPortDesc;
		if(str == DEFAULE_COM)
		{
			int len = str.GetLength();
			CString name = asi[i].strFriendlyName;
			int nlen = name.GetLength();
			int comlen = nlen-len-3;
			mComName = name.Mid(len+2,comlen);
			break;
		}
		

	}
	*/
}

bool FrameCom::FrameComClose()
{
	if(mComHandle)
	{
		CloseHandle(mComHandle);
		mComHandle = INVALID_HANDLE_VALUE;
	}
	return true;
}

bool FrameCom::FrameComOpen()
{	
	FrameComClose();

	mComHandle = CreateFile(mComName,
						GENERIC_READ|GENERIC_WRITE, //允许读和写
						0, //独占方式
						NULL,
						OPEN_EXISTING, //打开而不是创建
						0, //同步方式
						NULL);

	if(mComHandle ==INVALID_HANDLE_VALUE)
	{
		cout<<"打开串口失败"<<endl;
		CloseHandle(mComHandle);
		mComState = FALSE;
	}
	else
	{
		mComState = TRUE;
		SetupComm(mComHandle,100,100); 
	
		COMMTIMEOUTS TimeOuts;
		//设定读超时
		TimeOuts.ReadIntervalTimeout=MAXDWORD;
		TimeOuts.ReadTotalTimeoutMultiplier=0;
		TimeOuts.ReadTotalTimeoutConstant=0;
		//在读一次输入缓冲区的内容后读操作就立即返回，
		//而不管是否读入了要求的字符。


		//设定写超时
		TimeOuts.WriteTotalTimeoutMultiplier=100;
		TimeOuts.WriteTotalTimeoutConstant=500;
		SetCommTimeouts(mComHandle,&TimeOuts); //设置超时

		DCB dcb;
		GetCommState(mComHandle,&dcb);
		dcb.BaudRate=115200; //波特率为9600
		dcb.ByteSize=8; //每个字节有8位
		dcb.Parity=NOPARITY; //无奇偶校验位
		dcb.StopBits=TWOSTOPBITS; //两个停止位
		SetCommState(mComHandle,&dcb);

		PurgeComm(mComHandle,PURGE_TXCLEAR|PURGE_RXCLEAR);
		cout<<"打开串口成功"<<endl;
	}

	return mComState;
}

bool FrameCom::FrameComSend(unsigned char * buf, int buflen)
{
	bool ret = FALSE;
	DWORD len = buflen;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	if(mComHandle == INVALID_HANDLE_VALUE)
	{
		cout<<"串口还未打开"<<endl;
		return ret;
	}
	ClearCommError(mComHandle,&dwErrorFlags,&ComStat);
	bWriteStat=WriteFile(mComHandle,buf,len, &len,NULL);
	if(!bWriteStat)
	{
		cout<<"写串口失败"<<endl;
		mComState = FALSE;
	}	
	else
	{
		return TRUE;
	}
	
	#if _COM_DEBUG_
	Printf(_T("Send Com "),buf,len);
	#endif

	return ret;
}

int FrameCom::FrameComRecv(unsigned char * buf, int buflen)
{
	char str[100];
	memset(str,0,100);
	DWORD wCount = buflen;
	BOOL bReadStat;

	if(mComHandle == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	bReadStat = ReadFile(mComHandle,str,wCount,&wCount,NULL);
	if(bReadStat)
	{
		if(wCount > 0)
		{
			memcpy(buf,str,wCount);
		}
		return wCount;
	}
	else
	{
		return 0;
	}
}

void FrameCom::Printf(CString Str,unsigned char* buffer, int bufferlen)
{
	cout<<Str<<" Len["<<bufferlen<<"] ";
	for (int i = 0; i < bufferlen; i++)
	{
		cout<<" 0x"<<setw(2)<<setfill('0')<<hex<<(int)buffer[i];
	}
	cout<<" "<<endl;
}