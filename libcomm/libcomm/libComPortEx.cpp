
#include "stdafx.h"
#include <stdio.h>
#define LIBCOMPORT
#include "libComPortEx.h"
#include "libComPort.h"
#include <map>
using std::map;
static int s_fd = 0;
static map<int,ComPort *> mapComm;
int OpenComm(const char * chPortName,int nBaudRate, int nParity/*=0*/, int nByteSize/*=8*/, int nStopBits/*=ONESTOPBIT*/)
{
	ComPort * p = new ComPort;
	if(p->OpenComm(chPortName,nBaudRate,nParity,nByteSize,nStopBits)){
		s_fd ++;
		mapComm.insert(std::make_pair<int,ComPort*>(s_fd,p));
	}else{
		delete p;
		return COMM_ERR_CODE;
	}
	return s_fd;
}
int CloseComm(int fd)
{
	map<int,ComPort *>::iterator it = mapComm.find(fd);
	if(it != mapComm.end()){
		ComPort * p = it->second;
		p->CloseComm();
	}else{
		return COMM_ERR_CODE;
	}
	return fd;
}
int WriteComm(int fd,void * pData,int nLength)
{
	map<int,ComPort *>::iterator it = mapComm.find(fd);
	if(it != mapComm.end()){
		ComPort * p = it->second;
		if(!p->WriteComm(pData,nLength)){
			return COMM_ERR_CODE;
		}
	}else{
		return COMM_ERR_CODE;
	}
	return fd;
}
int ReadComm(int fd,void * pData,int nLength,int &outLen)
{
	map<int,ComPort *>::iterator it = mapComm.find(fd);
	if(it != mapComm.end()){
		ComPort * p = it->second;
		outLen = p->ReadComm(pData,nLength);
	}else{
		return COMM_ERR_CODE;
	}
	return fd;
}
int ClearPort(int fd,int mode)
{
	map<int,ComPort *>::iterator it = mapComm.find(fd);
	if(it != mapComm.end()){
		ComPort * p = it->second;
		p->ClearPort(mode);
	}else{
		return COMM_ERR_CODE;
	}
	return fd;
}

int WaitCommPortEvent(int fd)
{
	map<int,ComPort *>::iterator it = mapComm.find(fd);
	if(it != mapComm.end()){
		ComPort * p = it->second;
		return (int)p->WaitCommPortEvent();
	}else{
		return COMM_ERR_CODE;
	}
	return fd;
}
