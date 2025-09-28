CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = read-file

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean run
	
