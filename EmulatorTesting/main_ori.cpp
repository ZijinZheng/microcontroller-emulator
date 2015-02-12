#include <stdio.h>
#include <Windows.h>

char comports[38][16] = { "/dev/ttyS0", "/dev/ttyS1", "/dev/ttyS2", "/dev/ttyS3", "/dev/ttyS4", "/dev/ttyS5",
"/dev/ttyS6", "/dev/ttyS7", "/dev/ttyS8", "/dev/ttyS9", "/dev/ttyS10", "/dev/ttyS11",
"/dev/ttyS12", "/dev/ttyS13", "/dev/ttyS14", "/dev/ttyS15", "/dev/ttyUSB0",
"/dev/ttyUSB1", "/dev/ttyUSB2", "/dev/ttyUSB3", "/dev/ttyUSB4", "/dev/ttyUSB5",
"/dev/ttyAMA0", "/dev/ttyAMA1", "/dev/ttyACM0", "/dev/ttyACM1",
"/dev/rfcomm0", "/dev/rfcomm1", "/dev/ircomm0", "/dev/ircomm1",
"/dev/cuau0", "/dev/cuau1", "/dev/cuau2", "/dev/cuau3",
"/dev/cuaU0", "/dev/cuaU1", "/dev/cuaU2", "/dev/cuaU3" };

HANDLE openComport(int comport_num){
	if (comport_num > 37 || comport_num < 0){
		printf("invalid comport number");
		exit(1);
	}

	printf("Opening serial port...");
	HANDLE handle = CreateFile(comports[comport_num], GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	return handle;
}


int setParameters(DCB dcbParam, HANDLE handle){
	dcbParam.DCBlength = sizeof(dcbParam);
	if (GetCommState(handle, &dcbParam) == 0){
		printf("Error getting device state\n");
		CloseHandle(handle);
		return 1;
	}
	dcbParam.BaudRate = CBR_38400;
	dcbParam.ByteSize = 8;
	dcbParam.StopBits = ONESTOPBIT;
	dcbParam.Parity = NOPARITY;
	if (SetCommState(handle, &dcbParam) == 0){
		printf("Error setting device parameters\n");
		CloseHandle(handle);
		return 1;
	}
}

int setTimeouts(COMMTIMEOUTS timeouts, HANDLE handle){
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	if (SetCommTimeouts(handle, &timeouts) == 0){
		printf("Error setting timeouts\n");
		CloseHandle(handle);
		return 1;
	}
}

int pollComport(HANDLE handle, unsigned char *buf, int size){
	int n;
	ReadFile(handle, buf, size, (LPDWORD)((void *)&n), NULL);
	return n;
}


int sendByte(HANDLE handle, unsigned char byte){
	int n;
	if (!WriteFile(handle, &byte, 1, (LPDWORD)((void *)&n), NULL)){
		printf("Error%d\n", GetLastError());
		CloseHandle(handle);
		return 1;
	}
	return 0;
}

int sendBuf(HANDLE handle, unsigned char *buf, int size){
	int n;
	if (WriteFile(handle, buf, size, (LPDWORD)((void *)&n), NULL))
		return n;
	return 0;
}

void sendText(HANDLE handle, char *text){
	while (*text != 0)
		sendByte(handle, *(text++));
}

int closeComport(HANDLE handle){
	CloseHandle(handle);
	if (CloseHandle(handle) == 0)
		return 1;
	return 0;
}