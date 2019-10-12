
class WorkThread
{
public:
	WorkThread() :m_stop(false){}
	~WorkThread(){}

	void createThreads(HANDLE iocp,uint32_t num = 0);
	void workThread();

private:
	bool m_stop;
	HANDLE m_iocp;
};