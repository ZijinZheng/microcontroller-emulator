#include <stdio.h>
#include <Windows.h>

HANDLE openComport(int);
int setParameters(DCB, HANDLE);
int setTimeouts(COMMTIMEOUTS, HANDLE);
int pollComport(HANDLE, unsigned char*, int);
int sendByte(HANDLE, unsigned char);
int sendBuf(HANDLE, unsigned char, int);
void sendText(HANDLE, char*);
int closeComport(HANDLE);