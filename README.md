# TinyMaix

TinyMaix is a tiny inference Neural Network library specifically for microcontrollers (TinyML).

**Highlights**
- Core Code less than **400 lines**(tm_layers.c+tm_model.c+arch_O0.h), code .text section less than **3KB**   
- Low ram consume, even **Arduino ATmega328** (32KB Flash, 2KB Ram) can run mnist with TinyMaix~  
- Support **INT8/FP32** model, convert from keras h5 or tflite.
- Supoort multi architecture accelerate:  **ARM SIMD/NEON/MVEI，RV32P, RV64V** ~
- User-friendly interfaces, just load/run models~
- Support Full Static Memory config
- [MaixHub](https://maixhub.com) **Online Model Training** support soon~ 

## TinyMaix Design
TinyMaix is design for running AI Neural Network Mdoels on resources limited MCUs, which usually called **TinyML**  

There are many TinyML infer library now, like TFLite micro, microTVM, NNoM, so why we need TinyMaix?   

TinyMaix is a weekend hackathons project, so it is simple enough to read though in 30 minutes, and it will help TinyML newbies to understand how is it running.   

TinyMaix aims to be a simple TinyML infer library, it abandon many new features and don't use libs like CMSIS-NN.  

Follow this design goal, now TinyMaix is as simple as 5 files to compile~   

We hope TinyMaix can help any MCU run AI Neural Network Mdoels, every one can port it to theirself hardware platform~   

> Note: Although TinyMaix support multi architecture accelerate, but it still need more effort to balance size and speed.  

## Fisrt Try
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

If you are using normal mcu without any acceleration instructions, choose arch_O0.h, otherwise choose corresponding architecture header.  

And you should edit tm_port.h to fill your desired configs, all config macro have annotation follow it.   

Note TM_MAX_CSIZE,TM_MAX_KSIZE,TM_MAX_KCSIZE will occupy static buffers.  

And now just put them into your project, compile it~   

## How to train/convert models
There are training scripts in examples/mnist to learn how to train simple mnist models.  

After training and save h5 models, you can use scripts in tools to convert to tmdl or c header files.   

1. h5_to_tflite.py   
  convert h5 model to float or int8 quant tflite files   
  python3 h5_to_tflite.py h5/mnist.h5 tflite/mnist_f.tflite 0   
  python3 h5_to_tflite.py h5/mnist.h5 tflite/mnist_q.tflite 1 quant_img_mnist/ 0to1   
2. tflite2tmdl.py  
  convert tflite file to tmdl or c header files.    
  python3 tflite2tmdl.py tflite/mnist_q.tflite tmdl/mnist_q.tmdl int8 1 28,28,1 10  
Now you have tmdl or c header files, put it into your project to use it~  

## Available Operations
- [x] Core
  - [x] Conv2D  
  - [x] Fully-Connected
  - [x] dwConv2D
- [x] Activations
  - [x]  Relu/Relu6
  - [x]  Softmax
- [x]  Pooling
  - [x] GAP   
  - [ ] MaxPool
  - [ ] AvgPool
- [x] Shape
  - [x] Reshape

## TODO
concat    
Preprocess  
find good backbone for 64KB/128KB/256KB/512KB ram litmit
...

## Contacts
Caesar Wu: zepan@sipeed.com  


