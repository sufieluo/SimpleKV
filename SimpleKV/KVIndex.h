#pragma once
#include <string>
#include <unordered_map>
#include <pthread.h>
#include "KVCache.h"
#include "KVRecord.h"
#include "KVStore.h"
using namespace std;

class KVIndex
{
public:
	KVIndex()
	{
		m_store.init();
		pthread_rwlock_init(&m_rlock,NULL);
		pthread_mutex_init(&m_mutex,NULL);
	}
	~KVIndex()
	{
		pthread_rwlock_destroy(&m_rlock);
		pthread_mutex_destroy(&m_mutex);
	}
	int get(string &key,string &value)
	{
		pthread_rwlock_rdlock(&m_rlock);
		unordered_map<string, KVRecord*>::iterator it = m_mIndex.find(key);
		if (it != m_mIndex.end())
		{
			pthread_mutex_lock(&m_mutex);
			KVRecord *record = it->second;
			//��ȥcache����,����ȡ����������ȥfile��ȡ
			//������,file��ȡ
			 if(m_cache.getFromCache(record) < 0)
 			{
				
	 			if(m_store.getFromFile(record) < 0)
	 			{
 					return -1;
 				}
	 			//����cache
	 			m_cache.removeFromCache(record);
	 			m_cache.setToCache(record);
	 			
 			}
			
			value = record->value;
			
			pthread_mutex_unlock(&m_mutex);
			return 0;
		}
		else
		{
			pthread_rwlock_unlock(&m_rlock);
			return -1;
		}
	}
	int set(string &key, string &value)
	{
		pthread_mutex_lock(&m_mutex);
		unordered_map<string, KVRecord*>::iterator it = m_mIndex.find(key);
		//��ֵ�Ѵ���,���Ǿ�ֵ
		if (it != m_mIndex.end())
		{
			KVRecord *record = it->second;
			//cache��Ҳ���ڣ�cache��file����Ҫ����
			if(!record->value.empty())
			{
				//ɾ��cache�еļ�¼
				if(m_cache.removeFromCache(record) < 0)
				{
					return -1;
				}
				
				record->value = value;
				record->lenValue = value.length();

				//�����ļ�
				if(m_store.setToFile(record) < 0)
				{
					return -1;
				}
				
				if (m_cache.setToCache(record) < 0)
				{
					return -1;
				}
				//�����ڴ�����
				m_mIndex[key] = record;
				
			}
			else
			{
				//�ȸ����ļ�������append��ʽ
				record->value = value;
				record->lenValue = value.length();
				
				if(m_store.setToFile(record) <0)
				{
					return -1;
				}
				
				//���µ�cache
				if(m_cache.setToCache(record) <0)
				{
					return -1;
				}				
				//�����ڴ�����
				m_mIndex[record->key] = record;
			}
		}
		//��������ڸ�ֵ
		else
		{
			KVRecord *newRecord = new KVRecord();
			newRecord->flag = 0;
			newRecord->key = key;
			newRecord->value = value;
			newRecord->lenKey = key.length();
			newRecord->lenValue = value.length();
			//����cache��file
			if(m_cache.setToCache(newRecord) <0)
			{
				return -1;
			}
		
			if (m_store.setToFile(newRecord)<0)
			{
				return -1;
			}
			
			//�����ڴ�����
			m_mIndex[key] = newRecord;
		}
		
		pthread_mutex_unlock(&m_mutex);
		return 0;
	}
	
	int deleteData(string &key)
	{
		pthread_mutex_lock(&m_mutex);
		unordered_map<string, KVRecord*>::iterator it = m_mIndex.find(key);
		//��ֵ������
		if(it == m_mIndex.end())
		{
			pthread_mutex_lock(&m_mutex);
			return -1;
		}
		else
		{
			KVRecord *record = it->second;
			record->flag = 1;
			//��ɾ����¼׷����file
			if (m_store.setToFile(record) <0)
			{
				return -1;
			}
			//ɾ��cache��¼
			if (m_cache.removeFromCache(record) <0)
			{
				return -1;
			}
			//���ڴ�������ɾ��
			m_mIndex.erase(record->key);
			
		}
	}
	//����ʱ���ؽ�file������record���ؽ��ڴ�����
	void load()
	{
		m_store.load(m_mIndex);
		
	}
private:
	//�ڴ�������hashmap
	unordered_map<string, KVRecord*> m_mIndex;
	pthread_rwlock_t m_rlock;
	pthread_mutex_t m_mutex;
	KVCache m_cache;
	KVStore m_store;

};
