TARGET=robotpilot
CFLAGS=-O3 -g0 -Wall -c -fmessage-length=0
LDFLAGS=

SOURCES=Config.cpp Control.cpp Server.cpp geom.cpp nokia_server.cpp robotpilot.cpp tools.cpp
PRIMITIVES_SOURCES=Primitives.cpp PrimitivesCan.cpp PrimitivesNet.cpp
NODES_SOURCES=node.cpp node_BDC.cpp node_Broadcast.cpp node_Console.cpp node_Deadreck.cpp node_Gateway.cpp node_Input.cpp node_Power.cpp node_Servo.cpp node_Sonar.cpp node_Vacuum.cpp time.cpp
NET_SOURCES=Net.cpp
UDP_SOURCES=UDPconn.cpp UDPdriver.cpp conv.cpp
OBSTACLES_SOURCES=Circle.cpp Line.cpp Obstacle.cpp
LUA_SOURCES=lapi.c lauxlib.c lbaselib.c lcode.c ldblib.c ldebug.c ldo.c ldump.c lfunc.c lgc.c linit.c liolib.c llex.c lmathlib.c lmem.c loadlib.c lobject.c lopcodes.c loslib.c lparser.c lstate.c lstring.c lstrlib.c ltable.c ltablib.c ltm.c lundump.c lvm.c lzio.c print.c
LIBS=-lpthread -lrt

OBJ=Debug
OBJDIRS=$(OBJ) $(OBJ)/lua $(OBJ)/Obstacles $(OBJ)/Primitives $(OBJ)/Primitives/nodes $(OBJ)/Primitives/Net $(OBJ)/Primitives/UDP

CPP_SOURCES=$(SOURCES) $(addprefix Primitives/,$(PRIMITIVES_SOURCES)) $(addprefix Primitives/nodes/,$(NODES_SOURCES)) $(addprefix Primitives/Net/,$(NET_SOURCES)) $(addprefix Primitives/UDP/,$(UDP_SOURCES)) $(addprefix Obstacles/,$(OBSTACLES_SOURCES))
CPP_OBJS=$(addprefix $(OBJ)/,$(CPP_SOURCES:.cpp=.o))

C_SOURCES=$(addprefix lua/,$(LUA_SOURCES))
C_OBJS=$(addprefix $(OBJ)/,$(C_SOURCES:.c=.o))

all: $(TARGET)

$(TARGET): $(OBJDIRS) $(CPP_OBJS) $(C_OBJS)
	g++ -o $@ $(LDFLAGS) $(C_OBJS) $(CPP_OBJS) $(LIBS)

$(CPP_OBJS): $(OBJ)/%.o: %.cpp
	g++ $(CFLAGS) -o $@ $<

$(C_OBJS): $(OBJ)/%.o: %.c
	gcc $(CFLAGS) -o $@ $<

$(OBJDIRS):
	mkdir -p $@

clean:
	rm -rf *~ $(TARGET) $(CPP_OBJS) $(C_OBJS)
