
class Utils
{
public:
	static Utils* getInstance();

	void createCompletionPort();
	void associateDevices();

private:
	Utils(){}
	~Utils(){}
};