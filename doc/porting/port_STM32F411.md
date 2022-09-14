# Port TinyMaix to STM32F411

## Chip
|Item |Parameter|
|--   |--|
|Chip |STM32F411|
|Arch |ARM Cortex M4|
|Freq |150M |
|Flash|512KB|
|RAM  |128KB|
|Acceleration| ARM SIMD|

## Board
WeAct Studio STM32F411CEU6 V3.0  
<a href="assets/STM32F411CEU6.png"><img width=300 src="assets/STM32F411CEU6.png"/></a>

## Development Environment
MDK 5.37

## Step/Project
Just simple edit tm_port.h...   

Porting Project in:   
https://github.com/Zepan/TinyMaix_STM32F411



## Result
|config  |mnist|cifar|vww96|mbnet128|Note|
|---     |---  |---  |---    |---     |---|
|O0 CPU  |2    |136  |490    |902     ||
|O1 CPU  |2    |120  |495    |798     ||
|O0 SIMD |2    |107  |368    |681     ||
|O1 SIMD |2    |75   |366    |558     ||


## Note
STM32F411 Flash is too small to place 96x96 image, use random data instead.  
O1 slow than O0 is beacuse Flash random read speed limit.   


## Author
zepan@sipeed.com



