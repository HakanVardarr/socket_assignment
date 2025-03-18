# Socket Assignment: Client-Server Communication

## Overview
This project consists of a simple client-server model implemented in C++ using sockets. The server listens for incoming connections, while the client connects to the server, sends a number, and receives a randomly generated number from the server. The sum of these numbers is then calculated and displayed.

## Files
- `server.cpp`: Implements a multi-threaded server that accepts client connections.
- `client.cpp`: Implements a client that connects to the server and exchanges data.
- `Makefile`: Provides an alternative way to compile the project easily.

## How It Works
1. The server starts and listens for incoming connections on port `8080`.
2. The client connects to the server and sends its name along with a number.
3. The server processes the request, generates a random number (1-100), and sends it back to the client.
4. The client receives the response and displays:
   - The client's name and number
   - The server's name and number
   - The sum of the numbers

## Compilation & Execution
### Compile using g++
```sh
 g++ -Wall -Wextra -std=c++20 server.cpp -o server
 g++ -Wall -Wextra -std=c++20 client.cpp -o client
```

### Compile using Makefile
```sh
 make
```

### Run the Server
```sh
 ./server
```

### Run the Client
```sh
 ./client
```

## Notes
- The server runs indefinitely and handles multiple clients using threads.
- The client must input a valid number (between 0 and 100).
- The server validates the received message format.
- **This project is designed for Linux/macOS and does not work on Windows due to the use of POSIX sockets.**

## Dependencies
- C++ Standard Library
- POSIX sockets (Linux/MacOS compatible)

