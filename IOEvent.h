
#include "CommonHeader.h"

// ��Ϣ�¼�����
enum IOEvent
{
	NONE,
	RECV,
	SEND,
	ACCEPT,
	UNKNOWN
};

// �ص����ݽṹ
struct OVERLAPPED_DATA
{
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	SOCKET socket;
};
