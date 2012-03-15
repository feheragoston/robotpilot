#!/bin/bash

cd Build

mkdir -p ./lua
mkdir -p ./Obstacles
mkdir -p ./Pilot
mkdir -p ./Pilot/old
mkdir -p ./Primitives
mkdir -p ./Primitives/nodes
mkdir -p ./Primitives/Net
mkdir -p ./Primitives/UDP

g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Config.d" -MT"Config.d" -o"Config.o" "../Config.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Control.d" -MT"Control.d" -o"Control.o" "../Control.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Server.d" -MT"Server.d" -o"Server.o" "../Server.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"geom.d" -MT"geom.d" -o"geom.o" "../geom.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"robotpilot.d" -MT"robotpilot.d" -o"robotpilot.o" "../robotpilot.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"tools.d" -MT"tools.d" -o"tools.o" "../tools.cpp"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/lapi.d" -MT"lua/lapi.d" -o"lua/lapi.o" "../lua/lapi.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/lauxlib.d" -MT"lua/lauxlib.d" -o"lua/lauxlib.o" "../lua/lauxlib.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/lbaselib.d" -MT"lua/lbaselib.d" -o"lua/lbaselib.o" "../lua/lbaselib.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/lcode.d" -MT"lua/lcode.d" -o"lua/lcode.o" "../lua/lcode.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/ldblib.d" -MT"lua/ldblib.d" -o"lua/ldblib.o" "../lua/ldblib.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/ldebug.d" -MT"lua/ldebug.d" -o"lua/ldebug.o" "../lua/ldebug.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/ldo.d" -MT"lua/ldo.d" -o"lua/ldo.o" "../lua/ldo.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/ldump.d" -MT"lua/ldump.d" -o"lua/ldump.o" "../lua/ldump.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/lfunc.d" -MT"lua/lfunc.d" -o"lua/lfunc.o" "../lua/lfunc.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/lgc.d" -MT"lua/lgc.d" -o"lua/lgc.o" "../lua/lgc.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/linit.d" -MT"lua/linit.d" -o"lua/linit.o" "../lua/linit.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/liolib.d" -MT"lua/liolib.d" -o"lua/liolib.o" "../lua/liolib.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/llex.d" -MT"lua/llex.d" -o"lua/llex.o" "../lua/llex.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/lmathlib.d" -MT"lua/lmathlib.d" -o"lua/lmathlib.o" "../lua/lmathlib.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/lmem.d" -MT"lua/lmem.d" -o"lua/lmem.o" "../lua/lmem.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/loadlib.d" -MT"lua/loadlib.d" -o"lua/loadlib.o" "../lua/loadlib.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/lobject.d" -MT"lua/lobject.d" -o"lua/lobject.o" "../lua/lobject.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/lopcodes.d" -MT"lua/lopcodes.d" -o"lua/lopcodes.o" "../lua/lopcodes.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/loslib.d" -MT"lua/loslib.d" -o"lua/loslib.o" "../lua/loslib.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/lparser.d" -MT"lua/lparser.d" -o"lua/lparser.o" "../lua/lparser.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/lstate.d" -MT"lua/lstate.d" -o"lua/lstate.o" "../lua/lstate.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/lstring.d" -MT"lua/lstring.d" -o"lua/lstring.o" "../lua/lstring.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/lstrlib.d" -MT"lua/lstrlib.d" -o"lua/lstrlib.o" "../lua/lstrlib.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/ltable.d" -MT"lua/ltable.d" -o"lua/ltable.o" "../lua/ltable.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/ltablib.d" -MT"lua/ltablib.d" -o"lua/ltablib.o" "../lua/ltablib.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/ltm.d" -MT"lua/ltm.d" -o"lua/ltm.o" "../lua/ltm.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/lundump.d" -MT"lua/lundump.d" -o"lua/lundump.o" "../lua/lundump.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/lvm.d" -MT"lua/lvm.d" -o"lua/lvm.o" "../lua/lvm.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/lzio.d" -MT"lua/lzio.d" -o"lua/lzio.o" "../lua/lzio.c"
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"lua/print.d" -MT"lua/print.d" -o"lua/print.o" "../lua/print.c"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/nodes/node.d" -MT"Primitives/nodes/node.d" -o"Primitives/nodes/node.o" "../Primitives/nodes/node.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/nodes/node_BDC.d" -MT"Primitives/nodes/node_BDC.d" -o"Primitives/nodes/node_BDC.o" "../Primitives/nodes/node_BDC.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/nodes/node_Broadcast.d" -MT"Primitives/nodes/node_Broadcast.d" -o"Primitives/nodes/node_Broadcast.o" "../Primitives/nodes/node_Broadcast.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/nodes/node_Console.d" -MT"Primitives/nodes/node_Console.d" -o"Primitives/nodes/node_Console.o" "../Primitives/nodes/node_Console.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/nodes/node_Deadreck.d" -MT"Primitives/nodes/node_Deadreck.d" -o"Primitives/nodes/node_Deadreck.o" "../Primitives/nodes/node_Deadreck.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/nodes/node_Gateway.d" -MT"Primitives/nodes/node_Gateway.d" -o"Primitives/nodes/node_Gateway.o" "../Primitives/nodes/node_Gateway.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/nodes/node_Input.d" -MT"Primitives/nodes/node_Input.d" -o"Primitives/nodes/node_Input.o" "../Primitives/nodes/node_Input.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/nodes/node_Magnet.d" -MT"Primitives/nodes/node_Magnet.d" -o"Primitives/nodes/node_Magnet.o" "../Primitives/nodes/node_Magnet.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/nodes/node_Power.d" -MT"Primitives/nodes/node_Power.d" -o"Primitives/nodes/node_Power.o" "../Primitives/nodes/node_Power.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/nodes/node_Servo.d" -MT"Primitives/nodes/node_Servo.d" -o"Primitives/nodes/node_Servo.o" "../Primitives/nodes/node_Servo.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/nodes/node_Sonar.d" -MT"Primitives/nodes/node_Sonar.d" -o"Primitives/nodes/node_Sonar.o" "../Primitives/nodes/node_Sonar.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/nodes/time.d" -MT"Primitives/nodes/time.d" -o"Primitives/nodes/time.o" "../Primitives/nodes/time.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/UDP/UDPconn.d" -MT"Primitives/UDP/UDPconn.d" -o"Primitives/UDP/UDPconn.o" "../Primitives/UDP/UDPconn.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/UDP/UDPdriver.d" -MT"Primitives/UDP/UDPdriver.d" -o"Primitives/UDP/UDPdriver.o" "../Primitives/UDP/UDPdriver.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/UDP/conv.d" -MT"Primitives/UDP/conv.d" -o"Primitives/UDP/conv.o" "../Primitives/UDP/conv.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/Primitives.d" -MT"Primitives/Primitives.d" -o"Primitives/Primitives.o" "../Primitives/Primitives.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/PrimitivesCan.d" -MT"Primitives/PrimitivesCan.d" -o"Primitives/PrimitivesCan.o" "../Primitives/PrimitivesCan.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/PrimitivesNet.d" -MT"Primitives/PrimitivesNet.d" -o"Primitives/PrimitivesNet.o" "../Primitives/PrimitivesNet.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Primitives/Net/Net.d" -MT"Primitives/Net/Net.d" -o"Primitives/Net/Net.o" "../Primitives/Net/Net.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Obstacles/Circle.d" -MT"Obstacles/Circle.d" -o"Obstacles/Circle.o" "../Obstacles/Circle.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Obstacles/Line.d" -MT"Obstacles/Line.d" -o"Obstacles/Line.o" "../Obstacles/Line.cpp"
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Obstacles/Obstacle.d" -MT"Obstacles/Obstacle.d" -o"Obstacles/Obstacle.o" "../Obstacles/Obstacle.cpp"

