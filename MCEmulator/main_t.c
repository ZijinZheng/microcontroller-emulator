#include <stdlib.h>
#include <stdio.h>
#include "rs232.h"
#include "iRobot_program.h"

int main()
{
	/*
	iRobot Serial Port Settings
	Baud: 57600 or 19200 (see below)
	Data bits: 8
	Parity: None
	Stop bits: 1
	Flow control: None	
	*/
	int i = 0,
		cport_nr = 6,
		bdrate = 57600;       // 57600 baud

	// Change the com port
	cport_nr = /**change_com_port**/;
	
	char mode[] = { '8', 'N', '1', 0 };

	if (RS232_OpenComport(cport_nr, bdrate, mode))
	{
		printf("An error happened.\n");
		system("pause");
		return(0);
	}	

	setComPort(cport_nr);
	
	iRobot_program();
	RS232_CloseComport(cport_nr);

	return(0);
}