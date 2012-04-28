#include "nokia_server.h"
//#include <iostream>
#include <sstream>

nokia_server::nokia_server() {
	//ctor
	this->address_len = sizeof(this->address);
	this->sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	bzero(&this->address, this->address_len);
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = inet_addr("192.168.2.15");
	this->address.sin_port = htons(2001);
	this->instant_send = true;
}

nokia_server::~nokia_server() {
	//dtor
}

void nokia_server::set_location(int x, int y, int phi) {
	std::string out = "<location x=\"" + this->from_int(x) + "\" y=\"" + this->from_int(y) + "\" phi=\"" + this->from_int(phi) + "\" />\n";
	this->current_xml += out;
	if (this->instant_send)
		this->do_send();
}
void nokia_server::set_speed(int left_wheel, int right_wheel) {
	std::string out = "<speed vl=\"" + this->from_int(left_wheel) + "\" vr=\"" + this->from_int(right_wheel) + "\" />\n";
	this->current_xml += out;
	if (this->instant_send)
		this->do_send();
}
void nokia_server::set_state(robot_state state) {
	std::string out;
	if (state == OPENED)
		out = "<state name=\"opened\" />\n";
	else
		out = "<state name=\"closed\" />\n";
	this->current_xml += out;
	if (this->instant_send)
		this->do_send();
}

void nokia_server::set_opponent(std::string name, int x, int y, int phi, int r) {
	std::string out = "<opponent name=\"" + name + "\" x=\"" + this->from_int(x) + "\" y=\"" + this->from_int(y) + "\" phi=\"" + this->from_int(phi)
			+ "\" r=\"" + this->from_int(r) + "\" />\n";
	this->current_xml += out;
	if (this->instant_send)
		this->do_send();
}

void nokia_server::send_message(std::string msg, msg_type type, int alert_interval) {
	std::string types;
	if (type == NONE)
		types = "";
	else if (type == INFO)
		types = "info";
	else if (type == WARNING)
		types = "warning";
	else if (type == ERROR)
		types = "error";

	std::string out = "<message type=\"" + types + "\" text=\"" + msg + "\" duration=\"" + this->from_int(alert_interval) + "\" />\n";
	this->current_xml += out;
	if (this->instant_send)
		this->do_send();
}

void nokia_server::send_variable(std::string name, std::string value) {
	std::string out = "<variable name=\"" + name + "\" value=\"" + value + "\" />\n";
	this->current_xml += out;
	if (this->instant_send)
		this->do_send();
}

void nokia_server::clear_opponents() {
	std::string out = "<clear_opponents />\n";
	this->current_xml += out;
	if (this->instant_send)
		this->do_send();
}

void nokia_server::pause_sending() {
	this->instant_send = false;
}

void nokia_server::send() {
	this->do_send();
	this->instant_send = true;
}

void nokia_server::do_send() {
	std::string xmlbegin = "<command>\n";
	std::string xmlend = "</command>\n";

	if (this->current_xml.size() == 0)
		return;

	std::string out = xmlbegin + this->current_xml + xmlend;

	sendto(sd, out.c_str(), out.size() + 1, MSG_DONTWAIT, (struct sockaddr*) &address, address_len);
	this->current_xml.clear();
	//  std::cout << out;
}

std::string nokia_server::from_int(int n) {
	std::string re;
	std::stringstream s;
	s << n;
	re = s.str();
	return re;
}
