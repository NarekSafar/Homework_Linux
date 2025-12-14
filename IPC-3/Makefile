CXX      = g++
CXXFLAGS = -Wall -Wextra -O2
LDFLAGS  = -pthread

ARRAY_SRC = shared_array.cpp
ARRAY_HDR = shared_array.h

.PHONY: all clean ipc_clean

all: first second

first: first.cpp $(ARRAY_SRC) $(ARRAY_HDR)
	$(CXX) $(CXXFLAGS) first.cpp $(ARRAY_SRC) -o first $(LDFLAGS)

second: second.cpp $(ARRAY_SRC) $(ARRAY_HDR)
	$(CXX) $(CXXFLAGS) second.cpp $(ARRAY_SRC) -o second $(LDFLAGS)

ipc_clean: clean.cpp 
	$(CXX) $(CXXFLAGS) clean.cpp -o ipc_clean $(LDFLAGS)	
	./ipc_clean
	rm -f ipc_clean

clean:
	rm -f first second ipc_clean
