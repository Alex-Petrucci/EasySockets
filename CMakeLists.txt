cmake_minimum_required(VERSION 3.16)
project(EasySockets)

set(CMAKE_CXX_STANDARD 20)
set(ESInclude "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(ESSourceFiles "")

file(GLOB_RECURSE ESHeaderFiles CONFIGURE_DEPENDS
    "${ESInclude}/*.hpp"
)

if(WIN32)
    file(GLOB_RECURSE ESWinSrc CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Windows/*.cpp"
    )
    list(APPEND ESSourceFiles ${ESWinSrc})
elseif(UNIX)
    file(GLOB_RECURSE ESPosixSrc CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Posix/*.cpp"
    )
    list(APPEND ESSourceFiles ${ESPosixSrc})
else()
    message(FATAL_ERROR "This platform is not currently supported by EasySockets.")
endif()

add_library(EasySockets STATIC ${ESHeaderFiles} ${ESSourceFiles})
target_include_directories(EasySockets PUBLIC ${ESInclude})
set_target_properties(EasySockets PROPERTIES LINKER_LANGUAGE CXX)

if(WIN32)
    target_link_libraries(EasySockets PUBLIC wsock32 ws2_32)
endif()

OPTION(ESBuildTests "Build EasySockets tests" OFF)
if(ESBuildTests)
    enable_testing()
    add_subdirectory(tests)
endif()