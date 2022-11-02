# UPDATES LOG

## 2022.11.02
Add "ADD" OPS, now support resnet/mbnet v2 experimental~

## 2022.08.30
take time to optimize O0&O1.  now new O0 almost as fast as old O1~   
fast RVV fp16 model in 97ms now!!!   

## 2022.08.29
too busy recently... Slightly optimize for O1.   
prepare add kws example.   

## 2022.08.22
Add auto test script.  
Add experimental FP8 1.4.3/1.5.2 model, run simple mnist model successed.  
Rename arch_O0 to arch_cpu, Move O1,O2 to TM_OPT_LEVEL setting, start write tm_layer_O1.c   

## 2022.08.17
Add Support for RV64V's FP16 model, use RVV1.0 inntrinsic  
Optimize reduce sum with RVV's vfredosum_vs_f32m1_f32m1   
speed up TM_ARCH_RV64V infer mbnet v1 0.25:
|model type|before|after|speedup|
|--|--|--|--|
|INT8| 212ms| 178ms|19%|
|FP32| 190ms| 182ms|4%|
|FP16| 178ms| 161ms|11%|

tested infer time duty: sbuf:dot:sum ~ 9:5:2   
need opt sbuf time   

## 2022.08.08
First commit to github, support INT8/FP32 model,
support arch:
```   
arch_O0, 
arch_ARM_SIMD, arch_ARM_NEON,arch_ARM_MVEI,
arch_RV32P, arch_RV64V  
```
support OPS:
```
Conv2D,DWConv2D,(with relu/relu6,stride)
FC,GAP,SOFTMAX
```
support demos:
```
layer_test
mnist
mnist port for Arduino ATmega328
mbnet(mobilenet v1)
```