/*
 * node_Drive.h
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */


#ifndef NODE_DRIVE_H_
#define NODE_DRIVE_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node_Drive ELEJE ------------------------------
class node_Drive : public node{

public:

	node_Drive(void);
	~node_Drive();

	//----- mindegyik node-nak van ELEJE -----
	void evalMsg(UDPmsg* msg);
	//----- mindegyik node-nak van VEGE -----


	//----- function ELEJE -----
public:

	void STOP(double acc);
	bool stop_inProgress;
	bool stop_finished;

	void GO(double distance, double max_speed, double max_acc);
	bool go_inProgress;
	bool go_finished;

	void GOTO(double x, double y, double max_speed, double max_acc);
	bool goto_inProgress;
	bool goto_finished;

	void TURN(double angle, double max_speed, double max_acc);
	bool turn_inProgress;
	bool turn_finished;

	void SET_SPEED(double v, double w);
	bool setspeed_inProgress;
	bool setspeed_finished;

private:

	static const unsigned int	CMD_STOP				= 0x00;		//length = 4, data[0] = acc, mert float ertek
	static const unsigned int	CMD_HARD_STOP			= 0x01;		//length = 0
	static const unsigned int	MSG_STOP_REPLY			= 0x00;		//length = 0

	static const unsigned int	CMD_GO					= 0x02;		//length = 12, data[0..11] = distance, max_speed, max_acc, mert float ertekek
	static const unsigned int	MSG_GO_REPLY			= 0x02;		//length = 0

	static const unsigned int	CMD_GOTO				= 0x03;		//length = 16, data[0..15] = x, y, max_speed, max_acc, mert float ertekek
	static const unsigned int	MSG_GOTO_REPLY			= 0x03;		//length = 0

	static const unsigned int	CMD_TURN				= 0x04;		//length = 12, data[0..11] = angle, max_speed, max_acc, mert float ertekek
	static const unsigned int	MSG_TURN_REPLY			= 0x04;		//length = 0

	static const unsigned int	CMD_SET_SPEED			= 0x05;		//length = 8, data[0..7] = v, w, mert float ertekek
	static const unsigned int	MSG_SET_SPEED_REPLY		= 0x05;		//length = 0
	//----- function VEGE -----


};
//------------------------------ node_Drive VEGE ------------------------------





#endif /*NODE_DRIVE_H_*/
