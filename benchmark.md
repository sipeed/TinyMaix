# Benchmark
> Sort by cpu Freq; model time unit is ms;  
> mnist: 28x28x1 input,4->8->16, pad valid  
> __ mnist_q_valid.tmdl   2.4KB Flash 1.4KB RAM  
> mbnet128: mobile net v1 0.25 128x128x3 input, int8 quant  
> __ mbnet128_0.25_q.tmdl 485KB Flash 96KB RAM  (suit for MCU have >=512KB Flash, >=128KB RAM)  
> __ https://github.com/fchollet/deep-learning-models/releases  
 

> Note1: arduino run another smaller mnist model due to limited memory  
> Note2: item with two result, the first is O0, the second is enable ARCH Optimization

|Chip/Board  |Core      |Flash|RAM   |Freq |mnist|mbnet128|Note|
|---         |---       |---  |---   |---  |---      |---      |---|
|Atmega328   |AVR       |32KB |   2KB|  16M|50(*)    |---      ||
|STM32G030F6 |ARM CM0+  |32KB |   8KB|  64M|20      |---      ||
|STM32F103C8 |ARM CM3   |256KB|  20KB|  72M|10      |---      ||
|STM32F411CE |ARM CM4   |512KB| 128KB| 100M|5      |1793/1153  ||
|BL702       |RV32      |512KB| 132KB| 144M|3.5      |---      ||
|ESP32-S2    |Xtensa LX7|  4MB| 320KB| 240M|2    |---    ||
|BL618       |RV32P     |  4MB|0.5+4MB|320M|---      |---      ||
|BL808's E907|RV32P | 16MB|0.8+64MB|320M|<2ms     |525/371      |mdl in psram,O2|
|BL808's C906|RV64V | 16MB|0.8+64MB|400M(max480M)|<2ms   |315/212   |mdl in psram,O2|
|K210 MAIX-I |2xRV64GC  | 16MB|   8MB|400M|---     |---      ||
|F1C100s  LicheePi Nano|ARM9 | 16MB|32MB|400M  |---|---      ||
|STM32H750VB |ARM CM7   |128KB|1024KB| 480M|<2ms    |1056/954    |strange result, need confirm|
|V3s LicheePi Zero|ARM A7| ---|  64MB| 1.0G|---     |---      ||
|R329 M2A    |2xARM A53 | ---|  256MB| 1.5G|---     |---      ||
|Raspberry 4 |4xARM A72 | ---|  2~8GB| 1.8G|0.35    |128/112      ||
|N5105       |X86 Jaser Lake| ---|8GB|2~2.9G|0.15   |52      ||


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
|TM_ARCH_OPT0 && INT8| 1793ms|
|TM_ARCH_ARM_SIMD && INT8| 1153|

## TM_ARCH_ARM_MVEI
Optimization for ARM MCU which have MVEI instructions (Cortex-M55,etc.), suoport INT8 acceleration. 

> Experimental, not test data.


## TM_ARCH_ARM_NEON
Optimization for ARM MPU which have NEON instructions (Cortex-A7 and newer), suoport INT8/FP32 acceleration   

Raspberry Pi4 single core run mbnet 0.25, 128x128x3 input   
Strange result, need confirm...  


## TM_ARCH_RV32P
Optimization for RISC-V MCU which have P-extend instructions (like T-Head E907), suoport INT8 acceleration  
BL808 E907 core run mbnet 0.25, 128x128x3 input (mdl in psram, cpu run in 320M, O2)    

|Options|infer time|
|---|---|
|TM_ARCH_OPT0  && INT8| 525ms|
|TM_ARCH_RV32P && INT8| 371ms|

## TM_ARCH_RV64V
Optimization for RISC-V MCU which have V-extend instructions (like T-Head C906), suoport INT8/FP32 acceleration  
BL808 C906 core run mbnet 0.25, 128x128x3 input (mdl in psram, VLEN=128, cpu run in 400M, O2)    
|Options|infer time|
|---|---|
|TM_ARCH_OPT0  && INT8| 315ms|
|TM_ARCH_RV64V && INT8| 212ms|
|TM_ARCH_OPT0  && FP32| 300ms|
|TM_ARCH_RV64V && FP32| 190ms|

# compare to other infer library
## NNoM
run same mnist model  
STM32L476@150M, run NNoM, 62ms  
STM32F411@100M, run TinyMaix, 43ms/25ms(normal/SIMD)

## TFLite micro

## NCNN

## TVM


