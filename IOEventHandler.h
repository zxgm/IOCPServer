
class IOEventHandler
{
public:
	virtual void handleRecv();
	virtual void handleSend();
	virtual void handleAccept();
};