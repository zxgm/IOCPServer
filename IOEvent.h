
#include "CommonHeader.h"

// 消息事件类型
enum IOEvent
{
	NONE,
	RECV,
	SEND,
	ACCEPT,
	UNKNOWN
};

// 重叠数据结构
struct OVERLAPPED_DATA
{
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	SOCKET socket;
};
