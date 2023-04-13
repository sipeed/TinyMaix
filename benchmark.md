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


|Chip/Board  |Core      |Flash|RAM     |Freq |mbnet|vww96|cifar|mnist|Note|
|---         |---       |---  |---     |---  |---  |---  |---  |---  |---|
|BL808's NPU |BLAI      | 16MB|0.8+64MB| 320M| 5   | 3   | <1  | <1  ||
|i5-4590T    |AMD64     |256GB|     8GB|2000M|7/24 |5/17 |0.9/4|0.04/<1|native/wasm|
|RK3399's A72|ARM A72   | 32GB|     4GB|1800M| 15  | 10  |  3  | 0.07||
|TH1520      |RV64V(C910)|16GB|     4GB|1848M| 17  | 10.4| 3.6 | 0.15|fp16|
|RK3399's A53|ARM A53   | 32GB|     4GB|1600M| 29  | 19  |  5  | 0.14||
|D1-H        |RV64V     |128GB|     2GB|1008M| 43  | 22  | 3.5 | 0.29||
|SSD201      |ARM A7    |128MB|    64MB|1200M| 66  | 42  | 10  |0.23 ||
|ZYNQ7010's A9|ARM A9   | 32GB|     1GB| 667M| 66  | 44  | 7.8 |0.34 ||
|BL808's C906|RV64V     | 16MB|0.8+64MB| 480M| 81  | 57  | 10  | <1  ||
|STM32H750   |ARM CM7   |  1MB|  1024KB| 480M| 94  | 64  | 15  | <1  ||
|V851S' E907 |RV32P     |128MB|     4MB| 600M| 108 |  99 | 32  | 0.5  ||
|HPM6750     |Andes D45</br>RV32IMAFDCP|  8MB|2MB+32MB| 816M| 174  | 112  | 33  | 0.45  ||
|BL808's E907|RV32P     | 16MB|0.8+64MB| 320M| 188 | 149 | 35  | <1  |mdl in psram|
|F1C200S     |ARM926EJ-S| 16MB|    64MB| 608M| 213 | 145 | 38.5| 0.75||
|MT7621A     |MIPS1004Kc|128MB|   128MB| 880M| 413 | 244 | 41  |  1  |need confirm|
|AT32F403A   |ARM CM4   |  1MB|    96KB| 240M| 477 | 136 | 30  | 0.6 |mbnet in 224k ram mode|
|STM32G474RE |ARM CM4   |512KB|   128KB| 170M| XXX | 195 | 43  |  1  ||
|CH32V307    |RV32F</br>QingKe V4F|480KB|   128KB| 144M| XXX | 357 | 64  |  1  ||
|STM32F411CE |ARM CM4   |512KB|   128KB| 150M| 558 | 366 | 75  |  2  ||
| W801       |CSKYV2 ck804ef|2MB|160+128KB+8MB| 240M|606|364|52|<2| mdl in psram/(or sram if fit) |
|ESP32-S3    |Xtensa LX7|  8MB|   512KB| 240M| 610 | 381 |  86 |  5  |mdl in flash|
|LPC4337     |ARM CM4F  |  1MB|   136KB| 204M| 654 | 627 |  91 |  3  |need confirm|
|XR806       |ARMv8-M</br>Star-MC1|2MB|288KB|160M|712|453| 104 |  1  ||
|ESP32       |Xtensa LX6|  4MB|   520KB| 240M| 755 | 476 | 132 |  2  |mdl in flash|
|ACM32F403   |ARM CM33  |512KB|   192KB| 180M| XXX | 458 | 139 |  2  ||
|STM32F767   |ARM CM7   |  2MB|   512KB| 216M| 869 | 640 | 185 |  3  |need confirm|
|SC5864B's DSP|HIFI3    | 32MB|8MB+192KB|160M| 805 |---  |---  |15   |need confirm|
|STM32L496   |ARM CM4   |  1MB|   320KB|  80M| 809 | 695 | 162 |  3  ||
|BK7256      |RISC-V    |  4MB|   512KB| 320M|1036 | 678 | 186 |  2.8||
|NRF52832    |ARM CM4   |512KB|    64KB|  64M|XXX  |xxx  | 217 |  4  ||
|ESP32-S2    |Xtensa LX7|  4MB|   320KB| 240M|1079 | 659 | 241 |  2.5|need confirm|
|RP2040      |ARM CM0+  | 16MB|   264KB| 280M|1211 | 716 | 200 |  2  |overclock 280M|
|CH32V203G6  |RV32</br>QingKe V4B| 32KB|    10KB| 144M| XXX | XXX | XXX | 2.5  ||
|ESP32-C3    |RV32      |  4MB|   400KB| 160M|2370 |1430 | 127 |  6  |mdl in flash|
|MM32F3270   |ARM CM3   |512KB|   128KB|  96M|3126 |2919 | 257 | 11  ||
|Nuclei N300 |RV32IMAFDC| 16MB|   512KB|  16M|XXXX |2946 | 795 | 11  |need confirm|
|STM32F103RC |ARM CM3   |256KB|    48KB|  72M|XXX  |XXX  | 445 |  7  ||
|STM32F103C8 |ARM CM3   | 64KB|    20KB|  72M| XXX | XXX | XXX |  7  ||
|RiscyD2     |RV32IM    | 48KB|     8KB| 100M| XXX | XXX | XXX | 12  |Digilent Arty A7-35T|
|CH32V103    |RV32</br>QingKe V3A| 64KB|    20KB|  72M| XXX | XXX | XXX | 13  ||
|SAMD21G18   |ARM CM0+  |256KB|    32KB|  48M| XXX | XXX | 700 | 14  |seeed XIAO|
|APM32F072CB |ARM CM0+  | 128KB|   16KB|  48M| XXX | XXX | XXX | 17  ||
|STM32G030F6 |ARM CM0+  | 32KB|     8KB|  64M| XXX | XXX | XXX | 18  ||
|CM0(Kintex-7)|ARM CM0  |  ---|  1024KB|  50M| XXX | XXX | 1362| 23  |Kintex-7|
|CH582       |RV32</br>QingKe V4A|448KB|32KB|60M|XXX|XXX |XXX  | 31  ||
|STC32G12K128|80251     |128KB|    12KB|  35M| XXX | XXX | XXX | 37  ||
|PicoRV32(GW2A)|RV32    |  1MB|    64KB|  54M| XXX | XXX |26935| 385 |Tang Primer 20K|
|Atmega328   |AVR       | 32KB|     2KB|  16M| XXX | XXX | XXX |50(*)||


