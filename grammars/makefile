CXX = g++-7
.PHONY = all clean

TARGET = hello

C_F = -g -std=c++17 -Wall -Werror

$(TARGET): main.cpp
	$(CXX) -o $@ main.cpp $(C_F)

clean:
	[ -e $(TARGET) ] && rm $(TARGET)

all: clean $(TARGET)
