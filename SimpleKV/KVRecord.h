#pragma once
#include <string>
using namespace std;
struct KVRecord
{
	bool flag; //0 ��ʾ���� 1��ʾ��ɾ��
	string key;
	string value; //NULL ��ʾ���ļ��У�NOT NULL��ʾ�ڻ�����
	int offset;
	int lenValue;
	int lenKey;
	KVRecord *pre;
	KVRecord *next;

};