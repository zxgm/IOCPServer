
#include "CommonHeader.h"

unsigned WINAPI ThreadProc(void *param)
{
	WorkThread *workThread = (WorkThread *)param;
	workThread->workThread();
	return 0;
}

void WorkThread::createThreads(HANDLE iocp,uint32_t num)
{
	m_iocp = iocp;
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
	DWORD numberOfBytes = 0;
	void *key = NULL;
	OVERLAPPED *pOverlapped = NULL;
	OVERLAPPED_DATA *pOverlappedData = NULL;

	while (!m_stop)
	{
		BOOL bCompletionStatus = 
			GetQueuedCompletionStatus(
			m_iocp, 
			&numberOfBytes,
			(PULONG_PTR)&key,
			(LPOVERLAPPED *)&pOverlapped,
			INFINITE
		);
		if (FALSE == bCompletionStatus)
		{
			std::cout << "获取完成端口完成队列状态失败.错误码:"
				<< WSAGetLastError() << std::endl;
			return;
		}

		pOverlappedData = (OVERLAPPED_DATA *)(pOverlapped);
		std::cout << "接收数据长度:" << numberOfBytes
			<< ";接收数据:" << pOverlappedData->dataBuf.buf << std::endl;

		pOverlappedData->reInitialize();

		/*
#ifdef _DEBUG
		std::cout << "地址:0x" << std::hex << pOverlappedData
			<< ";数据地址:0x" << static_cast<void *>(pOverlappedData->dataBuf.buf)
			<< std::endl;
#endif
		*/
		DWORD recvBytes = 0;
		DWORD flags = 0;
		int recvStatus = WSARecv(
			pOverlappedData->s,
			&pOverlappedData->dataBuf,
			1, 
			&recvBytes,
			&flags,
			(LPWSAOVERLAPPED)&(pOverlappedData->overlapped),
			NULL
		);
		if (recvStatus == SOCKET_ERROR && WSA_IO_PENDING != WSAGetLastError())
		{
			std::cout << "异步收取数据失败.错误码:" << WSAGetLastError() << std::endl;
			return;
		}
	}
}
