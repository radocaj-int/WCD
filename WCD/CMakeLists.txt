cmake_minimum_required(VERSION 3.0)

project(WCD)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(WDK REQUIRED)

#target_include_directories(WCD INTERFACE .)

wdk_add_driver(WCD KMDF 1.15
        wcd.cpp
	)
