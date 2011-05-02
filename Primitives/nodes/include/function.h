#ifndef FUNCTION_H_
#define FUNCTION_H_





//------------------------------ function ------------------------------
//0x00-0x05:	node specifikus parancs, es reply
//0x0E-0x0F:	node specifikus periodikus uzenet
//0x10-0x10:	pc es gateway altal is kuldheto parancs, csak node-nak
//0x1A-0x1F:	pc altal kuldheto parancs, ill. valamelyikre reply, node-nak es gateway-nek is


//---------- all node ----------
#define CMD_NODE_START_CANBOOTLOADER		0x10	//length = 0


//---------- all ----------
#define CMD_CORE_RESET						0x1A	//length = 0

#define CMD_KEEP_ALIVE						0x1B	//length = 0
#define CMD_SET_KEEP_ALIVE_MS				0x1C	//length = 2, data[0..1] = period_ms, mert u16 ertek

#define CMD_SET_SEND_PERIOD_TO_NODE_MS		0x1D	//length = 2, data[0..1] = period_ms, mert u16 ertek
#define CMD_SET_SEND_PERIOD_TO_PC_MS		0x1E	//length = 2, data[0..1] = period_ms, mert u16 ertek

#define CMD_PING							0x1F	//length = 0
#define MSG_PING_REPLY						0x1F	//length = 0


//---------- GATEWAY_ID ----------
#define CMD_GATEWAY_REMOVE_NODE				0x00	//length = 1, data[0] = node_id
#define CMD_GATEWAY_ADD_NODE_CANA			0x01	//length = 1, data[0] = node_id
#define CMD_GATEWAY_ADD_NODE_CANB			0x02	//length = 1, data[0] = node_id
#define CMD_GATEWAY_ADD_BOOTLOADER_ID		0x03	//length = 1, data[0] = node_id, amivel megegyezo CAN-re kell


//---------- CONSOLE_ID ----------
#define CMD_CONSOLE_STOP					0x00	//length = 0
#define MSG_CONSOLE_STOP_REPLY				0x00	//length = 0

#define CMD_CONSOLE_SET_POS					0x01	//length = 12, data[0..11] = pos, speed, acc, mert float ertekek
#define MSG_CONSOLE_SET_POS_REPLY			0x01	//length = 0

#define CMD_CONSOLE_CALIBRATE				0x02	//length = 0
#define MSG_CONSOLE_CALIBRATE_REPLY			0x02	//length = 0

#define MSG_CONSOLE_POS						0x0F	//length = 4, data[0..3] = pos, mert float ertek


//---------- DEADRECK_ID ----------
#define CMD_DEADRECK_RESET_POS				0x00	//length = 0
#define MSG_DEADRECK_RESET_POS_REPLY		0x00	//length = 0

#define MSG_DEADRECK_POS					0x0F	//length = 12, data[0..11] = x, y, phi, mert float ertekek


//---------- BDC_ID ----------
#define CMD_BDC_STOP						0x00	//length = 4, data[0..3] = acc, mert float ertek
#define MSG_BDC_STOP_REPLY					0x00	//length = 0

#define CMD_BDC_HARD_STOP					0x01	//length = 0
#define MSG_BDC_HARD_STOP_REPLY				0x01	//length = 0

#define CMD_BDC_GO							0x02	//length = 12, data[0..11] = distance, max_speed, max_acc, mert float ertekek
#define MSG_BDC_GO_REPLY					0x02	//length = 0

#define CMD_BDC_GOTO						0x03	//length = 16, data[0..15] = x, y, max_speed, max_acc, mert float ertekek
#define MSG_BDC_GOTO_REPLY					0x03	//length = 0

#define CMD_BDC_TURN						0x04	//length = 12, data[0..11] = angle, max_speed, max_acc, mert float ertekek
#define MSG_BDC_TURN_REPLY					0x04	//length = 0

#define CMD_BDC_SET_SPEED					0x05	//length = 8, data[0..7] = v, w, mert float ertekek
#define MSG_BDC_SET_SPEED_REPLY				0x05	//length = 0


//---------- INPUT_ID ----------
#define MSG_INPUT_DISTANCE					0x0E	//length = 12, data[0..11] = distance_num[0..5], mert u16 ertekek
#define MSG_INPUT_BUTTON					0x0F	//length = 1, data[0] = gombok bitenkent


//---------- MAGNET_ID ----------
#define CMD_MAGNET_SET_POLARITY				0x00	//length = 2, data[0] = num, data[1] = polarity //01: pull (vonz), 00: off (kikapcsolt), FF: push (taszit)
#define MSG_MAGNET_SET_POLARITY_REPLY		0x00	//length = 1, data[0] = num


//---------- SERVO_ID ----------
#define CMD_SERVO_SET_POS					0x00	//length = 13, data[0] = num, data[1..12] = pos, speed, acc, mert float ertekek
#define MSG_SERVO_SET_POS_REPLY				0x00	//length = 1, data[0] = num


//---------- SONAR_ID ----------
#define MSG_SONAR_POS						0x0F	//length = 8, data[0..7] = x, y, mert float ertekek


//---------- POWER_ID ----------
#define CMD_POWER_MOTOR_ON					0x00	//length = 0
#define MSG_POWER_MOTOR_ON_REPLY			0x00	//length = 0

#define CMD_POWER_MOTOR_OFF					0x01	//length = 0
#define MSG_POWER_MOTOR_OFF_REPLY			0x01	//length = 0

#define MSG_POWER_STOP_BUTTON				0x0F	//length = 1, data[0] = stop gomb
//------------------------------ function VEGE ------------------------------





#endif	//FUNCTION_H_
