.PHONY : clean

SRC_DIR1 = ./libs/libwnnDictionary/engine
SRC_DIR2 = ./libs/libwnnJpnDic
INC1= ./libs/libwnnDictionary/include
INC2= ./libs/libwnnDictionary
INC3= ./src/include

CFLAGS=  -fPIC -g -O2 -I$(INC1) -I$(INC2) -I$(INC3)
LDFLAGS= -shared

SOURCES=$(wildcard ./libs/libwnnDictionary/engine/*.c) \
        $(wildcard ./libs/libwnnJpnDic/*.c) \
        $(wildcard ./src/*.cpp)

OBJECTS=$(SOURCES:.c%=.o)

TARGET= libWnnJpn.so

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CXX) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS) -std=c++11

