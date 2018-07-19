#ifndef _WORKQUEUE_H_
#define _WORKQUEUE_H_
#include <pthread.h>
#include <queue>
#include "Task.h"
#define DEFAULTSIZE 10;
using namespace std;

//线程安全的queue
class WorkQueue
{
public:
	WorkQueue()
	{
		m_imaxSize = DEFAULTSIZE;
		pthread_mutex_init(&m_mutex, NULL);
		pthread_cond_init(&m_notEmpty, NULL);
		pthread_cond_init(&m_notFull, NULL);
	}
	WorkQueue(int maxSize)
	{
		m_imaxSize = maxSize;
		pthread_mutex_init(&m_mutex, NULL);
		pthread_cond_init(&m_notEmpty, NULL);
		pthread_cond_init(&m_notFull, NULL);
	}
	~WorkQueue()
	{
// 		pthread_mutex_destroy(&m_mutex);
// 		pthread_cond_destroy((&m_notEmpty);
// 		pthread_cond_destroy((&m_notFull);
	}
	void push(Task *task)
	{
		pthread_mutex_lock(&m_mutex);
		while (isFull())
		{
			pthread_cond_wait(&m_notFull, &m_mutex);
		}
		m_queue.push(task);
		pthread_mutex_unlock(&m_mutex);
		pthread_cond_signal(&m_notEmpty);
	}
	Task *pop()
	{
		pthread_mutex_lock(&m_mutex);
		while (isEmpty())
		{
			pthread_cond_wait(&m_notEmpty, &m_mutex);
		}
		Task *task = m_queue.front();
		m_queue.pop();
		pthread_mutex_unlock(&m_mutex);
		pthread_cond_signal(&m_notFull);
		return task;
	}
	bool isEmpty()
	{
		return m_queue.empty();
	
	}
	bool isFull()
	{
		bool flag = false;	
		if (m_queue.size() == m_imaxSize)
		{
			flag = true;
		}
		return flag;
	}

private:
	pthread_mutex_t m_mutex;
	pthread_cond_t m_notEmpty;
	pthread_cond_t m_notFull;
	
	int m_imaxSize;
	queue<Task *> m_queue;
	
};
#endif // !_WORKQUEUE_H_
