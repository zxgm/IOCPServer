#include "CommonHeader.h"

Utils* Utils::getInstance()
{
	static Utils *instance = new Utils();
	return instance;
}

void Utils::createCompletionPort()
{
	HANDLE completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,
		NULL,0,0);

}

void Utils::associateDevices()
{

}

