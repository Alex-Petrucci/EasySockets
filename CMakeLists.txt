cmake_minimum_required(VERSION 3.16)
project(EasySockets)

set(CMAKE_CXX_STANDARD 20)
set(ESInclude "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE ESHeaderFiles CONFIGURE_DEPENDS
    "${ESInclude}/*.hpp"
)

if(${WIN32})
    file(GLOB_RECURSE ESSourceFiles CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/windows/*.cpp"
    )
endif()

if(${UNIX})
    file(GLOB_RECURSE ESSourceFiles CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/posix/*.cpp"
    )
endif()

add_library(EasySockets STATIC ${ESHeaderFiles} ${ESSourceFiles})
target_include_directories(EasySockets PUBLIC ${ESInclude})

if(${WIN32})
    target_link_libraries(EasySockets PUBLIC wsock32 ws2_32)
endif()