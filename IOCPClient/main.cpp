
#include <WinSock2.h>
#include <iostream>
#include <string>
#pragma comment(lib,"ws2_32.lib")

#define COCURRENCY_NUM 10000
int main(int argc, char *argv[])
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	/*
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(5000);
	*/

	// 模拟多并发连接
	for (int i = 0; i < COCURRENCY_NUM; i++)
	{
		SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		struct sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		serverAddr.sin_port = htons(5000);

		int iRet = connect(s, (struct sockaddr *)&serverAddr, sizeof(sockaddr));
		if (iRet == SOCKET_ERROR)
		{
			std::cout << "connect failed.Error code " << WSAGetLastError() << std::endl;
			return -1;
		}

		char sendData[64];
		int len = sprintf(sendData, "client%d send data.", i);
		sendData[len] = '\0';
		send(s, sendData, len, 0);
	}

	std::cin.get();
	/*
	int iRet = connect(s, (struct sockaddr *)&serverAddr, sizeof(sockaddr));
	if (iRet == SOCKET_ERROR)
	{
		std::cout << "connect failed.Error code " << WSAGetLastError() << std::endl;
		return -1;
	}

	std::string input_str = "";
	while (std::getline(std::cin,input_str))
	{
		if (input_str.compare("exit") == 0)
			break;
		send(s, input_str.c_str(), input_str.length(), 0);
	}
	*/

	return 0;
}