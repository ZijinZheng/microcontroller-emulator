#include "rs232.h"
#include "sci.h"
#include "iRobot_program.h"

static int com_port_no;

void set_com_port(int com)
{
	com_port_no = com;
}

void byteTx(unsigned char byte)
{
	RS232_SendByte(com_port_no, (unsigned char)byte);
}

void iRobot_program()
{
//SONG_DEF 1,76,32,74,32,76,32,76,32,69,32,74,32

byteTx(CmdSong);
byteTx(1);
byteTx(6);

byteTx(76);
byteTx(32);
byteTx(74);
byteTx(32);
byteTx(76);
byteTx(32);
byteTx(76);
byteTx(32);
byteTx(69);
byteTx(32);
byteTx(74);
byteTx(32);

//SONG_PLAY 1

byteTx(CmdPlay);
byteTx(1);



}