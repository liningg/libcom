// test_libcomm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "libComPortEx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int ret = 0;
	int fd = 0;
	fd = OpenComm("COM8",460800,0,8,0);
	if(fd == -1){
		printf("OpenComm faild\n");
		goto exit;
	}
	//蓝牙建立连接字节流
	byte data[] = {0xff,0x06,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x03};
	ret = WriteComm(fd,data,sizeof(data)/sizeof(byte));
	if(ret == -1){
		printf("WriteComm faild\n");
		goto exit;
	}
	while(true){
		int evt = WaitCommPortEvent(fd);
		if(evt == 2){
			byte dataStream[100] = {0};
			int len = 0;
			ret = ReadComm(fd,dataStream,100,len);
			if(ret == -1){
				printf("ReadComm faild\n");
				goto exit;
			}
			if(len > 0 ){
				for (int i = 0;i<len;i++)
				{
					printf("0x%02x,",dataStream[i]);
				}
				printf("\n");
			}else{
				printf("Read len is 0\n");
			}
		}else{
			printf ("Recv Other Event\n");
		}
	}
	CloseComm(fd);
exit:
	getchar();
	return 0;
}

