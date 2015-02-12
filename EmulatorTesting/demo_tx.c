
/**************************************************

file: demo_tx.c
purpose: simple demo that transmits characters to
the serial port and print them on the screen,
exit the program by pressing Ctrl-C

compile with the command: gcc demo_tx.c rs232.c -Wall -Wextra -o2 -o test_tx

**************************************************/

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"



int main()
{
	int i = 0,
		cport_nr = 3,        /* /dev/ttyS0 (COM1 on windows) */
		bdrate = 57600;       /* 9600 baud */

	unsigned char mode[] = { '8', 'N', '1', 0 },
		str[2][512];
		

	str[0][0] = 139;
	str[0][1] = 10;
	str[0][2] = 0;
	str[0][3] = 128;
	str[0][4] = 0;

	unsigned char color = 0;
	// strcpy(str[0], "The quick brown fox jumped over the lazy grey dog.\n");

	//strcpy(str[1], "Happy serial programming!\n");


	if (RS232_OpenComport(cport_nr, bdrate, mode))
	{

		printf("Can not open comport\n");

		return(0);
	}


	RS232_SendByte(cport_nr, 128);
	RS232_SendByte(cport_nr, 132);
	//RS232_cputs(cport_nr, str[i]);

	while (1)
	{
		color = (unsigned char)255 - color;
		RS232_SendByte(cport_nr, 139);
		RS232_SendByte(cport_nr, 10);
		RS232_SendByte(cport_nr, color);
		RS232_SendByte(cport_nr, 255);
		Sleep(1000);
	}

	printf("sent: %s\n", str[i]);

#ifdef _WIN32
	Sleep(1000);
#else
	usleep(1000000);  /* sleep for 1 Second */
#endif


	return(0);
}

