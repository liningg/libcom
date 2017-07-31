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
	*	�򿪴���
	*/
	bool OpenComm(string strPortName, int nBaudRate, int nParity=0, int nByteSize=8, int nStopBits=ONESTOPBIT);
	/*
	*	�رմ���
	*/
	bool CloseComm();
	/*
	*	д����
	*/
	bool WriteComm(void * pData,int nLength);
	/*
	*	������
	*/
	int ReadComm(void * pData,int nLength);
	/*
	*	����������
	*/
	void ClearPort(int nMode);
	/*
	*	�ȴ������¼�
	*/
	COMMEVENT WaitCommPortEvent();
private:
	HANDLE m_hComm;
	bool m_bOpendComm;
	OVERLAPPED m_overLapped;
};

