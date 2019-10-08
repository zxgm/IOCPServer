#include "CommonHeader.h"

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	int32_t ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		std::cout << "WSAStartup failed." << std::endl;
		return -1;
	}



	

	WSACleanup();

	std::cin.get();
	return 0;
}