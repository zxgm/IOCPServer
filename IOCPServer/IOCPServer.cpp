#include "CommonHeader.h"

IOCPServer::IOCPServer()
{
	m_pWorkThread = new WorkThread();
}

IOCPServer::~IOCPServer()
{
}

void IOCPServer::initialize()
{
	try
	{
		initializeWinSock();
		initializeSocket();
		initializeThreadPool();
		acceptClient();
	}
	catch (...)
	{
		uninitialize();
		throw;
	}
}

void IOCPServer::uninitialize()
{
	WSACleanup();
	if (m_pWorkThread)
	{
		delete m_pWorkThread;
		m_pWorkThread = NULL;
	}

	CloseHandle(m_hIocp);
	closesocket(m_listenSocket);
}

void IOCPServer::acceptClient()
{
	OVERLAPPED_DATA *pOverlappedData = NULL;
	while (true)
	{
		sockaddr_in clientAddr;
		int addr_len = sizeof(sockaddr);

		SOCKET clientSocket = WSAAccept(
			m_listenSocket, 
			(sockaddr *)&clientAddr,
			&addr_len, 
			NULL,
			NULL
		);
		if (clientSocket == INVALID_SOCKET)
		{
			std::cout << "Accept socket failed." << std::endl;
			return;
		}

		std::cout << "远程连接成功.远程地址:" 
			<< inet_ntoa(clientAddr.sin_addr) << std::endl;

		pOverlappedData = new OVERLAPPED_DATA();
		pOverlappedData->initialize(clientSocket);
		/*
#ifdef _DEBUG
		std::cout << "地址:0x"<<std::hex<<pOverlappedData
			<< ";数据地址:0x" << static_cast<void *>(pOverlappedData->dataBuf.buf)
			<< std::endl;
#endif
*/
		associateDeviceWithCompletionPort(m_hIocp, 
			(HANDLE)clientSocket, 
			(uint32_t)pOverlappedData);
		
		DWORD flags = 0, recvBytes = 0;
		int ret = WSARecv(
				pOverlappedData->s,
				&pOverlappedData->dataBuf,
				1,
				&recvBytes,
				&flags,
				(LPWSAOVERLAPPED)&(pOverlappedData->overlapped),
				NULL
			);
		if (ret == SOCKET_ERROR && WSA_IO_PENDING != WSAGetLastError())
		{
			std::cout << "异步收取数据失败.错误码:" << WSAGetLastError() << std::endl;
			return;
		}
	}
}

void IOCPServer::initializeThreadPool()
{
	m_pWorkThread->createThreads(m_hIocp);
}

void IOCPServer::initializeWinSock()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		throw std::exception("WSAStartup failed.");
	}
}

void IOCPServer::initializeSocket()
{
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(5000);

	SOCKET listenSocket = WSASocket(
		AF_INET,
		SOCK_STREAM,
		IPPROTO_TCP,
		NULL,
		0,
		WSA_FLAG_OVERLAPPED
	);
	if (listenSocket == INVALID_SOCKET)
		throw std::exception("WSASocket failed.");

	if (SOCKET_ERROR == bind(listenSocket,
		(struct sockaddr *)&serverAddr, sizeof(sockaddr)))
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

bool IOCPServer::associateDeviceWithCompletionPort(HANDLE hCompletionPort,
	HANDLE device, uint32_t dwCompletionKey)
{
	HANDLE h = CreateIoCompletionPort(device,
		hCompletionPort, dwCompletionKey,0);
	return (h == hCompletionPort);
}
