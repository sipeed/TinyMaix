cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(vww)

set(CMAKE_AR "ar")
set(CMAKE_RANLIB "ranlib")
set(CMAKE_STRIP "strip")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 ")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../../src lib_tinymaix)

include_directories(. ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
#set(LIB_INFER ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/libtinymaix.a)

add_executable(${PROJECT_NAME} main.c ${lib_tinymaix})
target_link_libraries(${PROJECT_NAME} -lm)
