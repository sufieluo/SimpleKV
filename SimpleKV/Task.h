#ifndef _TASK_H_
#define _TASK_H_

class Task
{
public:
	Task()
	{
	}
	Task(int id)
	{
		m_itaskID = id;
	}
	virtual ~Task()
	{
	}
	virtual void doTask()
	{
	}
	void setTaskID(int id)
	{
		m_itaskID = id;
	}
	int getTaskID()
	{
		return m_itaskID;
	}
private:
	int m_itaskID;
};
#endif // !_TASK_H_
