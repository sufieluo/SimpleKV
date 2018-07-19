#pragma once
#include <string>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unordered_map>
#include "KVIndex.h"
#define MAXSIZE 1024
using namespace std;

class KVStore
{
public:
	KVStore()
	{
	}
	~KVStore()
	{
	}
	int setToFile(KVRecord* record)
	{
		int lenKey = record->lenKey;
		int lenValue = record->lenValue;
		
		write(m_fd,&record->flag,sizeof(bool));
		write(m_fd,&lenKey,sizeof(int));
		write(m_fd,record->key.c_str(),lenKey);
		write(m_fd,&lenValue,sizeof(int));
		
		int pos;
		if ((pos = lseek(m_fd,0,SEEK_CUR)) < 0)
		{
			return -1;
		}
		
		record->offset = pos;
		
		write(m_fd,record->value.c_str(),lenValue);
		close(m_fd);
		return 0;
		
	}
	int getFromFile(KVRecord* record)
	{
		int pos = record->offset;
		if (lseek(m_fd,pos,SEEK_SET) < 0)
		{
			return -1;
		}
		char buf[MAXSIZE];
		read(m_fd, buf, record->lenValue);
		record->value = buf;
		close(m_fd);
		return 0;
	}
	//将文件中已删除或者更新的文件进行整理，形成新文件
	int reStore()
	{
		char * newPath = "/tmp/temp.txt";
		int newfd;
		if ((newfd = open(m_path, O_RDWR | O_APPEND | O_CREAT, 00644)) < 0)
		{
			printf("open temp file error\n");
			return -1;
		}
		unordered_map<string, KVRecord*>::iterator it = m_map.begin();
		while (it != m_map.end())
		{
			KVRecord *record = it->second;
			write(newfd, &record->flag, sizeof(bool));
			write(newfd, &record->lenKey, sizeof(int));
			write(newfd, record->key.c_str(), record->lenKey);
			write(newfd, &record->lenValue, sizeof(int));
			
			record->offset = lseek(newfd, 0, SEEK_CUR);
			
			write(newfd, record->value.c_str(), record->lenValue);
			
			it++;
		}
		return 0;
	}
	//将文件中所有内容重新加载到内存中
	int compact()
	{
		while (1)
		{
			KVRecord *record;
			char buf[MAXSIZE];
			
			int len;
			if ((len = read(m_fd, &record->flag, sizeof(bool))) < 0)
			{
				return -1;
			}
			else if(len == 0)
			{
				break;
			}
			
			if (read(m_fd,&record->lenKey,sizeof(int)) < 0)
			{
				return -1;
			}
			
			if (read(m_fd, buf, record->lenKey) < 0)
			{
				return -1;
			}
			record->key = buf;
			bzero(buf, MAXSIZE);
			if (read(m_fd, &record->lenValue, sizeof(int)) < 0)
			{
				return -1;
			}
			
			if (read(m_fd, buf, record->lenValue) < 0)
			{
				return -1;
			}
			record->value = buf;
			bzero(buf, MAXSIZE);
			
			//如果该数据没被标记删除，加入内存中
			if (record->flag == 0)
			{
				m_map[record->key] = record;
			}
		}
		
		if (reStore() <0)
		{
			return -1;
		}
		return 0;
	}
	void load(unordered_map<string, KVRecord*> &m_mIndex)
	{
		m_mIndex = m_map;
	}
	void init()
	{
		m_path = "/tmp/test.txt";
		if ((m_fd = open(m_path, O_RDWR | O_APPEND | O_CREAT,00644)) < 0)
		{
			printf("open file error\n");
			exit(0);
		}
		
	}
private:
	unordered_map<string, KVRecord*> m_map;
	char *m_path;
	int m_fd;
};
