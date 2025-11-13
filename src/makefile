CXX = g++
CXXFLAGS = -Wall -Wextra -g -O2 -std=c++17

TARGET = miao
SRC = miao.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJ)

rebuild: clean all

.PHONY: all clean rebuild