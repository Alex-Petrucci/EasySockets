cmake_minimum_required(VERSION 3.16)
project(Tests)

set(CMAKE_CXX_STANDARD 20)

add_executable(TestUdp TestUdp.cpp)
target_link_libraries(TestUdp PRIVATE EasySockets)
add_test(NAME UdpTests COMMAND TestUdp)

add_executable(TestTcp TestTcp.cpp)
target_link_libraries(TestTcp PRIVATE EasySockets)
add_test(NAME TcpTests COMMAND TestTcp)