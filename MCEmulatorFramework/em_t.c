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



void set_com_port(int com)
{
	com_port_no = com;
}


// Private functions

void read_sensor(uint8_t package)
{
	//printf("Sensor package=%d\n", package);
	byteTx(CmdSensors);
	byteTx(package);

	//Sleep(30);

	int byteRead = RS232_PollComport(com_port_no, (unsigned char*)sensors, Sen0Size);
	printf("byteRead:%d,", byteRead);
	for (int i = 0; i < Sen0Size; i++)
	{
		printf("%d ", sensors[i]);
	}
	printf("\n");
}

// Functions for Microcontroller's C

void byteTx(uint8_t byte)
{
	int byteSent = RS232_SendByte(com_port_no, byte);
	//printf("byteSent:%d,", byteSent);
	Sleep(20);
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
	Sleep(time_ms);
	read_sensor(0);
	// Update running totals of distance and angle
	distance += (int16_t)((sensors[SenDist1] << 8) | sensors[SenDist0]);
	angle += (int16_t)((sensors[SenAng1] << 8) | sensors[SenAng0]);

	while ((clock() - begin) * 1000 / CLOCKS_PER_SEC  < time_ms)
	{
		read_sensor(0);
		// Update running totals of distance and angle
		distance += (int16_t)((sensors[SenDist1] << 8) | sensors[SenDist0]);
		angle += (int16_t)((sensors[SenAng1] << 8) | sensors[SenAng0]);
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
	printf("Start\n");
	byteTx(128);

	printf("Full mode\n");
	byteTx(132);

	printf("Drive 50, RadStraight\n");
	byteTx(137);
	byteTx(0);
	byteTx(100);
	byteTx(0);
	byteTx(1);
	//byteTx(RadStraight);	

	angle = 0;
	while (1)
	{		
		delaySensors(30);
		printf("distance: %d\n", angle);
		if (angle > 90)
		{
			byteTx(137);
			byteTx(00);
			byteTx(0);
			byteTx(0x7F);
			byteTx(0xFF);
			break;
		}
	}


	//periodically_read_sensor(1000);


}