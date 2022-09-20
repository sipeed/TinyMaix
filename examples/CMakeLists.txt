cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(tinymaix_examples)

set(CMAKE_AR "ar")
set(CMAKE_RANLIB "ranlib")
set(CMAKE_STRIP "strip")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 ")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../src lib_tinymaix)

include_directories(. ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set(example_mnist ${CMAKE_CURRENT_SOURCE_DIR}/mnist/tm_example_mnist.c)
set(example_cifar10 ${CMAKE_CURRENT_SOURCE_DIR}/cifar10/tm_example_cifar10.c)
set(example_vww ${CMAKE_CURRENT_SOURCE_DIR}/vww/tm_example_vww.c)
set(example_mbnet ${CMAKE_CURRENT_SOURCE_DIR}/mbnet/tm_example_mbnet.c)

add_executable(${PROJECT_NAME} tm_examples.c ${lib_tinymaix} ${example_mnist} ${example_cifar10} ${example_vww} ${example_mbnet})

option(MNIST "enable mnist example" OFF)
option(CIFAR10 "enable cifar10 example" OFF)
option(VWW "enable vww example" OFF)
option(MBNET "enable mbnet example" OFF)

if(MNIST)
    add_compile_definitions(TM_USE_EXAMPLE_MNIST=1)
endif(MNIST)

if(CIFAR10)
    add_compile_definitions(TM_USE_EXAMPLE_CIFAR10=1)
endif(CIFAR10)

if(VWW)
    add_compile_definitions(TM_USE_EXAMPLE_VWW=1)
endif(VWW)

if(MBNET)
    add_compile_definitions(TM_USE_EXAMPLE_MBNET=1)
endif(MBNET)

option(OPT_0 "enable opt0" OFF)
option(OPT_1 "enable opt1" OFF)

if(OPT_0)
    add_compile_definitions(TM_OPT_LEVEL=0)
endif(OPT_0)

if(OPT_1)
    add_compile_definitions(TM_OPT_LEVEL=1)
endif(OPT_1)

# add_definitions(-Werror)
target_link_libraries(${PROJECT_NAME} -lm)
