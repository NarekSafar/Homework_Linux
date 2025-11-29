CXX = g++
CXXFLAGS = -std=c++17 -fPIC -pthread
LDFLAGS = -pthread

all: libparallel_scheduler.so demo-application

parallel_scheduler.o: parallel_scheduler.cpp parallel_scheduler.h
	$(CXX) $(CXXFLAGS) -c parallel_scheduler.cpp

libparallel_scheduler.so: parallel_scheduler.o
	$(CXX) -shared -o libparallel_scheduler.so parallel_scheduler.o $(LDFLAGS)

demo-application: demo.cpp libparallel_scheduler.so
	$(CXX) demo.cpp -L. -lparallel_scheduler -Wl,-rpath=. -o demo-application $(LDFLAGS)

clean:
	rm -f *.o *.so demo-application
