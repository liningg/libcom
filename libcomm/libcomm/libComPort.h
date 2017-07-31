#pragma once
#include <string>
#include <windows.h>
using std::string;
typedef enum COMMEVENT
{
	COMM_EV_UNDEF = -1,
	COMM_EV_DSR = 0,
	COMM_EV_CTS,
	COMM_EV_RXCHAR,
};
 class  ComPort
{
public:
	ComPort(void);
	~ComPort(void);

	/*
	*	打开串口
	*/
	bool OpenComm(string strPortName, int nBaudRate, int nParity=0, int nByteSize=8, int nStopBits=ONESTOPBIT);
	/*
	*	关闭串口
	*/
	bool CloseComm();
	/*
	*	写串口
	*/
	bool WriteComm(void * pData,int nLength);
	/*
	*	读串口
	*/
	int ReadComm(void * pData,int nLength);
	/*
	*	清理串口数据
	*/
	void ClearPort(int nMode);
	/*
	*	等待串口事件
	*/
	COMMEVENT WaitCommPortEvent();
private:
	HANDLE m_hComm;
	bool m_bOpendComm;
	OVERLAPPED m_overLapped;
};

