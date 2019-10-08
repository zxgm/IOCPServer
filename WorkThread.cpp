
#include "CommonHeader.h"

unsigned WINAPI ThreadProc(void *param)
{
	WorkThread *workThread = (WorkThread *)param;
	workThread->workThread();
}

void WorkThread::createThreads(uint32_t num)
{
	uint32_t numOfThreads = num;
	if (num == 0)
	{
		SYSTEM_INFO systemInfo;
		GetSystemInfo(&systemInfo);
		uint32_t numOfProcessors = systemInfo.dwNumberOfProcessors;
		numOfThreads = numOfProcessors * 2;
	}
	
	for (uint32_t i = 0; i < numOfThreads; numOfThreads--)
	{
		_beginthreadex(NULL, 0, ThreadProc, this, 0, NULL);
	}
}

void WorkThread::workThread()
{
	while (!m_stop)
	{
		GetQueuedCompletionStatus();
	}
}
