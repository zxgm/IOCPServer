
// ��Ϣ�¼�����
enum IOEvent
{
	NONE,
	RECV,
	SEND,
	ACCEPT,
	UNKNOWN
};

// �������ݻ���������
#define BUFFER_SIZE 1024

// �ص����ݽṹ
struct OVERLAPPED_DATA
{
	OVERLAPPED overlapped;
	WSABUF dataBuf;
	SOCKET s;						// ͨ��SOCKET
	//char wsaData[BUFFER_SIZE];	// ���ݽ��յ�ַ

	void initialize(SOCKET socket)
	{
		s = socket;
		reInitialize();
	}

	void reInitialize()
	{
		memset(&overlapped, 0, sizeof(OVERLAPPED));
		if (dataBuf.buf == NULL)
			mallocWSABuf();
		else
			memset(dataBuf.buf, 0, BUFFER_SIZE);
	}

	void mallocWSABuf(size_t malloc_size = BUFFER_SIZE)
	{
		dataBuf.buf = (char *)::calloc(malloc_size, sizeof(char));
		dataBuf.len = malloc_size;
	}

	void freeWSABuf()
	{	
		::free(dataBuf.buf);
		dataBuf.buf = NULL;
		dataBuf.len = 0;
	}
};
