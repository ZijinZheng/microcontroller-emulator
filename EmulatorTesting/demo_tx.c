#include <stdlib.h>
#include <stdio.h>
#include "rs232.h"

int main()
{
	int i = 0,
		cport_nr = 2,
		bdrate = 57600;       // 57600 baud

	unsigned char mode[] = { '8', 'N', '1', 0 };

	char testString[] = "139 10 255 128";
	int byteToSent;
	char * pch;

	if (RS232_OpenComport(cport_nr, bdrate, mode))
	{
		printf("Can not open comport\n");
		return(0);
	}
	RS232_SendByte(cport_nr, 128);
	RS232_SendByte(cport_nr, 132);

	pch = strtok(testString, " ");
	while (pch != NULL)
	{
		byteToSent = atoi(pch);
		RS232_SendByte(cport_nr, (unsigned char)byteToSent);
		printf("%d\n", byteToSent);
		pch = strtok(NULL, " ");
	}

	return(0);
}