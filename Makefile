CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20

all: server client

server: server.cpp
	$(CXX) $(CXXFLAGS) -o server server.cpp

client: client.cpp
	$(CXX) $(CXXFLAGS) -o client client.cpp

clean:
	rm -f client server

.PHONY: all clean
