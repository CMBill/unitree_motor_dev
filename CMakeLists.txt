cmake_minimum_required(VERSION 3.1.0)
project(unitree_motor_dev)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

include_directories(include)
link_directories(lib)

if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64") 
    set(EXTRA_LIBS libUnitreeMotorSDK_Arm64.so)
else()
    set(EXTRA_LIBS libUnitreeMotorSDK_Linux64.so)
endif()

#demos
add_executable(demo demos/demo.cpp)
target_link_libraries(demo ${EXTRA_LIBS})

set(LIBRARY_OUTPUT_PATH "../lib")

