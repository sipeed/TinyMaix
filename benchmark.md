# Benchmark 
## Test Models
### mnist
mnist: 28x28x1 input,4->8->16, pad valid  
mnist_q_valid.h   2.4KB Flash 1.4KB RAM  
suit for MCU have >=16KB Flash, >=2KB RAM 
### cifar
cifar: 32x32x3 input, 32->32->64->1024->10, 5x5 conv  
cifar10_q.h 89KB Flash 11KB RAM   
suit for MCU have >=128KB Flash, >=20KB RAM  
### vww96
vww96: vww model based on mobile net v1 0.25 96x96x3 input  
vww96_q.tmdl 227KB Flash 54KB RAM  
suit for MCU have >=256KB Flash, >=64KB RAM  
https://mlcommons.org/en/inference-tiny-07/
### mbnet128
mbnet128: mobile net v1 0.25 128x128x3 input  
mbnet128_0.25_q.tmdl 485KB Flash 96KB RAM  
suit for MCU have >=512KB Flash, >=128KB RAM   
https://github.com/fchollet/deep-learning-models/releases  

## Test Record
model infer time unit is ms;   
Sort by performance, compare priority: mbnet128 > vww96 > cifar > mnist   
> Note1: arduino run another smaller mnist model due to limited memory  
> Note2: all model record fastest model type's infer time, for example, C906 use FP16 result.
> Note3: XXX means impossible run this model on that chip

|Chip/Board  |Core    |Flash|RAM     |Freq |mbnet|vww96|cifar|mnist|Note|
|---         |---     |---  |---     |---  |---  |---  |---  |---  |---|
|BL808's NPU |BLAI    | 16MB|0.8+64MB| 320M| 5   | 3   | <1  | <1  ||
|BL808's C906|RV64V   | 16MB|0.8+64MB| 480M| 81  | 57  | 10  | <1  ||
|STM32H750   |ARM CM7 |  1MB|  1024KB| 480M| 94  | 64  | 15  | <1  ||
|BL808's E907|RV32P   | 16MB|0.8+64MB| 320M| 188 | 149 | 35  | <1  |mdl in psram|
|STM32G474RE |ARM CM4 |512KB|   128KB| 170M| XXX | 195 | 43  |  1  ||
|STM32F411CE |ARM CM4 |512KB|   128KB| 150M| 558 | 366 | 75  |  2  ||
|CH32V307    |RISCV32 IMAC|480KB|128KB|144M| XXX | 357 | 64  |  1  ||
|STM32F103C8 |ARM CM3 | 64KB|    20KB|  72M| XXX | XXX | XXX |  8  ||
|STM32G030F6 |ARM CM0+| 32KB|     8KB|  64M| XXX | XXX | XXX | 18  ||
|Atmega328   |AVR     | 32KB|     2KB|  16M| XXX | XXX | XXX |50(*)||


Normalization to 100M freq to compare CPU efficiency, using cifar model:  

|Chip/Board  |Core    |cifar|
|---         |---     |---  |
|BL808's NPU |BLAI    | 2   |
|BL808's C906|RV64V   | 48  |
|STM32H750   |ARM CM7 | 72  |
|STM32G474RE |ARM CM4 | 73  |
|CH32V307    |RV32 IMAC|92  |
|BL808's E907|RV32P   | 112 |
|STM32F411CE |ARM CM4 | 113 |


# Infer Time & Input Size
mbnet infer time under different input size   
BL808 C906 core 480M, use RV64V, FP16 model  
|input size|infer time|
|---    |---   |
| 96x 96|  60ms|
|128x128|  81ms|
|160x160| 156ms|
|192x192| 183ms|
|224x224| 296ms|


# Optimization 
## TM_FASTSCALE
Optimization for MCU which don't have FPU  
STM32F103C8 run mnist
|Options|infer time|
|---|---|
|TM_FASTSCALE=0| 16ms|
|TM_FASTSCALE=1| 10ms|


## TM_ARCH_ARM_SIMD
Optimization for ARM MCU which have DSP (Cortex-M4,M7,etc.), suoport INT8 acceleration  
STM32F411CE run mbnet 0.25, 128x128x3 input  
|Options|infer time|
|---|---|
|TM_ARCH_OPT0 && INT8| 1199ms|
|TM_ARCH_ARM_SIMD && INT8| 840ms|

## TM_ARCH_ARM_MVEI
Optimization for ARM MCU which have MVEI instructions (Cortex-M55,etc.), suoport INT8 acceleration. 

> Experimental, not test data.


## TM_ARCH_ARM_NEON
Optimization for ARM MPU which have NEON instructions (Cortex-A7 and newer), suoport INT8/FP32 acceleration   

Raspberry Pi4 single core run mbnet 1.0, 224x224x3 input   
(NEON INT8 not well optimized)  

|ARCH|MDL_TYPE|OPT0 time|OPT1 time|
|---|---|---|---|
|TM_ARCH_CPU     | INT8| 860ms| 821ms|
|TM_ARCH_CPU     | FP32| 2307ms| 2271ms|
|TM_ARCH_ARM_NEON| FP32| 1275ms|  1223ms|
|TM_ARCH_ARM_NEON| INT8| 959ms|  923ms|

## TM_ARCH_RV32P
Optimization for RISC-V MCU which have P-extend instructions (like T-Head E907), suoport INT8 acceleration  
BL808 E907 core run mbnet 0.25, 128x128x3 input (mdl in psram, cpu run in 320M, O2)    

|ARCH|MDL_TYPE|OPT0 time|OPT1 time|
|---|---|---|---|
|TM_ARCH_CPU  | INT8| 443ms| 283ms|
|TM_ARCH_RV32P| INT8| 345ms| 188ms|

## TM_ARCH_RV64V
Optimization for RISC-V MCU which have V-extend instructions (like T-Head C906), suoport INT8/FP32 acceleration  
BL808 C906 core run mbnet 0.25, 128x128x3 input (mdl in psram, VLEN=128, cpu run in 480M, O2)    

|ARCH|MDL_TYPE|OPT0 time|OPT1 time|
|---|---|---|---|
|TM_ARCH_CPU  | INT8| 153ms| 125ms|
|TM_ARCH_CPU  | FP32| 215ms| 177ms|
|TM_ARCH_RV64V| INT8| 123ms|  95ms|
|TM_ARCH_RV64V| FP32| 160ms| 121ms|
|TM_ARCH_RV64V| FP16| 129ms|  81ms|

# Compare to other infer library
Use SmallCifar model. TinyMaix use stride=2's time multipy by 4.   
NNoM&TinyMaix run with STM32H750@218M, other run with STM32F746@216M   

|InferLib|time(ms)|
|---|---|
|TFlite-micro|393|
|MicroTVM untuned|294|
|TinyMaix CPU O0|224|
|TinyMaix CPU O1|204|
|TinyMaix SIMD O0|176|
|NNoM|159|
|MicroTVM tuned|157|
|CMSIS-NN|136|
|TinyMaix SIMD O1|132|
|tinyengine|129|





