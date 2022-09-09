# Rewarded Porting TinyMaix
In order to show the portability of TinyMaix, Sipeed launched the Rewarded Porting TinyMaix~  

From now on until **December 31, 2022**, add support for any new chip that is not in the current tested list (see benchmark.md),   
You can get Sipeed's latest AIoT board: 480M C906 + 320M E907 + WiFi/BT + 64M PSRAM + 2MP Camera + CTP LCD ~  

Rules:  
1. The same series of chips are considered as one chip. For example, stm32f411 has been transplanted, so stm32f401 is considered as the same chip; Stm32h7 and stm32f7 are considered as different series chips as "F7" and "H7" is different
2. Porting of non-MCU chip is allowed, but only record one chip for one core. For example, arm cortex A7 only takes the first one A7 chip commit.
3. The benchmark of the runnable model within the chip capability need to be tested, from low to high is:
   1. mnist   2KB Flash,  2KB RAM    //Handwritten digit recognition (28x28x1)
   2. cifar  89KB Flash, 11KB RAM    //10 classes classification(32x32x3)
   3. vww   227KB Flash, 54KB RAM    //2 classes，mobilenet v1 0.25, 96x96x3 input
   4. mbnet 485KB Flash, 96KB RAM    //1000 classes，mobilenet v1 0.25,128x128x3 input
4. Write a porting log (refer to doc/porting/port_STM32F411.md), and submit PR to this repo.

# Advanced feature Awards
Those who provide TinyMaix with the following advanced function improvements will get a Quad-Core C910 board in addition!  
1. Add mobilenetv2 support for TinyMaix, that is, mainly add concat operator (involving model conversion script and tinymaix C code, requiring model structure compatibility)
2. Take mobilenet V1 0.25 96x96 as the test model and the corresponding architecture O1 as the benchmark to try to optimize the execution speed and increase the execution speed by at least 15%
3. Improve FP8 support for tinymaix, support FP8 mobilenetv1 model
4. Add a new architecture acceleration for tinymaix, such as xtensa's vector instruction set, and reduce infer time by at least 25% compared with the original CPU execution speed
5. Other advanced function updates approved by sipeed

# Recruit
For users in China we will provide priority interview opportunity if you submit result successfully. (send resume to support@sipeed.com)