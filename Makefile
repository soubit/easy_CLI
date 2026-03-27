CXX = g++
CC  = gcc

CXXFLAGS = -std=c++11 -g -Wall -DFILELOC=\"/home/souvik-sasmal/Desktop/EasyFileOpener/\"
CFLAGS   =  -g -Wall 

CPP_SRCS = EasyFileOpener.cpp helper.cpp Fileop.cpp
C_SRCS   = table_print.c my402list.c

CPP_OBJS = $(CPP_SRCS:.cpp=.o)
C_OBJS   = $(C_SRCS:.c=.o)

TARGET = ep

all: $(TARGET)

$(TARGET): $(CPP_OBJS) $(C_OBJS)
	$(CXX)  -o $(TARGET) $(CPP_OBJS) $(C_OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install :
	sudo cp ./$(TARGET) /usr/local/bin


clean:
	rm -f *.o $(TARGET)
	rm -f .temp.bin
