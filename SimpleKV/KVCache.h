#pragma once
#include "KVRecord.h"
#define MAXSIZE 10
using namespace std;

class KVCache 
{
public:
	KVCache()
	{
		m_iSize = 0;
		m_head = NULL;
		m_tail = NULL;
	}
	~KVCache()
	{
	}
	
	//����ͷ��
	int setToCache(KVRecord *record)
	{
		record->next = m_head;
		record->pre = NULL;
		
		if (m_head == NULL)
		{
			m_head = record;
			m_tail = record;
		}
		else
		{
			m_head->pre = record;
			m_head = record;
		}
		
		m_iSize++;
		
		if (m_iSize > MAXSIZE)
		{
			removeFromCache(m_tail);
		}
		
	}
	
	int removeFromCache(KVRecord *record)
	{
		if (record->pre != NULL)
		{
			record->pre->next = record->next;
		}
		else
		{
			m_head = record->next;
		}
		if (record->next != NULL)
		{
			record->next->pre = record->pre;
		}
		else
		{
			m_tail = record->pre;
		}
		record->value.erase(NULL);
	
		return 0;
	}
	int getFromCache(KVRecord *record)
	{
		//�������cache��
		if(record->pre == NULL && record->next == NULL)
		{
			return -1;
		}
		//����record��cache�е�λ�ü���
		else
		{
			removeFromCache(record);
			setToCache(record);
		}
		
	}
private:
	int m_iSize;
	KVRecord *m_head;
	KVRecord *m_tail;
};