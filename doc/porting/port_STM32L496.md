# Port TinyMaix to STM32L496


## Chip
|Item |Parameter|
|--   |--|
|Chip |STM32L496|
|Arch |ARM Cortex M4|
|Freq |80M |
|Flash|1024KB|
|RAM  |320KB|
|Acceleration| ARM SIMD|



## Board
alios development board STM32L496VGT6
<a href="image/board.png"><img width=300 src="image/board.png"/></a>

## Development Environment
MDK 5.37

## Step/Project
Just simple edit tm_port.h...   

Porting Project in:   
https://github.com/starry-m/TinyMaix_STM32L496



## Result
|config  |mnist|cifar|vww96|mbnet128|Note|
|---     |---  |---  |---    |---     |---|
|O0 CPU  |7    |194  |967    |11973   ||
|O1 CPU  |3    |149  |677    |792     ||





## Author
2386279494@qq.com# TinyMaix_STM32L496
# TinyMaix_STM32L496
