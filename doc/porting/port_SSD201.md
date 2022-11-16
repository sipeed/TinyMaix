# Port TinyMaix to SSD201

## Chip

| Item         | Parameter              |
| ------------ | ---------------------- |
| Chip         | SSD201                 |
| Arch         | ARM Cortex-A7, 2 Cores |
| Freq         | 1.2GHz                 |
| Flash        | 128M                   |
| RAM          | 64M                    |
| Acceleration | CPU                    |

## Board

Purple Pi, with Linux buildroot 4.9.84.

![](assets\Purple Pi.jpg)


## Development Environment

System: Ubuntu 22.04.1 LTS
Compiler: arm-linux-gnueabihf-gcc, gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.1)

## Step/Project
1. compiler setting, edit CMakeLists.txt:

   set(CMAKE_C_COMPILER "/usr/bin/arm-linux-gnueabihf-gcc")
   set(CMAKE_CXX_COMPILER "/usr/bin/arm-linux-gnueabihf-g++")

2. compiling setting, edit ./include/tm_port.h:

   #define TM_LOCAL_MATH   (1) 

Porting Project in:   
https://github.com/shannon2000/TinyMaix-SSD201.git



## Result

TM_MDL_INT8 (ms)

| config | mnist | cifar  | vww96  | mbnet128 |
| ------ | ----- | ------ | ------ | -------- |
| O0 CPU | 0.245 | 12.370 | 42.865 | 76.985   |
| O1 CPU | 0.233 | 10.229 | 41.746 | 65.692   |



## Author

xxlong1999@163.com

