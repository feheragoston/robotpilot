#include "Config.h"

Config::Config() {
	strcpy(CanIp, "");
	strcpy(NetIp, "");
	strcpy(BindIp, "");
	strcpy(LuaFile, "");

	PrimitivesCan = true;
	PrimitivesNet = false;

	DebugSimulation = false;
}

bool Config::Parse(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'h': // help
				std::cout << "Command line parameters:" << std::endl;
				std::cout << "	-h	This message" << std::endl;
				std::cout << "	-s	Sim Mode" << std::endl;
				std::cout << "	-c [ip_address]	CAN Mode" << std::endl;
				std::cout << "	-n [ip_address]	Net Mode" << std::endl;
				std::cout << "	-l <lua/file.lua>	lua file to run" << std::endl;
				std::cout << "	-b <ip_address>	Bind server on ip" << std::endl;
				std::cout << "	-ds	Debug simulation" << std::endl;
				return false;
				break;
			case 's':
				std::cout << "Sim Mode" << std::endl;
				PrimitivesCan = false;
				PrimitivesNet = false;

				DebugSimulation = true;
				break;
			case 'c':
				std::cout << "CAN Mode";
				if (i + 1 < argc) {
					if (isIp(argv[i + 1])) {
						strcpy(CanIp, argv[i + 1]);
						i++;
						std::cout << ": " << CanIp;
					}
				}
				std::cout << std::endl;
				PrimitivesCan = true;
				PrimitivesNet = false;
				break;
			case 'n':
				std::cout << "Net Mode";
				if (i + 1 < argc) {
					if (isIp(argv[i + 1])) {
						strcpy(NetIp, argv[i + 1]);
						i++;
						std::cout << ": " << NetIp;
					}
				}
				std::cout << std::endl;
				PrimitivesNet = true;
				PrimitivesCan = false;
				break;
			case 'l':
				if (i + 1 < argc) {
					if (isLua(argv[i + 1])) {
						strcpy(LuaFile, argv[i + 1]);
						i++;
						std::cout << "Lua file: " << LuaFile << std::endl;
					} else {
						std::cout << "Error: invalid lua file" << std::endl;
						return false;
					}
				} else {
					std::cout << "Error: no lua file specified" << std::endl;
					return false;
				}
				break;
			case 'b':
				if (i + 1 < argc) {
					if (isIp(argv[i + 1])) {
						strcpy(BindIp, argv[i + 1]);
						i++;
						std::cout << "Bind on " << BindIp << std::endl;
					} else {
						std::cout << "Error: invalid bind ip" << std::endl;
						return false;
					}
				} else {
					std::cout << "Error: no bind ip specified" << std::endl;
					return false;
				}
				break;
			case 'd':
				if (strlen(argv[i]) == 3) {
					if (argv[i][2] == 's') {
						DebugSimulation = true;
						std::cout << "Simulation with debug delay!" << std::endl;
					}
				}
				break;
			default:
				std::cout << "Unknown command line parameter: " << argv[i][1] << std::endl;
				return false;
				break;
			}
		}
	}
	return true;
}

bool Config::isLua(char* name) {
	int len = strlen(name);
	if (len >= 128) {
		return false;
	}
	if (!strcmp(&(name[len - 4]), ".lua")) {
		return true;
	}
	return false;
}

bool Config::isIp(char* name) {
	if (strlen(name) > 15) {
		return false;
	}
	for (unsigned int i = 0; i < strlen(name); i++) {
		if ((name[i] < '0' || name[i] > '9') && name[i] != '.') {
			return false;
		}
	}
	return true;
}

Config::~Config() {
}
