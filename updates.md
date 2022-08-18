# UPDATES LOG

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