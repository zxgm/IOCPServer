
class IOCPServer
{
public:
	void initialize(); 
	IOCPServer();
	~IOCPServer();

private:
	void initializeWinSock();
	void initializeSocket();
	void initializeThreadPool();
	void acceptClient();

	HANDLE createNewCompletionPort(uint32_t dwThreads = 0);
	bool associateDeviceWithCompletionPort(
		HANDLE hCompletionPort,HANDLE device,uint32_t dwCompletionKey);

	HANDLE m_hIocp;
	SOCKET m_listenSocket;
	OVERLAPPED_DATA *m_pOverlappedData;
	WorkThread *m_pWorkThread;
};