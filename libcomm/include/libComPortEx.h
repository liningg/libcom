
#ifdef LIBCOMPORT
#define LIBEXPORT __declspec( dllexport)
#else
#define LIBEXPORT __declspec( dllimport)
#endif
#define COMM_ERR_CODE  -1
#ifdef __cplusplus
extern "C"{
#endif
/*
*	打开端口
*/
LIBEXPORT int OpenComm(const char * chPortName,int nBaudRate, int nParity/*=0*/, int nByteSize/*=8*/, int nStopBits/*=ONESTOPBIT*/); 
/*
*	关闭端口
*/
LIBEXPORT int CloseComm(int fd);
/*
*	写串口
*/
LIBEXPORT int WriteComm(int fd,void * pData,int nLength);
/*
*	读串口
*/
LIBEXPORT int ReadComm(int fd,void * pData,int nLength,int &outLen);
/*
*	刷新窗口
*/
LIBEXPORT int ClearPort(int fd,int mode);
/*
*	串口事件
*/
LIBEXPORT int WaitCommPortEvent(int fd);


#ifdef __cplusplus
}
#endif