# TinyMaix

中文 | [English](README.md)

TinyMaix是面向单片机的超轻量级的神经网络推理库，即TinyML推理库，可以让你在任意单片机上运行轻量级深度学习模型~   
我们的设计原则：**易用性** > **移植性** > **速度** > **空间**  

tinyML基础介绍: [**TinyML**](tinyml_intro.md)  
查看已移植的芯片测试结果:  [**benchmark**](benchmark.md)  
好消息:  [**TinyMaix有奖移植**](reward.md)

**关键特性**
- 核心代码少于**400行**(tm_layers.c+tm_model.c+arch_cpu.h), 代码段(.text)少于**3KB**   
- 低内存消耗，甚至**Arduino ATmega328** (32KB Flash, 2KB Ram) 都能基于TinyMaix跑mnist(手写数字识别)
- 支持**INT8/FP32/FP16**模型，实验性地支持**FP8**模型，支持keras h5或tflite模型转换 
- 支持多种芯片架构的专用指令优化:  **ARM SIMD/NEON/MVEI，RV32P, RV64V** 
- 友好的用户接口，只需要load/run模型~
- 支持全静态的内存配置(无需malloc)
- [MaixHub](https://maixhub.com) **在线模型训练**支持

**在Arduino ATmega328上运行mnist demo**
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

## TODO 
1. ~~将 tm_layers.c 优化到 tm_layers_O1.c, 目标提升速度到 1.4~2.0X~~  已完成
2. 针对64/128/256/512KB内存限制，找到合适的骨干网络
3. 增加例程： Detector,KWS,HAR,Gesture,OCR,...
4. ...

如果想参与进TinyMaix的开发，或者想与TinyML爱好者交流，   
请加入我们的telegram交流群：https://t.me/tinymaix  



## TinyMaix 设计思路
TinyMaix 是专为低资源的单片机设计的AI神经网络推理框架，通常被称为**TinyML**  

现在已经有很多TinyML推理库，比如TFLite micro, microTVM, NNoM, 那为什么我们又捏了TinyMaix这个轮子呢?   

TinyMaix是两个周末业余时间完成的项目，所以它足够简单，可以再30分钟内走读完代码，可以帮助TinyML新手理解它是怎么运行的。

TinyMaix希望成为一个足够简单的TinyML推理库，所以它放弃了很多特性，并且没有使用很多现成的NN加速库，比如CMSIS-NN

在这个设计思路下，TinyMaix只需要5个文件即可编译~

我们希望TinyMaix可以帮助任何单片机运行AI神经网络模型, 并且每个人都能移植TinyMaix到自己的硬件平台上~

> 注意：虽然TinyMaix支持多架构加速，但是它仍然需要更多工作来平衡速度和尺寸

### 设计内的特性
- [x] 最高支持到mobilenet v1, RepVGG的骨干网络
  - 因为它们对单片机来说是最常用的，最高效的结构
  - [x] 基础的 Conv2d, dwConv2d, FC, Relu/Relu6/Softmax, GAP, Reshape
  - [ ] MaxPool, AvgPool (现在使用stride代替)
- [x] FP32浮点模型, INT8量化模型, **FP16**半精度模型
- [x] 转换keras h5或tflite到tmdl
  - 简单模型使用keras/tf训练已经足够
  - 复用了tflite现成的量化功能
- [x] 模型统计功能
  - 可选以减少代码尺寸

### 可考虑添加的特性
- [ ] INT16量化模型
  - 优点: 
    - 更精确
    - 对于 SIMD/RV32P 指令加速更友好
  - 缺点: 
    - 占用了2倍的FLASH/RAM
- [ ] Concat算子
  - 优点: 
    - 支持mobilenet v2, 模型精度更高
  - 缺点: 
    - 占用了2倍的RAM
    - concat 张量占用了更多时间，使得模型运算变慢
    - 需要更多转换脚本工作转换分支模型到扁平结构
- [ ] Winograd卷积优化
  - 优点:
    - 可能加速卷积计算
  - 缺点: 
    - 增加了RAM空间和带宽消耗
    - 增大了代码段(.text)尺寸
    - 需要很多变换，弱单片机可能会消耗更多时间
    
### 不考虑添加的特性
- [ ] BF16模型
  - 多数单片机不支持BF16计算
  - 精度不会比INT16高太多
  - 占用了2倍的FLASH/RAM
- [ ] AVX/vulkan 加速
  - TinyMaix是为单片机设计的，而不是为强大的电脑/手机设计
- [ ] 其他多样化的算子
  - TinyMaix仅为单片机提供基础模型算子支持，如果你需要更特殊的算子，可以选择TFlite-micro/TVM/NCNN... 

## 例程体验
### mnist
MNIST是手写数字识别任务，足够简单以至于可以在ATmega328这样的8位单片机上运行。  
在电脑上测试： 
```
cd examples/mnist
mkdir build
cd build 
cmake ..
make
./mnist
```

### mbnet
mbnet (mobilenet v1) 是适用于移动手机设备的简单图像分类模型，不过对单片机来说也稍微沉重了些。  
例程里的模型是mobilenet v1 0.25，输入128x128x3的RGB图像，输出1000分类的预测
它需要至少128KB SRAM 和 512KB Flash, STM32F411是典型可以运行该模型的最低配置。

在PC上测试运行mobilenet 1000分类图片例程
```
cd examples/mbnet
mkdir build
cd build 
cmake ..
make
./mbnet
```

## 如何使用 (API)
### 加载模型
tm_err_t tm_load  (tm_mdl_t* mdl, const uint8_t* bin, uint8_t*buf, tm_cb_t cb, tm_mat_t* in);   

mdl: 模型句柄;   
bin: 模型bin内容;   
buf: 中间结果的主缓存；如果NULL，则内部自动malloc申请；否则使用提供的缓存地址
cb: 网络层回调函数;   
in: 返回输入张量，包含输入缓存地址 //可以忽略之，如果你使用自己的静态输入缓存

### 移除模型
void     tm_unload(tm_mdl_t* mdl);                         

### 输入数据预处理
tm_err_t tm_preprocess(tm_mdl_t* mdl, tm_pp_t pp_type, tm_mat_t* in, tm_mat_t* out);              
TMPP_FP2INT    //用户自己的浮点缓存转换到int8缓存
TMPP_UINT2INT  //典型uint8原地转换到int8数据；int16则需要额外缓存
TMPP_UINT2FP01 //uint8转换到0~1的浮点数 u8/255.0  
TMPP_UINT2FPN11//uint8转换到-1~1的浮点数

### 运行模型
tm_err_t tm_run   (tm_mdl_t* mdl, tm_mat_t* in, tm_mat_t* out);         


## 如何移植
TinyMaix的核心文件只有这5个：tm_model.c, tm_layers.c, tinymaix.h, tm_port.h, arch_xxx.h  

如果你使用没有任何指令加速的普通单片机，选择 arch_cpu.h, 否则选择对应架构的头文件 

然后你需要编辑tm_port.h，填写你需要的配置，所有配置宏后面都有注释说明 

注意 TM_MAX_CSIZE,TM_MAX_KSIZE,TM_MAX_KCSIZE 会占用静态缓存。

最后你只需要把他们放进你的工程里编译~

## 怎样训练/转换模型
在examples/mnist下有训练脚本可以学习如何训练基础的mnist模型
> 注意：你需要先安装TensorFlow (>=2.7) 环境.

完成训练并保存h5模型后，你可以使用以下脚本转换原始模型到tmdl或者c头文件。 

1. h5_to_tflite.py   
  转换h5模型到浮点或者int8量化的tflite模型
  python3 h5_to_tflite.py h5/mnist.h5 tflite/mnist_f.tflite 0   
  python3 h5_to_tflite.py h5/mnist.h5 tflite/mnist_q.tflite 1 quant_img_mnist/ 0to1   
2. tflite2tmdl.py  
  转换tflite文件到tmdl或者c头文件   
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

现在你有了tmdl或者C头文件，把它放到你的工程里编译吧~

## 使用 Maixhub 在线训练模型

使用 [MaixHub](https://maixhub.com), 你可以下载或者上传已有模型， 同时可以在线训练你的 AI 模型， 不需要编写训练代码，也不需要机器学习基础，点点鼠标模型就训练好啦！

* 注册 [MaixHub](https://maixhub.com) 并登录。
* 你可以从[模型库](https://maixhub.com/model/zoo)下载已有模型，或者上传分享你的模型。
* 创建一个在线模型训练项目， 采集数据集并在线训练，最后会得到:
  * `.tmdl`文件和`.h`文件，用其中一个即可。
  * `report.json`, 训练报告信息, json 格式, 我们可以在此文件中获取到 标签（labels）或者 anchors, 在代码中可能会用到的参数，注意每次训练参数都不一样，注意更改，否则无法正确识别
> * 有两种项目类别，分类 和 检测， **第一次使用建议使用 分类**。
> * 训练是有多个骨干网络(backbone)可选择，根据你的单片机的内存大小选择合适的骨干网络，内存越小则需要选择更小的骨干网络。
> * 为了更好地理解 MaixHub 的使用方法， 开始可以不选择 tinymaix 平台， 而是选择 tfjs 平台训练，训练出可以在手机上运行的模型进行体验。
* 在 [examples](./examples) 目录下找到对应的例程，比如 `maixhub_image_classification` 或者 `maixhub_image_detection` 来运行你训练的模型

## 怎样添加新平台的加速代码

对于新增平台，你只需要在src里添加arch_xxx.h文件并实现其中的函数即可，主要为以下几个函数（重要性降序排列，不重要的函数可以直接拷贝纯CPU运算的函数）：

```
a. TM_INLINE void tm_dot_prod(mtype_t* sptr, mtype_t* kptr,uint32_t size, sumtype_t* result)
	实现平台相关的点积函数，可以使用MAC相关的加速指令加速。

b. TM_INLINE void tm_dot_prod_pack2(mtype_t* sptr, mtype_t* kptr, uint32_t size, sumtype_t* result)
	实现平台相关的双通道点积函数。（仅提供到双通道是因为有些芯片平台的寄存器不足以支持更多通道的点积加速）

c. TM_INLINE void tm_postprocess_sum(int n, sumtype_t* sums, btype_t* bs, int act, mtype_t* outp, sctype_t* scales, sctype_t out_s, zptype_t out_zp)
	实现平台相关的批量后处理函数，注意n为2的次幂。

d. TM_INLINE void tm_dot_prod_3x3x1(mtype_t* sptr, mtype_t* kptr, sumtype_t* result)
	实现平台相关的3x3点积加速
e. TM_INLINE void tm_dot_prod_gap_3x3x1(mtype_t* sptr, mtype_t* kptr, uint32_t* k_oft, sumtype_t* result)
	实现平台相关的3x3 gap的点积加速
```


## 贡献/联系
如果你需要向TinyMaix贡献代码，请先阅读“TinyMaix设计思路”一节，我们只需要“设计内的特性”和“可考虑添加的特性”。

如果你想要提交你的移植测试结果，请提交到 benchmark.md.
我们非常欢迎你移植TinyMaix到自己的芯片/板子上，这会证明使用TinyMaix运行深度学习模型是非常容易的事情~

如果你对TinyMaix的使用和移植有问题，可以在此仓库提交Issues。

如果你有商业或私有项目咨询，你可以发邮件到support@sipeed.com 或 zepan@sipeed.com (泽畔).  


