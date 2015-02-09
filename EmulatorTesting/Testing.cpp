#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <Windows.h>
#include "Emulator.h"

int main(){
	//declare variables
	char *buffer;
	HANDLE handle;
	long fileSize;
	size_t result = 0;

	DCB dcbParam = {0};
	COMMTIMEOUTS timeouts = {0};
	int comport_num = 0;
	
	//open file
	FILE *ifp;
	ifp = fopen("test.c", "rb");
	if (ifp == NULL) {
		fprintf(stderr, "Can't open input filet!\n");
		exit(1);
	}

	//obtain file size
	fseek(ifp, 0, SEEK_END);
	fileSize = ftell(ifp);
	rewind(ifp);
	buffer = (char*)malloc(sizeof(char));

	//open comport
	handle = openComport(comport_num);
	if (handle == INVALID_HANDLE_VALUE){
		printf("CreateFile failed with error %d.\n", GetLastError());
		return 1;
	}

	//set dcp parameters and timeouts
	if (setParameters(dcbParam, handle) || setTimeouts(timeouts, handle)){
		printf("Error: %d.\n", GetLastError());
		return 1;
	}

	//write to comport
	while (result != fileSize){
		result = fread(&buffer, 1, 1, ifp);
		sendText(handle, buffer);
	}

	//close file and comport
	free(buffer);
	fclose(ifp);
	closeComport(handle);
}