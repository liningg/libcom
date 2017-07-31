
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
*	�򿪶˿�
*/
LIBEXPORT int OpenComm(const char * chPortName,int nBaudRate, int nParity/*=0*/, int nByteSize/*=8*/, int nStopBits/*=ONESTOPBIT*/); 
/*
*	�رն˿�
*/
LIBEXPORT int CloseComm(int fd);
/*
*	д����
*/
LIBEXPORT int WriteComm(int fd,void * pData,int nLength);
/*
*	������
*/
LIBEXPORT int ReadComm(int fd,void * pData,int nLength,int &outLen);
/*
*	ˢ�´���
*/
LIBEXPORT int ClearPort(int fd,int mode);
/*
*	�����¼�
*/
LIBEXPORT int WaitCommPortEvent(int fd);


#ifdef __cplusplus
}
#endif