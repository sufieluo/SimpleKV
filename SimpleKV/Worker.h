#ifndef _WORKER_H_
#define _WORKER_H_

#include "WorkQueue.h"
#include "Thread.h"
#include <unistd.h>

class Worker :public Thread
{
public:
	Worker()
	{
	}
	Worker(WorkQueue *aqueue,int id)
	{
		m_queue = aqueue;
		m_threadID = id;
	}
	~Worker()
	{
	}

	void run()
	{
		while (1)
		{
		
			while (!m_queue->isEmpty())
			{
				
				Task *task = m_queue->pop();
				cout << "Thread " << m_threadID << " doing Task " << task->getTaskID() << endl;
				task->doTask();
			}
		
			
		}
	}
private:
	WorkQueue *m_queue;
	pthread_t m_threadID;
};

#endif