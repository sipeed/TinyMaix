set(TARGET_NAME kws)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_AR "ar")
set(CMAKE_RANLIB "ranlib")
set(CMAKE_STRIP "strip")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os ")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../../src lib_tinymaix) 


include_directories( . ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

#set(LIB_INFER ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/libtinymaix.a)

add_executable(${TARGET_NAME} main.c mel_test.c ${lib_tinymaix} )  
target_link_libraries(${TARGET_NAME} -lm )   
