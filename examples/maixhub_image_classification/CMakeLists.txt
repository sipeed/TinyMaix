cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(classification)

set(CMAKE_AR "ar")
set(CMAKE_RANLIB "ranlib")
set(CMAKE_STRIP "strip")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os " CACHE STRING "c flags")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os " CACHE STRING "c++ flags")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../../src lib_tinymaix)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# include_directories( . ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

aux_source_directory(. srcs)
#set(LIB_INFER ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/libtinymaix.a)

add_executable(${PROJECT_NAME} ${srcs} ${lib_tinymaix})
target_link_libraries(${PROJECT_NAME} -lm)
