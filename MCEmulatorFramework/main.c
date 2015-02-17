#include <stdlib.h>
#include <stdio.h>
#include "rs232.h"
#include "iRobot_program.h"

void display_sensor_data(int interval)
{

}

int main()
{
	int i = 0,
		cport_nr = 3,
		bdrate = 57600;       // 57600 baud

	char mode[] = { '8', 'N', '1', 0 };

	if (RS232_OpenComport(cport_nr, bdrate, mode))
	{
		printf("Can not open comport\n");
		return(0);
	}	

	set_com_port(cport_nr);
	
	iRobot_program();
	RS232_CloseComport(cport_nr);

	return(0);
}