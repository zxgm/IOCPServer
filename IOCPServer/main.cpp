#include "CommonHeader.h"

int main(int argc, char *argv[])
{
	IOCPServer iocpServer;
	iocpServer.initialize();

	std::cin.get();
	return 0;
}