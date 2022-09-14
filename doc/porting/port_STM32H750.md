# Port TinyMaix to STM32H750

## Chip
|Item |Parameter|
|--   |--|
|Chip |STM32H750|
|Arch |ARM Cortex M7|
|Freq |480M |
|Flash|128KB|
|RAM  |1024KB|
|Acceleration| ARM SIMD|

## Board
WeAct Studio STM32H750   
<a href="assets/STM32H750.png"><img width=300 src="assets/STM32H750.png"/></a>

## Development Environment
MDK 5.37

## Step/Project
Just simple edit tm_port.h...   

Porting Project to be upload...



## Result
|config  |mnist|cifar|vww96  |mbnet128|Note|
|---     |---  |---  |---    |---     |---|
|O0 CPU  |1    | 26  | 84    |151     ||
|O1 CPU  |1    | 24  | 79    |124     ||
|O0 SIMD |1    | 21  | 65    |115     ||
|O1 SIMD |1    | 15  | 64    | 94     ||


## Note
STM32H750 have 1024KB Flash inside.


## Author
zepan@sipeed.com



