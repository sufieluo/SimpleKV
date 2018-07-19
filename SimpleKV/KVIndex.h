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
			//先去cache中找,命中取出，不命中去file中取
			//不命中,file中取
			 if(m_cache.getFromCache(record) < 0)
 			{
				
	 			if(m_store.getFromFile(record) < 0)
	 			{
 					return -1;
 				}
	 			//更新cache
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
		//该值已存在,覆盖旧值
		if (it != m_mIndex.end())
		{
			KVRecord *record = it->second;
			//cache中也存在，cache和file都需要更新
			if(!record->value.empty())
			{
				//删除cache中的记录
				if(m_cache.removeFromCache(record) < 0)
				{
					return -1;
				}
				
				record->value = value;
				record->lenValue = value.length();

				//更新文件
				if(m_store.setToFile(record) < 0)
				{
					return -1;
				}
				
				if (m_cache.setToCache(record) < 0)
				{
					return -1;
				}
				//更新内存索引
				m_mIndex[key] = record;
				
			}
			else
			{
				//先更新文件，采用append方式
				record->value = value;
				record->lenValue = value.length();
				
				if(m_store.setToFile(record) <0)
				{
					return -1;
				}
				
				//更新到cache
				if(m_cache.setToCache(record) <0)
				{
					return -1;
				}				
				//更新内存索引
				m_mIndex[record->key] = record;
			}
		}
		//如果不存在该值
		else
		{
			KVRecord *newRecord = new KVRecord();
			newRecord->flag = 0;
			newRecord->key = key;
			newRecord->value = value;
			newRecord->lenKey = key.length();
			newRecord->lenValue = value.length();
			//更新cache和file
			if(m_cache.setToCache(newRecord) <0)
			{
				return -1;
			}
		
			if (m_store.setToFile(newRecord)<0)
			{
				return -1;
			}
			
			//更新内存索引
			m_mIndex[key] = newRecord;
		}
		
		pthread_mutex_unlock(&m_mutex);
		return 0;
	}
	
	int deleteData(string &key)
	{
		pthread_mutex_lock(&m_mutex);
		unordered_map<string, KVRecord*>::iterator it = m_mIndex.find(key);
		//该值不存在
		if(it == m_mIndex.end())
		{
			pthread_mutex_lock(&m_mutex);
			return -1;
		}
		else
		{
			KVRecord *record = it->second;
			record->flag = 1;
			//将删除记录追加至file
			if (m_store.setToFile(record) <0)
			{
				return -1;
			}
			//删除cache记录
			if (m_cache.removeFromCache(record) <0)
			{
				return -1;
			}
			//从内存索引中删除
			m_mIndex.erase(record->key);
			
		}
	}
	//重启时加载将file中所有record加载进内存索引
	void load()
	{
		m_store.load(m_mIndex);
		
	}
private:
	//内存索引，hashmap
	unordered_map<string, KVRecord*> m_mIndex;
	pthread_rwlock_t m_rlock;
	pthread_mutex_t m_mutex;
	KVCache m_cache;
	KVStore m_store;

};