g++  -o"robotpilot"  ./Config.o ./Control.o ./Server.o ./geom.o ./robotpilot.o ./tools.o  ./lua/lapi.o ./lua/lauxlib.o ./lua/lbaselib.o ./lua/lcode.o ./lua/ldblib.o ./lua/ldebug.o ./lua/ldo.o ./lua/ldump.o ./lua/lfunc.o ./lua/lgc.o ./lua/linit.o ./lua/liolib.o ./lua/llex.o ./lua/lmathlib.o ./lua/lmem.o ./lua/loadlib.o ./lua/lobject.o ./lua/lopcodes.o ./lua/loslib.o ./lua/lparser.o ./lua/lstate.o ./lua/lstring.o ./lua/lstrlib.o ./lua/ltable.o ./lua/ltablib.o ./lua/ltm.o ./lua/lundump.o ./lua/lvm.o ./lua/lzio.o ./lua/print.o  ./Primitives/nodes/node.o ./Primitives/nodes/node_BDC.o ./Primitives/nodes/node_Broadcast.o ./Primitives/nodes/node_Console.o ./Primitives/nodes/node_Deadreck.o ./Primitives/nodes/node_Gateway.o ./Primitives/nodes/node_Input.o ./Primitives/nodes/node_Magnet.o ./Primitives/nodes/node_Power.o ./Primitives/nodes/node_Servo.o ./Primitives/nodes/node_Sonar.o ./Primitives/nodes/time.o  ./Primitives/UDP/UDPconn.o ./Primitives/UDP/UDPdriver.o ./Primitives/UDP/conv.o  ./Primitives/Primitives.o ./Primitives/PrimitivesCan.o ./Primitives/PrimitivesNet.o  ./Primitives/Net/Net.o  ./Obstacles/Circle.o ./Obstacles/Line.o ./Obstacles/Obstacle.o   -lpthread -lrt
