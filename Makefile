TARGET=robotpilot
CFLAGS=-O3 -g0 -Wall -c -fmessage-length=0
LDFLAGS=
LIBS=-lpthread -lrt

OBJ=Debug
CPP_SOURCES=$(patsubst ./%,%,$(shell find . -name "*.cpp"))
C_SOURCES=$(patsubst ./%,%,$(shell find . -name "*.c"))

OBJDIRS=$(OBJ) $(addprefix $(OBJ)/,$(filter-out ./,$(sort $(dir $(CPP_SOURCES) $(C_SOURCES)))))
CPP_OBJS=$(addprefix $(OBJ)/,$(CPP_SOURCES:.cpp=.o))
C_OBJS=$(addprefix $(OBJ)/,$(C_SOURCES:.c=.o))

all: $(TARGET)

$(TARGET): $(OBJDIRS) $(C_OBJS) $(CPP_OBJS)
	g++ -o $@ $(LDFLAGS) $(C_OBJS) $(CPP_OBJS) $(LIBS)

$(CPP_OBJS): $(OBJ)/%.o: %.cpp
	g++ $(CFLAGS) -o $@ $<

$(C_OBJS): $(OBJ)/%.o: %.c
	gcc $(CFLAGS) -o $@ $<

$(OBJDIRS):
	mkdir -p $@

clean:
	rm -rf *~ $(TARGET) $(CPP_OBJS) $(C_OBJS)
