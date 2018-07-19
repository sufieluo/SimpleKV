#include "Thread.h"
#include "iostream"
#include <pthread.h>
using namespace std;

Thread::Thread()
	:m_delete(false)
{
	cout << "Thread"  << endl;
}

Thread::~Thread()
{
	cout << "~Thread ..." << endl;
}

int Thread::start()
{
	return pthread_create(&m_threadID, NULL, threadRoutine, this);
}

int Thread::join()
{
	pthread_join(m_threadID, NULL);
}

void *Thread::threadRoutine(void *arg)
{
	Thread *thread = static_cast<Thread *>(arg);
	thread->run();
	if (thread->m_delete)
	{
		delete thread;
	}
	return NULL;
}

void Thread::setDelete(bool abDelete)
{
	m_delete = abDelete;
}
pthread_t Thread::getThreadID()
{
	return m_threadID;
}

void Thread::setThreadID(int id)
{
	m_threadID = id;
}