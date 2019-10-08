
class WorkThread
{
public:
	WorkThread() :m_stop(false){}
	~WorkThread(){}

	void createThreads(uint32_t num = 0);
	void workThread();

private:
	bool m_stop;
};