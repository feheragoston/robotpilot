#ifndef CONFIG_H_
#define CONFIG_H_

#include <iostream>
#include <string.h>

class Config {
public:
	char CanIp[16];
	char BindIp[16];
	char LuaFile[128];

	Config();
	virtual ~Config();

	bool Parse(int argc, char *argv[]);

private:
	bool isLua(char* name);
	bool isIp(char* name);
};

#endif /*CONFIG_H_*/
