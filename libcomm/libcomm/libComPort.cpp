// libComPort.cpp : Defines the exported functions for the DLL application.
//
#include "StdAfx.h"
#include <assert.h>
#define LIBCOMPORT
#include "libComPort.h"
ComPort::ComPort(void)
{
	m_hComm = INVALID_HANDLE_VALUE;
	m_bOpendComm = false;
}
ComPort::~ComPort(void)
{
}

bool ComPort::OpenComm( string strPortName, int nBaudRate, int nParity/*=0*/, int nByteSize/*=8*/, int nStopBits/*=ONESTOPBIT*/ )
{
	if(m_bOpendComm){
		return false;
	}
	DCB dcb;		
	BOOL fSuccess;// ���ڿ��ƿ�;
	COMMTIMEOUTS timeouts = {							// ���ڳ�ʱ���Ʋ���;
		MAXDWORD,										// ���ַ�������ʱʱ��: 100 ms;
		0,												// ������ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms);
		0,												// ������(������)����ʱʱ��: 500 ms;
		1,												// д����ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms);
		200};											// ������(������)д��ʱʱ��: 100 ms;

		//	::EnterCriticalSection(&m_csComm);
		// 	CString tempPort;
		// 	tempPort.Format(_T("\\\\.\\%s"),pszPortName);
		// 	string strPort;

		//�ַ�����ת��
#ifdef UNICODE
		const  char  *pFilePathName = strPortName.c_str();    

		int nLen = strlen(pFilePathName) + 1;      

		int nwLen = MultiByteToWideChar(CP_ACP, 0, pFilePathName, nLen, NULL,0); 
		TCHAR   temp[256];     
		MultiByteToWideChar(CP_ACP, 0, pFilePathName, nLen, temp, nwLen); 
		TCHAR   lpszFile[256];
		//wcscpy(lpszFile,TEXT("////.//"));
		wcscpy(lpszFile,temp);
#else
		char  lpszFile[256];
		strcpy(lpszFile,(char*)strPortName.c_str());
#endif
		m_hComm = CreateFile(lpszFile,					// �������ƻ��豸·��;
			GENERIC_READ | GENERIC_WRITE,				// ��д��ʽ;
			0,											// ������ʽ����ռ;
			NULL,										// Ĭ�ϵİ�ȫ������;
			OPEN_EXISTING,								// ������ʽ;
			0,											// ���������ļ�����;
			NULL);										// ��������ģ���ļ�;

		if(m_hComm == INVALID_HANDLE_VALUE)
		{
			m_bOpendComm = false;
			return FALSE;								// �򿪴���ʧ��;
		}
		// Set the event mask. 

		fSuccess = SetCommMask(m_hComm, EV_RXCHAR);

		m_overLapped.hEvent = CreateEvent(
			NULL,   // default security attributes 
			TRUE,   // manual-reset event 
			FALSE,  // not signaled 
			NULL    // no name
			);


		// Initialize the rest of the OVERLAPPED structure to zero.
		m_overLapped.Internal = 0;
		m_overLapped.InternalHigh = 0;
		m_overLapped.Offset = 0;
		m_overLapped.OffsetHigh = 0;

		assert(m_overLapped.hEvent);
		if (!fSuccess) 
		{
			// Handle the error. 
			printf("SetCommMask failed with error %d.\n", GetLastError());
			return false;
		}
		GetCommState(m_hComm, &dcb);					// ȡDCB;

		dcb.BaudRate = nBaudRate;
		dcb.ByteSize = nByteSize;
		dcb.Parity   = nParity;
		dcb.StopBits = (int)nStopBits;
		// 			dcb.XonLim = 0;
		// 			dcb.XoffLim = 0;
		// 			dcb.XonChar = (char) 11;
		// 			dcb.XoffChar= (char) 13;

		if(SetCommState(m_hComm, &dcb)==0)
		{
			DWORD dwErro = GetLastError();
			return false;	// ����DCB;
		}        
		if(SetupComm(m_hComm, 8192, 8192)==0)
		{
			return false;	// ��������������������С;
		}
		if(SetCommTimeouts(m_hComm, &timeouts)==0)
		{
			return false;	// ���ó�ʱ;
		}
		if(PurgeComm(m_hComm,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR)==0)
		{					//���ɾ�;
			return false;
		}
		m_bOpendComm = true;
		return true;
}

bool ComPort::CloseComm()
{
	bool b = true;
	if(true == m_bOpendComm)
	{
		b = CloseHandle(m_hComm);
		m_bOpendComm = false;
	}

	return b;
}

bool ComPort::WriteComm( void * pData,int nLength )
{
	DWORD dwNumWrite;    // ���ڷ��������ݳ���
	return WriteFile(m_hComm, pData, (DWORD)nLength, &dwNumWrite, NULL);
}

int ComPort::ReadComm( void * pData,int nLength )
{
	DWORD dwNumRead;    // �����յ������ݳ���;
	if(FALSE == ReadFile(m_hComm, pData, (DWORD)nLength, &dwNumRead, NULL))
	{
		int iErr = GetLastError();
		return 0;
	}
	return (int)dwNumRead;
}

void ComPort::ClearPort( int nMode )
{
	if( nMode == 0 )
	{
		PurgeComm(m_hComm, PURGE_RXCLEAR);
	}
	else
	{
		PurgeComm(m_hComm, PURGE_TXCLEAR);
	}
}

COMMEVENT ComPort::WaitCommPortEvent()
{
	COMMEVENT e = COMM_EV_UNDEF;
	DWORD dwEventMask = 0;
	
	if(WaitCommEvent(m_hComm,&dwEventMask,&m_overLapped)){
		if (dwEventMask & EV_DSR) {
			e = COMM_EV_DSR;
		}else if (dwEventMask & EV_CTS) {
			e = COMM_EV_CTS;
		}else if(dwEventMask & EV_RXCHAR){
			e = COMM_EV_RXCHAR;
		}
	}else{
		DWORD dwRet = GetLastError();
		if( ERROR_IO_PENDING == dwRet)
		{
			printf("I/O is pending...\n");
		}
		else {
			printf("Wait failed with error %d.\n", GetLastError());
		}
	}
	return e;
}