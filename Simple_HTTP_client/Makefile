CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

TARGET = http_client
SRC = http_client.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
