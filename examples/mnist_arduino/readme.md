# TinyMaix port for Arduino ATmega328
It is dirty port for Arduino ATmega328 with 32KB Flash and 2KB Ram  
Due to AVR is Harvard structure, it can't read struct var in flash directly,  
we have to modify tm_model file to adapt it.  
and limited of its memory, we reduce the mnist model size:  
```
Model: "sequential"
_________________________________________________________________
 Layer (type)                Output Shape              Param #   
=================================================================
 ftr0 (Conv2D)               (None, 13, 13, 1)         10        
                                                                 
 bn0 (BatchNormalization)    (None, 13, 13, 1)         4         
                                                                 
 relu0 (Activation)          (None, 13, 13, 1)         0         
                                                                 
 ftr1 (Conv2D)               (None, 6, 6, 3)           30        
                                                                 
 bn1 (BatchNormalization)    (None, 6, 6, 3)           12        
                                                                 
 relu1 (Activation)          (None, 6, 6, 3)           0         
                                                                 
 ftr2 (Conv2D)               (None, 2, 2, 6)           168       
                                                                 
 batch_normalization (BatchN  (None, 2, 2, 6)          24        
 ormalization)                                                   
                                                                 
 activation (Activation)     (None, 2, 2, 6)           0         
                                                                 
 GAP (GlobalAveragePooling2D  (None, 6)                0         
 )                                                               
                                                                 
 fc1 (Dense)                 (None, 10)                70        
                                                                 
 sm (Activation)             (None, 10)                0         
                                                                 
=================================================================
Total params: 318
Trainable params: 298
Non-trainable params: 20
_________________________________________________________________
```

the tmdl param size is:
```
================    pack done!   ================
    model  size 0.9KB (920 B) FLASH
    buffer size 0.9KB (960 B) RAM
    single layer mode subbuff size 0.4KB (64+296=360 B) RAM
Saved to tmdl/mnist_arduino_q.tmdl, tmdl/mnist_arduino_q.h
```

the arduino compile result is:
```
项目使用了 12174 字节，占用了 (42%) 程序存储空间。最大为 28672 字节。
全局变量使用了1682字节，(82%)的动态内存，余留366字节局部变量。最大为2048字节。

(Flash use 12174 Bytes, Global var use 1682 Bytes)
```

Arduino run result is:
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




