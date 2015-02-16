#include <Windows.h>
#include <time.h>
#include <stdint.h>

#include "rs232.h"
#include "sci.h"
#include "iRobot_program.h"



static int com_port_no;


volatile uint8_t sensors_in[Sen0Size];
volatile uint8_t sensors[Sen0Size];
volatile int16_t distance = 0;
volatile int16_t angle = 0;

volatile uint8_t read_buf;


void byteTx(uint8_t byte);
void delay(uint16_t time_ms);
void delaySensors(uint16_t time_ms);


uint8_t RS232_ReadByte()
{
	RS232_PollComport(com_port_no, (unsigned char*)&read_buf, 1);
	return read_buf;
}


void set_com_port(int com)
{
	com_port_no = com;
}


// Private functions

void read_sensor(uint8_t package)
{
	byteTx(CmdSensors);
	byteTx(package);

	RS232_PollComport(com_port_no, (unsigned char*)sensors, Sen0Size);
}

// Functions for Microcontroller's C

void byteTx(uint8_t byte)
{
	RS232_SendByte(com_port_no, byte);
}


// Delay for the specified time in ms without updating sensor values
void delay(uint16_t time_ms)
{
	Sleep(time_ms);
}


// Delay for the specified time in ms and update sensor values
void delaySensors(uint16_t time_ms)
{
	clock_t begin = clock();
	while ((clock() - begin) / CLOCKS_PER_SEC < time_ms)
	{
		read_sensor(0);

		// Update running totals of distance and angle
		distance += (int)((sensors[SenDist1] << 8) | sensors[SenDist0]);
		angle += (int)((sensors[SenAng1] << 8) | sensors[SenAng0]);
	}
}


/* ===== testing functions =====*/
/* Periodically read from sensor
t interval in millionsecond */
void periodically_read_sensor(int interval)
{
	while (1)
	{
		read_sensor(0);
		for (int i = 0; i < Sen0Size; i++)
		{
			printf("%d ", sensors[i]);
		}
		printf("\n");
		Sleep(interval);
	}
}

void iRobot_program()
{
	periodically_read_sensor(1000);


}