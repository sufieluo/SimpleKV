#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>

class Thread
{
public:
	Thread();
	virtual ~Thread();
	
	int start();
	int join();
	
	void setDelete(bool abDelete);
	pthread_t getThreadID();
	void setThreadID(int id);

private:
	static void *threadRoutine(void *arg);
	virtual void run() = 0;
	pthread_t m_threadID;
	bool m_delete;
	
};
#endif // !_THREAD_H_
