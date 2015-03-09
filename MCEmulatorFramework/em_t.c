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
void readSensor(uint8_t package);


/* ===== Functions for Microcontroller's C ===== */

// Transmit a byte over the serial port
void byteTx(uint8_t byte)
{
	int byteSent = RS232_SendByte(com_port_no, byte);
	printf("byteSent:%d, %02X\n", byteSent, byte);

	// Delay 10ms, otherwise some data will get lost.
	//Sleep(10);
}


// Delay for the specified time in ms without updating sensor values
void delay(uint16_t time_ms)
{
	Sleep(time_ms);
}


/* Read sensor data to sensors[] */
void readSensor(uint8_t package)
{
	//printf("Sensor package=%d\n", package);
	byteTx(CmdSensors);
	byteTx(package);

	//Sleep(30);

	int byteRead = 0;

	// For Cptimeouts.ReadIntervalTimeout = 0, block until all bytes are received.
	byteRead = RS232_ReadBuf(com_port_no, (unsigned char*)sensors, Sen0Size);
	printf("error: only %d bytes read. ", byteRead);

	/*
	// For Cptimeouts.ReadIntervalTimeout = MAXDWORD, keep reading until sensors[] is filled up.
	while (1)
	{
	byteRead += RS232_ReadBuf(com_port_no, (unsigned char*)sensors + byteRead, Sen0Size - byteRead);
	if (byteRead == Sen0Size)
	break;
	else
	{
	printf("%d bytes read, continue reading\n", byteRead);
	}
	}
	*/

	printf("byteRead:%d, ", byteRead);
	for (int i = 0; i < Sen0Size; i++)
	{
		printf("%02X ", sensors[i]);
	}
	printf("\n");
}

// Delay for the specified time in ms and update sensor values
void delaySensors(uint16_t time_ms)
{
	// clock_t begin = clock();
	Sleep(time_ms);
	readSensor(0);
	// Update running totals of distance and angle
	distance += (int16_t)((sensors[SenDist1] << 8) | sensors[SenDist0]);
	angle += (int16_t)((sensors[SenAng1] << 8) | sensors[SenAng0]);

	/*
	// Continuously check sensor data.
	// This make cause precision issue if velocity is too small - value less than 1 will be treated as 0.
	while ((clock() - begin) * 1000 / CLOCKS_PER_SEC  < time_ms)
	{
		readSensor(0);
		// Update running totals of distance and angle
		distance += (int16_t)((sensors[SenDist1] << 8) | sensors[SenDist0]);
		angle += (int16_t)((sensors[SenAng1] << 8) | sensors[SenAng0]);
	}
	*/
}






/* ===== Sample functions ===== */

/* Periodically read from sensor
t interval in millionsecond */
void periodically_read_sensor(int interval)
{
	while (1)
	{
		readSensor(0);
		for (int i = 0; i < Sen0Size; i++)
		{
			printf("%d ", sensors[i]);
		}
		printf("\n");
		Sleep(interval);
	}
}

/* Send the Roomba drive commands in terms of velocity and radius */
void drive(int16_t velocity, int16_t radius)
{
	byteTx(CmdDrive);
	byteTx((uint8_t)((velocity >> 8) & 0x00FF));
	byteTx((uint8_t)(velocity & 0x00FF));
	byteTx((uint8_t)((radius >> 8) & 0x00FF));
	byteTx((uint8_t)(radius & 0x00FF));
}

/* Rotate at place, positive velocity for counterclockwise. */
void rotate(int16_t velocity, int16_t dest_angle)
{
	drive(velocity, 1);
	angle = 0;
	while (1)
	{
		delaySensors(30);
		printf("angle: %d\n", angle);
		if (angle >= dest_angle || angle <= -dest_angle)
		{
			drive(0, 1);
			break;
		}
	}
}

/* Move in line, positive velocity for forward. */
void move(int16_t velocity, int16_t dest_distance)
{
	drive(velocity, RadStraight);
	distance = 0;
	while (1)
	{
		delaySensors(30);
		printf("distance: %d\n", distance);
		if (distance >= dest_distance)
		{
			drive(0, RadStraight);
			break;
		}
	}
}

void led(uint8_t bits, uint8_t color, uint8_t intensity)
{
	byteTx(CmdLeds);
	byteTx(bits);
	byteTx(color);
	byteTx(intensity);
}





/* ===== Public functions =====*/

void setComPort(int com)
{
	com_port_no = com;
}


void iRobot_program()
{
	// Initialize

	printf("Start\n");
	byteTx(CmdStart);

	printf("Full mode\n");
	byteTx(CmdFull);

	delay(100);
	// Commands

	drive(10, RadStraight);

	// rotate(-80, 90);

	led(10, 255, 255);

	//move(300, 300);

	//periodically_read_sensor(1000);

	/** main program **/


}