#pragma once
#include <string>
using namespace std;
struct KVRecord
{
	bool flag; //0 表示正常 1表示被删除
	string key;
	string value; //NULL 表示在文件中，NOT NULL表示在缓存中
	int offset;
	int lenValue;
	int lenKey;
	KVRecord *pre;
	KVRecord *next;

};