cmake_minimum_required(VERSION 3.16)
project(EasySockets)

set(CMAKE_CXX_STANDARD 20)
set(ESInclude "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE ESSourceFiles CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${ESInclude}/*.h"
)

message(STATUS "EasySockets source files: ${ESSourceFiles}")

add_library(EasySockets STATIC ${ESSourceFiles})
target_include_directories(EasySockets PUBLIC ${ESInclude})

if(${WIN32})
    target_link_libraries(EasySockets PUBLIC wsock32 ws2_32)
endif()