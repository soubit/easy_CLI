CXX = g++
CXXFLAGS = -std=c++11 -Wall 

SRCS = EasyFileOpener.cpp helper.cpp Fileop.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = easyopen

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
