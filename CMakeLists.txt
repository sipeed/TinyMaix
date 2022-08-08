set(TARGET_NAME tinymaix)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_AR "ar")
set(CMAKE_RANLIB "ranlib")
set(CMAKE_STRIP "strip")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 " CACHE STRING "c flags")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 " CACHE STRING "c++ flags")
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src lib_tinymaix) 
include_directories( . ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${TARGET_NAME} STATIC ${lib_tinymaix})

