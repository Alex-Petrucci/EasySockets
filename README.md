# EasySockets
<small>Version 2.0.0</small>

Simple to use, cross-platform socket library for C++.

## Features
* Supports Windows, macOS and Linux.
* TCP and UDP sockets.
* IPv4 to IPv6 conversions.
* CMake based.

## Adding to your project
### Cloning
```shell
git clone "https://github.com/Alex-Petrucci/EasySockets.git"
```
### CMake
```cmake
add_subdirectory(EasySockets) # change if EasySockets dir is not in root
target_link_libraries(${PROJECT_NAME} EasySockets)
```

## Example
Below is an example of a server which waits until it receives one connection,
then it waits for data from that connection, and outputs that data.
```c++
#include <EasySockets/EasySockets.hpp>
#include <iostream>

int main()
{
    es::Context context{}; // context object required to use sockets
    
    // create a socket which accepts IPv6 addresses and 
    // converted IPv4 addressess, using TCP
    es::Socket server{es::IpVersion::dual_stack, es::Protocol::tcp};
    server.bind_to({"::", 8080}); // Bind the socket to all interfaces on port 8080
    
    server.listen_for_connections(1); // 1 is the backlog
    
    es::Socket accept{server.accept_connection()}; // Get a new socket from an incomming connection
    
    // receive the data from the connected client into a buffer
    char buffer[1024];
    int64_t bytes{accept.receive_data(buffer, sizeof(buffer))};
    
    // output the received data
    std::cout << "Received:\n" << std::string{buffer, static_cast<size_t>(bytes)} << '\n';
    
    return 0;
}
```

## Documentation
All documentation is on the [EasySockets website](https://alex-petrucci.github.io/EasySockets/)

If something is confusing, or for some reason is not part of the documentation,
feel free to post something in the discussions tab.

## Changelog
You can view the changelog [here](CHANGELOG.md).

## Contributing
See [CONTRIBUTING.md](CONTRIBUTING.md)

If you would like to see a new feature, make a
change to the library or notify us of bugs, feel 
free to make an issue.