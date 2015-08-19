#ifndef _MUTEX_
#define _MUTEX_

class Mutex
{
public:  
    Mutex();  
    ~Mutex();
	
	void Lock();
    void Unlock();  

private:  
    HANDLE mMutex;  
};

#endif