Normalization to 100M freq to compare CPU efficiency, using cifar model:  

|Chip/Board  |Core      |cifar(ms)|
|---         |---       |---  |
|BL808's NPU |BLAI      | 2   |
|D1-H        |RV64V     | 35  |
|BL808's C906|RV64V     | 48  |
|RK3399's A72|ARM A72   | 52  |
|ZYNQ7010's A9|ARM A9   | 52  |
|TH1520      |RV64V(C910)| 66 |
|STM32H750   |ARM CM7   | 72  |
|AT32F403A   |ARM CM4   | 72  |
|STM32G474RE |ARM CM4   | 73  |
|RK3399's A53|ARM A53   | 79  |
|CH32V307    |RV32 IMAC | 92  |
|BL808's E907|RV32P     | 112 |
|STM32F411CE |ARM CM4   | 113 |
|SSD201      |ARM A7    | 123 |
|W801        |CSKYV2 ck804ef| 125 |
|Nuclei N300 |RV32IMAFDC| 127 |
|STM32L496   |ARM CM4   | 130 |
|NRF52832    |ARM CM4   | 139 |
|XR806       |ARMv8-M</br>Star-MC1| 166 |
|ESP32-C3    |RV32      | 203 |
|ESP32-S3    |Xtensa LX7| 206 |
|F1C200S     |ARM926EJ-S| 234 |
|MM32F3270   |ARM CM3   | 247 |
|ACM32F403   |ARM CM33  | 250 |
|ESP32       |Xtensa LX6| 317 |
|STM32F103RC |ARM cM3   | 320 |
|SAMD21G18   |ARM CM0+  | 336 |
|MT7621A     |MIPS1004Kc| 360 |
|RP2040      |ARM CM0+  | 560 |
|ESP32-S2    |Xtensa LX7| 578 |
|CM0(Kintex-7)|ARM CM0  | 681 |
|PicoRV32(GW2A)|RV32    |14545|


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

## TM_ARCH_CSKYV2
Optimization for CskyV2 MCU which have DSP instructions (like ck804ef), suoport INT8 acceleration  
w801 ck804ef core run mbnet 0.25, 128x128x3 input (mdl in psram, cpu run at 240M,psram overclock to 120M, O3, with data cache on)

|ARCH|MDL_TYPE|OPT0 time|OPT1 time|
|---|---|---|---|
|TM_ARCH_CPU     | INT8| 950ms| 816ms|
|TM_ARCH_CSKYV2  | INT8| 724ms| 606ms|

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





