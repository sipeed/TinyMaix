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

<a href="assets/STM32L496VGT6.png"><img width=300 src="assets/STM32L496VGT6.png"/></a>

## Development Environment
MDK 5.37

## Step/Project
Just simple edit tm_port.h...   

Porting Project in:   
https://github.com/starry-m/TinyMaix_STM32L496



## Result
|config  |mnist|cifar|vww96|mbnet128|Note|
|---     |---  |---  |---    |---     |---|
|O0 CPU  |4    |281  |900    |1022    |MDK AC6.-Oz image size|
|O1 CPU  |4    |254  |923    |1044    |MDK AC6.-Oz image size|
|O0 SIMD |4    |211  |708    |823     |MDK AC6.-Oz image size|
|O1 SIMD |3    |162  |695    |809     |MDK AC6.-Oz image size|





## Author
2386279494@qq.com# TinyMaix_STM32L496
# TinyMaix_STM32L496
