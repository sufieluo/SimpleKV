#pragma once
#include "Task.h"
#include <unistd.h>
#include <string>
#include "ProtocolAnaylze.h"
#define MAXSIZE 1024
using namespace std;
class MyTask:public Task
{
public:
	MyTask()
	{
	}
	MyTask(int clientfd)
	{
		this->m_iClientfd = clientfd;
	}
	~MyTask()
	{
	}
	void doTask()
	{
		char buf[MAXSIZE];
		int len = read(m_iClientfd, buf, MAXSIZE);
		//Ω‚Œˆ–≠“È
		const string req = buf;
		
		string ack;
		ProtocolAnaylze anaylzer;
		anaylzer.anaylze(req,ack);
		//bzero(buf);

		write(m_iClientfd, ack.c_str(), ack.length());
		close(m_iClientfd);
		
	}
private:
	int m_iClientfd;
};