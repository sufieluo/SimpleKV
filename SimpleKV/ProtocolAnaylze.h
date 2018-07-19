#include <string>
#include "KVIndex.h"
#include <string>
using namespace std;
class ProtocolAnaylze
{
public:
	ProtocolAnaylze()
	{
	}
	~ProtocolAnaylze()
	{
	}
	void anaylze(const string &req,string &ack)
	{
		if (req.substr(0,1) != "-")
		{
			printf("commond error,must start with -\n");
			exit(0);
		}
		int pos = req.find(' ');
		//协议get、set、delete
		if(pos != -1)
		{
			 string commond = req.substr(1, pos);
			if (commond == "get")
			{
				string key = req.substr(pos + 1, req.length());
				string value;
				if (m_Index.get(key, value) < 0)
				{
					ack = "+FAILED";
				}
				else
				{
					ack = "+" + value;
				}
			}
			else if (commond  == "set")
			{
				string newstring = req.substr(pos + 1, req.length());
				int secpos = newstring.find(' ');
				string key = newstring.substr(0, secpos);
				string value = newstring.substr(secpos + 1, newstring.length());
				if (m_Index.set(key,value) < 0)
				{
					ack = "+FAILED";
				}
				else
				{
					ack = "+OK";
				}
			}
			else if (commond  == "delete")
			{
				string key = req.substr(pos + 1, req.length());
				if (m_Index.deleteData(key) < 0)
				{
					ack = "+FAILED";
				}
				else
				{
					ack = "+OK";
				}
			}
		}
		//协议stats、quit
		else
		{
			string commond = req.substr(1, req.length());
			if (commond.compare("stats") == 0)
			{
			}
			else if (commond.compare("quit") == 0)
			{
				ack = "+OK";
			}
		}
	}
private:
	KVIndex m_Index;
};