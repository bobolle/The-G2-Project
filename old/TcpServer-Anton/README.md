# TCP Server

## Overview
This is a lightweight TCP server written in C using POSIX sockets. The server listens for incoming TCP connections and echoes received messages. It supports a configurable port via command-line arguments.

## Features
- Accepts a custom port via `-p <port>` argument
- Uses non-blocking sockets for efficient handling
- Supports client timeout handling
- Runs on Linux and other UNIX-based systems

## Prerequisites
Ensure that you have the following installed:
- **GCC** (for compilation)
- **CMake** (for build configuration)
- **POSIX-compliant OS** (Linux, macOS, or BSD)

## Installation
Clone the repository and build the project using CMake:

```sh
git clone https://github.com/bobolle/The-G2-Project.git
cd TcpServer-Anton
mkdir build && cd build
cmake ..
make
```

## Usage
Run the server with a specified port:

```sh
./tcp_server -p 8080
```

If no port is specified, the server defaults to **port 1337**:

```sh
./tcp_server
```

## Configuration
Modify the **CMakeLists.txt** file to adjust server settings such as:
- Default listening port (`#define DEFAULT_PORT 1337` in `server.c`)
- Maximum number of connections
- Logging verbosity

## Code Structure
```
tcp-server/
│── server.c            # Main server implementation
│── CMakeLists.txt      # Build configuration
│── README.md           # Documentation
```

## Troubleshooting
- **"Address already in use" error:** Ensure no other process is using the same port.
- **Compilation issues:** Verify that `gcc` and `cmake` are installed and available in `PATH`.
- **Firewall issues:** Check your system’s firewall settings to allow inbound connections on the server port.

## License
This project is licensed under the MIT License. See `LICENSE` for details.

