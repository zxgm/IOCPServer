#include "CommonHeader.h"

IOCPServer::IOCPServer()
{
	m_pWorkThread = new WorkThread();
}

IOCPServer::~IOCPServer()
{
	WSACleanup();
}

void IOCPServer::initialize()
{
	initializeWinSock();
	initializeSocket();
	initializeThreadPool();
}

void IOCPServer::acceptClient()
{
	while (true)
	{
		struct sockaddr_in remoteAddr;
		uint32_t len = sizeof(sockaddr);
		SOCKET clientSocket = accept(m_listenSocket,(struct sockaddr *)&remoteAddr,&len);

		m_pOverlappedData = new OVERLAPPED_DATA();
		m_pOverlappedData->socket = clientSocket;
		memset(m_pOverlappedData->wsaBuf.buf, 0, m_pOverlappedData->wsaBuf.len);

		associateDeviceWithCompletionPort(m_hIocp, clientSocket, );
	}
}

void IOCPServer::initializeThreadPool()
{
	m_pWorkThread->createThreads();
}

void IOCPServer::initializeWinSock()
{
	WSADATA wsaData;
	int32_t ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		throw std::exception("WSAStartup failed.");
	}
}

void IOCPServer::initializeSocket()
{
	int32_t len = sizeof(sockaddr);
	struct sockaddr_in bindSockaddr;
	bindSockaddr.sa_family = AF_INET;
	bindSockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bindSockaddr.sin_port = htons(5000);

	SOCKET listenSocket = WSASocket(AF_INET,
		SOCK_STREAM,IPPROTO_TCP,NULL,0,WSA_FLAG_OVERLAPPED);
	
	if (SOCKET_ERROR == bind(listenSocket, (struct sockaddr *)&bindSockAddr, len))
	{
		std::cout << "errorCode:" << WSAGetLastError() << std::endl;
		throw std::exception("bind failed.");
	}

	if (SOCKET_ERROR == listen(listenSocket, SOMAXCONN))
	{
		std::cout << "errorCode:" << WSAGetLastError() << std::endl;
		throw std::exception("listen failed.");
	}

	m_listenSocket = listenSocket;
	m_hIocp = createNewCompletionPort();
}

HANDLE IOCPServer::createNewCompletionPort(uint32_t dwThreads)
{
	return CreateIoCompletionPort(INVALID_HANDLE_VALUE,
		NULL, 0, dwThreads);
}

bool IOCPServer::associateDeviceWithCompletionPort(HANDLE hCompletionPort, HANDLE device, uint32_t dwCompletionKey)
{
	HANDLE h = CreateIoCompletionPort(device, hCompletionPort, dwCompletionKey);
	return (h == hCompletionPort);
}
