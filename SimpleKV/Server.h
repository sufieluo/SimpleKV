#pragma once
#include <sys/socket.h>
#include <sys//types.h>
#include <arpa/inet.h>
#include "MyTask.h"
#include "WorkThreadPool.h"
#define PORT 7777
using namespace std;
class Server
{
public:
	Server()
	{
	}
	~Server()
	{
	}
	int initSocket()
	{
		m_iServerfd = socket(AF_INET, SOCK_STREAM, 0);
		m_serverAddr.sin_addr.s_addr =inet_addr("127.0.0.1");
		m_serverAddr.sin_port = htons(PORT);
		m_serverAddr.sin_family = AF_INET;
		
		if (bind(m_iServerfd, (sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) < 0)
		{
			return -1;
		}

		
		if (listen(m_iServerfd, 10) < 0)
		{
			return -1;
		}
		return 0;
	}
	
	void startServer()
	{
		//建一个10个线程的线程池
		WorkThreadPool *pool = new WorkThreadPool(1);
		//m_index.load();
		while (1)
		{
			int iClientfd;
			if ((iClientfd = accept(m_iServerfd,NULL,NULL)) < 0 )
			{
				printf("accept error\n");
				continue;
			}
			Task *task = new MyTask(iClientfd);
			pool->addTask(task);
		}
		close(m_iServerfd);
	}
	
private:
	int m_iServerfd;
	sockaddr_in m_serverAddr;
	KVIndex m_index;
	
};