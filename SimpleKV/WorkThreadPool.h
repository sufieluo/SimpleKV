#ifndef _WORKTHREADPOOL_H_
#define _WORKTHREADPOOL_H_
#include <pthread.h>
#include <vector>
#include "WorkQueue.h"
#include "Worker.h"

class WorkThreadPool
{
private:
	int m_iInitNum;
	
	WorkQueue *m_queue;
	vector<Worker *> m_vecWorker;
	pthread_mutex_t m_threadpoolMutex;
	
public:
	WorkThreadPool()
	{
	}
	WorkThreadPool(int initNum)
	{
		m_queue = new WorkQueue(5);
		m_iInitNum = initNum;
		createThreadPool();
	}
	virtual ~WorkThreadPool()
	{
	}
	
	void setInitNum(int initNum)
	{
		m_iInitNum = initNum;
	}
	int getInitNum()
	{
		return m_iInitNum;
	}
	
	void createThreadPool()
	{
		for (int i = 0; i < m_iInitNum; i++)
		{
			Worker *worker = new Worker(m_queue,i);
			
			m_vecWorker.push_back(worker);
			worker->start();
			
		}
	}
	void destoryThreadPool()
	{
		if(!m_queue->isEmpty())
		{
			sleep(20);
		}
		
		//delete m_queue;
		//delete &m_vecWorker;

	}
	void addTask(Task *task)
	{	
		m_queue->push(task);
	}
	
};

#endif // !_THREADPOOL_H_