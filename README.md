# TinyMaix

[中文](README_ZH.md) | English

TinyMaix is a tiny inference Neural Network library specifically for microcontrollers (TinyML).   
We design it follow the rule:  **Easy-to-Use** > **Portable** > **Speed** > **Space**  

Introduction to tinyML: [**TinyML**](tinyml_intro.md)  
See tested **48** chips and benchmark:  [**benchmark**](benchmark.md)  
Good News:  [**Rewarded Porting TinyMaix**](reward.md)

**Highlights**
- Core Code less than **400 lines**(tm_layers.c+tm_model.c+arch_cpu.h), code .text section less than **3KB**   
- Low ram consume, even **Arduino ATmega328** (32KB Flash, 2KB Ram) can run mnist with TinyMaix~  
- Support **INT8/FP32/FP16** model, experimentally support **FP8**, convert from keras h5 or tflite.
- Supoort multi architecture accelerate:  **ARM SIMD/NEON/MVEI，RV32P, RV64V, CSKYV2, X86 SSE2** ~
- User-friendly interfaces, just load/run models~
- Support Full Static Memory config
- [MaixHub](https://maixhub.com) **Online Model Training** support

**Run mnist demo on Arduino ATmega328**
```
mnist demo
0000000000000000000000000000
0000000000000000000000000000
0000000000000000000000000000
000000000077AFF9500000000000
000000000AFFFFFFD10000000000
00000000AFFFD8BFF70000000000
00000003FFD2000CF80000000000
00000004FD10007FF40000000000
00000000110000DFF40000000000
00000000000007FFC00000000000
0000000000004FFE300000000000
0000000000008FF9000000000000
00000000000BFF90000000000000
00000000001EFE20000000000000
0000000000CFF800000000000000
0000000004FFB000000000000000
000000001CFF8000000000000000
000000008FFA0000000000000000
00000000FFF10000000000000000
00000000FFF21111000112999900
00000000FFFFFFFFA8AFFFFFFF70
00000000AFFFFFFFFFFFFFFA7730
0000000007777AFFF97720000000
0000000000000000000000000000
0000000000000000000000000000
0000000000000000000000000000
0000000000000000000000000000
0000000000000000000000000000
===use 49912us
0: 0
1: 0
2: 89
3: 0
4: 1
5: 6
6: 1
7: 0
8: 0
9: 0
### Predict output is: Number 2, prob=89
```

## TODO List
1. ~~optimize tm_layers.c to tm_layers_O1.c, aimed to speed up to 1.4~2.0X~~  Done
2. ~~Add "ADD" OPS to support resnet/mbnet v2 ~~ Done!
3. Train good backbone for 64KB/128KB/256KB/512KB ram litmit
4. Add example: Detector,KWS,HAR,Gesture,OCR,...
5. ...

Do you want take participate in development of TinyMaix, or discuss with TinyML hobbyist?  
**Join our telegram group:** https://t.me/tinymaix


## TinyMaix Design
TinyMaix is design for running AI Neural Network Mdoels on resources limited MCUs, which usually called **TinyML**  

There are many TinyML infer library now, like TFLite micro, microTVM, NNoM, so why we need TinyMaix?   

TinyMaix is a weekend hackathons project, so it is simple enough to read though in 30 minutes, and it will help TinyML newbies to understand how is it running.   

TinyMaix aims to be a simple TinyML infererence library, it abandon many new features and doesn't use libs like CMSIS-NN.  

Following this design goal, now TinyMaix is as simple as 5 files to compile~   

We hope TinyMaix can help any MCU run AI Neural Network Mdoels, every one can port it to theirself hardware platform~   

> Note: Although TinyMaix support multi architecture accelerate, but it still need more effort to balance size and speed.  

### Features in design
- [x] Support up to mobilenet v1, RepVGG backbone
  - they are most common used, efficient structure for MCUs
  - [x] Basic Conv2d, dwConv2d, FC, Relu/Relu6/Softmax, GAP, Reshape
  - [x] ADD (for resnet,mbnet v2 struct, experimental)
  - [ ] MaxPool, AvgPool (now use stride instead)
- [x] FP32 model, INT8 quant model, FP16 model(**NEW**)
- [x] Convert tmdl from keras h5 or tflite
  - model is simple enough to train with keras/tf
  - tflite have quant functions already
- [x] Model statistics functions in C 
  - Optional for reduce code

### Features maybe added
- [ ] INT16 quant model
  - Advantages: 
    - more accuracy
    - friendly for SIMD/RV32P accelerate
  - Disadvantages: 
    - increase FLASH/RAM consume 2X
- [ ] Concat OP
  - Advantages: 
    - support mobilenet v2, more accuracy
  - Disadvantages: 
    - increase RAM consume 2X
    - concat mat cost many time cause model infer slow
    - need more work to cvt model into flat structure (in script)
- [ ] Winograd Convolution Optimization
  - Advantages:
    - may speed up Conv computing 
  - Disadvantages: 
    - increase RAM consume, and consume more memory bandwidth
    - increase code (.text) size
    - need many Transforms, weak MCU may cost many time here
    
### Features won't be added
- [ ] BF16 model
  - most MCU don't have BF16 computing ability
  - accuracy won't better than INT16 to much
  - increase FLASH/RAM consume 2X
- [ ] AVX/vulkan acceleration
  - TinyMaix is for MCUs, not for powerful PC/mobilephones
- [ ] other misc OPs
  - TinyMaix support MCUs to run basic model in minimum resource consumption, if you want more OPs, switch to TFlite-micro/TVM/NCNN... 

## Try Demos
### mnist
MNIST is handwritten digit recognition task, it is simple enough for even 8bit MCU like ATmega328.  
Try it on PC:  
```
cd examples/mnist
mkdir build
cd build 
cmake ..
make
./mnist
```

### mbnet
mbnet (mobilenet v1) is simple classification model for mobile devices, but it is still a little heavy for MCUs.  
The model in demo is mobilenet v1 0.25, it input 128x128x3 RGB image, output 1000 classes predict.  
It need at least 128KB SRAM and 512KB Flash, STM32F411 is the typical minimum config for this model.  

Try run mobilenet  
```
cd examples/mbnet
mkdir build
cd build 
cmake ..
make
./mbnet
```

## How to use (API)
### Load Model
tm_err_t tm_load  (tm_mdl_t* mdl, const uint8_t* bin, uint8_t*buf, tm_cb_t cb, tm_mat_t* in);   

mdl: model handle;   
bin: model bin buf;   
buf: main buf for middle output; if NULL, auto malloc main buf; else, use your static buffer.  
cb: layer callback;   
in: return input mat, include buf addr; //you can ignore it if use static buf  

### Remove Model
void     tm_unload(tm_mdl_t* mdl);                         

### Preprocess Input Data
tm_err_t tm_preprocess(tm_mdl_t* mdl, tm_pp_t pp_type, tm_mat_t* in, tm_mat_t* out);              
TMPP_FP2INT    //user own fp buf -> int input buf  
TMPP_UINT2INT  //int8: cvt in place; int16: can't cvt in place  
TMPP_UINT2FP01 // u8/255.0  
TMPP_UINT2FPN11// (u8-128)/128   

### Run Model
tm_err_t tm_run   (tm_mdl_t* mdl, tm_mat_t* in, tm_mat_t* out);         


## How to port
The core file is those 5 files: tm_model.c, tm_layers.c, tinymaix.h, tm_port.h, arch_xxx.h  

If you are using normal mcu without any acceleration instructions, choose arch_cpu.h, otherwise choose corresponding architecture header.  

And you should edit tm_port.h to fill your desired configs, all config macro have annotation follow it.   

Note TM_MAX_CSIZE,TM_MAX_KSIZE,TM_MAX_KCSIZE will occupy static buffers.  

And now just put them into your project, compile it~   

## How to train/convert models
There are training scripts in examples/mnist to learn how to train simple mnist models. 
> Note: you need install TensorFlow (>=2.7) first.

After training and save h5 models, you can use scripts in tools to convert to tmdl or c header files.   

1. h5_to_tflite.py   
  convert h5 model to float or int8 quant tflite files   
  python3 h5_to_tflite.py h5/mnist.h5 tflite/mnist_f.tflite 0   
  python3 h5_to_tflite.py h5/mnist.h5 tflite/mnist_q.tflite 1 quant_img_mnist/ 0to1   
2. tflite2tmdl.py  
  convert tflite file to tmdl or c header files.    
  python3 tflite2tmdl.py tflite/mnist_q.tflite tmdl/mnist_q.tmdl int8 1 28,28,1 10  
```
================ pack model head ================
mdl_type   =0
out_deq    =1
input_cnt  =1
output_cnt =1
layer_cnt  =6
buf_size   =1464
sub_size   =0
in_dims    = [3, 28, 28, 1]
out_dims   = [1, 1, 1, 10]
================   pack layers   ================
CONV_2D
    [3, 28, 28, 1] [3, 13, 13, 4]
    in_oft:0, size:784;  out_oft:784, size:680
    padding valid
    layer_size=152
CONV_2D
    [3, 13, 13, 4] [3, 6, 6, 8]
    in_oft:784, size:680;  out_oft:0, size:288
    padding valid
    layer_size=432
CONV_2D
    [3, 6, 6, 8] [3, 2, 2, 16]
    in_oft:0, size:288;  out_oft:1400, size:64
    padding valid
    layer_size=1360
MEAN
    [3, 2, 2, 16] [1, 1, 1, 16]
    in_oft:1400, size:64;  out_oft:0, size:16
    layer_size=48
FULLY_CONNECTED
    [1, 1, 1, 16] [1, 1, 1, 10]
    in_oft:0, size:16;  out_oft:1448, size:16
    layer_size=304
SOFTMAX
    [1, 1, 1, 10] [1, 1, 1, 10]
    OUTPUT!
    in_oft:1448, size:16;  out_oft:0, size:56
    layer_size=48
================    pack done!   ================
    model  size 2.4KB (2408 B) FLASH
    buffer size 1.4KB (1464 B) RAM
    single layer mode subbuff size 1.4KB (64+1360=1424 B) RAM
Saved to tmdl/mnist_q.tmdl, tmdl/mnist_q.h
```

Now you have tmdl or c header files, put it into your project to use it~  

## How to train models online with MaixHub

You can download models from [MaixHub](https://maixhub.com) or train your AI models online easily with MaixHub, don't need AI knowledge, train your model just click your mouse.

* Register [MaixHub](https://maixhub.com) account and login.
* You can download TinyMaix models from [model zoo](https://maixhub.com/model/zoo) or upload your models to model zoo for sharing.
* Create a train project, collect dataset and train models online, finally you will get files:
  * `.tmdl` file and `.h` file, use one of them in your code.
  * `report.json`, report info, json format, we can find labels or anchors in this file, we will use these params in our code. Attention, these params will change in every training, you should copy these params to your code when change model, or you will the result will be wrong.
> * There's two type: classification and detection, for first time usage, use **classification is recommended**.
> * There's many backbone, you should select proper backbone according to your MCU's RAM size, the smaller RAM size, should choose the smaller backbone.
> * For easier understanding how MaixHub works, at first time you can choose tfjs platform instead of tinymaix to run model on your mobile phone.
* Find demo in [examples](./examples) folder, use the `maixhub_image_classification` demo or `maixhub_image_detection` demo to run your model.


## How to add new platform acceleration code

For new platforms, you just need add arch_xxx.h to src dir, and implement functions inside.   
Here is the main functions you need implement (sort by importance):

```
a. TM_INLINE void tm_dot_prod(mtype_t* sptr, mtype_t* kptr,uint32_t size, sumtype_t* result)
	implement platform's dot product functions, usually use MAC related instructions. 

b. TM_INLINE void tm_dot_prod_pack2(mtype_t* sptr, mtype_t* kptr, uint32_t size, sumtype_t* result)
	implement platform's dual channel dot product functions  
  (not 4 or more channel, because some chip platform's register is not enough to support more channels)

c. TM_INLINE void tm_postprocess_sum(int n, sumtype_t* sums, btype_t* bs, int act, mtype_t* outp, sctype_t* scales, sctype_t out_s, zptype_t out_zp)
	implement platform's batch postprocess functions, note n is power of 2.

d. TM_INLINE void tm_dot_prod_3x3x1(mtype_t* sptr, mtype_t* kptr, sumtype_t* result)
	implement platform 3x3 dot product. mostly use handwrite cpu code.
  
e. TM_INLINE void tm_dot_prod_gap_3x3x1(mtype_t* sptr, mtype_t* kptr, uint32_t* k_oft, sumtype_t* result)
	implement platform 3x3 gap dot product. 
```

...

## Contribution & Contacts
If you want contribute functions to TinyMaix, please read "TinyMaix Design" sections, we only want functions in "Features in design" and "Features maybe added".  

If you want commit your port test result, please commit to benchmark.md.
You are welcome to port TinyMaix to your chip/boards, it will prove how easy to use TinyMaix run Deeplearning model in MCUs~

If you have question with TinyMaix usage/porting, please feedback Issues in this repo. 

If you have bussiness project consulting or private questions, you can send mail to support@sipeed.com or zepan@sipeed.com (Caesar Wu).  


