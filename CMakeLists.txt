cmake_minimum_required(VERSION 3.0)

project(WCD_driver)

option(SIGN_DRIVER ON)
option(CREATE_CERT OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4 /WX")

add_subdirectory(WCD)
add_subdirectory(WCDClient)
