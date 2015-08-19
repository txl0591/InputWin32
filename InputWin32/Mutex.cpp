#include "stdafx.h"
#include <windows.h>   
#include "Mutex.h"


Mutex::Mutex()
{
	 mMutex = CreateMutex(NULL, FALSE, NULL);  
}

Mutex::~Mutex()
{
	CloseHandle(mMutex);  
}

void Mutex::Lock()  
{  
    DWORD d = WaitForSingleObject(mMutex, INFINITE);  
}  
  
void Mutex::Unlock()
{  
    ReleaseMutex(mMutex);  
}  