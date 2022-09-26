# Port TinyMaix to silan audio soc SC5864B

## Chip

| Item  | Parameter    |
| ----- | ------------ |
| Chip  | SC5864B      |
| Arch  | M0+HiFi3+RISC   |
| Freq  | 160M         |
| Flash | 32MB         |
| SRAM  | 192KB        |
| DRAM  | 8MB          |

## Board

<a href="assets/SC5864B.jpg"><img width=300 src="assets/SC5864B.jpg"/></a>

## Development Environment

candence Xplorer 7.0.9

## Step/Project

Just simple edit `tm_port.h`
change struct with union error(old compiler donot support new c99, we dont know why).
change some const data to sdram.


## Result
mnist demo(int8)
```
[M0]BASE = 00003700
[M0]LEGTH = 00000032
[M0]



[M0]SYS_CLK : 160Mhz
[M0]BUS_CLK : 160Mhz
[M0]MCU_CLK : 160Mhz
[M0]DSP_CLK : 160Mhz
[M0]FLASH_CLK : 80Mhz
[M0]SDRAM Init Ok(4*16Mbit 
[13:51:03.540]收←◆Cap)
[M0][PIC]Normal   id = 15
[M0][PIC]hdl 		= 0x0034a3
[M0]start memcpy

[M0]boot via flash:0

[M0]src_addr:0x11000 dst_addr:0x2400000


[M0]end memcpy :0x0

[pic]init pic.
mnist demo
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,116,125,171,255,255,150, 93,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,169,253,253,253,253,253,253,218, 30,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,169,253,253,253,213,142,176,253,253,122,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0, 52,250,253,210, 32, 12,  0,  6,206,253,140,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0, 77,251,210, 25,  0,  0,  0,122,248,253, 65,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0, 31, 18,  0,  0,  0,  0,209,253,253, 65,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,117,247,253,198, 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 76,247,253,231, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,253,253,144,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,176,246,253,159, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25,234,253,233, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,198,253,253,141,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0, 78,248,253,189, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0, 19,200,253,253,141,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,134,253,253,173, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,248,253,253, 25,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,248,253,253, 43, 20, 20, 20, 20,  5,  0,  5, 20, 20, 37,150,150,150,147, 10,  0,
  0,  0,  0,  0,  0,  0,  0,  0,248,253,253,253,253,253,253,253,168,143,166,253,253,253,253,253,253,253,123,  0,
  0,  0,  0,  0,  0,  0,  0,  0,174,253,253,253,253,253,253,253,253,253,253,253,249,247,247,169,117,117, 57,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,118,123,123,123,166,253,253,253,155,123,123, 41,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
================================ model stat ================================
mdl_type=0 (int8))
out_deq=1 
input_cnt=1, output_cnt=1, layer_cnt=6
input 3dims: (28, 28, 1)
output 1
[13:51:04.128]收←◆dims: (1, 1, 10)
main buf size 1464; sub buf size 0
//Note: PARAM is layer param size, include align padding

Idx	Layer	         outshape	inoft	outoft	PARAM	MEMOUT OPS
---	Input    	 28, 28,  1	-   	0    	0 	784 	0
###L69: body oft = 64
###L74: type=0, is_out=0, size=152, in_oft=0, out_oft=784, in_dims=[3,28,28,1], out_dims=[3,13,13,4], in_s=0.004, in_zp=-128, out_s=0.016, out_zp=-128
###L86: Conv2d: kw=3, kh=3, sw=2, sh=2, dw=1, dh=1, act=1, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=96, b_oft=136
000	Conv2D      	 13, 13,  4	0	784	72	676	6084
###L69: body oft = 216
###L74: type=0, is_out=0, size=432, in_oft=784, out_oft=0, in_dims=[3,13,13,4], out_dims=[3,6,6,8], in_s=0.016, in_zp=-128, out_s=0.016, out_zp=-128
###L86: Conv2d: kw=3, kh=3, sw=2, sh=2, dw=1, dh=1, act=1, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=112, b_oft=400
001	Conv2D      	  6,  6,  8	784	0	352	288	10368
###L69: body oft = 648
###L74: type=0, is_out=0, size=1360, in_oft=0, out_oft=1400, in_dims=[3,6,6,8], out_dims=[3,2,2,16], in_s=0.016, in_zp=-128, out_s=0.057, out_zp=-128
###L86: Conv2d: kw=3, kh=3, sw=2, sh=2, dw=1, dh=1, act=1, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=144, b_oft=1296
002	Conv2D      	  2,  2, 16	0	1400	1280	64	4608
###L69: body oft = 2008
###L74: type=1, is_out=0, size=48, in_oft=1400, out_oft=0, in_dims=[3,2,2,16], out_dims=[1,1,1,16], in_s=0.057, in_zp=-128, out_s=0.022, out_zp=-128
003	GAP      	  1,  1, 16	1400	0	0	16	64
###L69: body oft = 2056
###L74: type=2, is_out=0, size=304, in_oft=0, out_oft=1448, in_dims=[1,1,1,16], out_dims=[1,1,1,10], in_s=0.022, in_zp=-128, out_s=0.151, out_zp=42
###L95: FC: ws_oft=64, w_oft=104, b_oft=264
004	FC      	  1,  1, 10	0	1448	240	10	160
###L69: body oft = 2360
###L74: type=3, is_out=1, size=48, in_oft=1448, out_oft=0, in_dims=[1,1,1,10], out_dims=[1,1,1,10], in_s=0.151, in_zp=42, out_s=0.004, out_zp=-128
005	Softmax      	  1,  1, 10	1448	0	0	10	60

Total param ~1.9 KB, OPS ~0.02 MOPS, buffer 1.4 KB


[13:55:43.270]收←◆start

[13:55:43.295]收←◆===tm_run use -1610612736 ticks
0: 0.004
1: 0.004
2: 0.996
3: 0.004
4: 0.000
5: 0.000
6: 0.004
7: 0.004
8: 0.004
9: 0.004
### Predict output is: Number 2, prob 0.996
```
demo(FP32)
```
[M0]BASE = 00003700
[M0]LEGTH = 00000032
[M0]



[M0]SYS_CLK : 160Mhz
[M0]BUS_CLK : 160Mhz
[M0]MCU_CLK : 160Mhz
[M0]DSP_CLK : 160Mhz
[M0]FLASH_CLK : 80Mhz
[M0]SDRAM Init Ok(4*16Mbit Cap)
[M0][PIC]Normal   id = 15
[M0][PIC]hdl 		= 0x0034a3
[M0]start memcpy

[M0]boot via flash:0

[M0]src_addr:0x11000 dst_addr:0x2400000


[M0]end memcpy :0x0

[pic]init pic.
mnist demo
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,116,125,171,255,255,150, 93,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,169,253,253,253,253,253,253,218, 30,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,169,253,253,253,213,142,176,253,253,122,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0, 52,250,253,210, 32, 12,  0,  6,206,253,140,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0, 77,251,210, 25,  0,  0,  0,122,248,253, 65,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0, 31, 18,  0,  0,  0,  0,209,253,253, 65,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,117,247,253,198, 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 76,247,253,231, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,253,253,144,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,176,246,253,159, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25,234,253,233, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,198,253,253,141,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0, 78,248,253,189, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0, 19,200,253,253,141,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,134,253,253,173, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,248,253,253, 25,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,248,253,253, 43, 20, 20, 20, 20,  5,  0,  5, 20, 20, 37,150,150,150,147, 10,  0,
  0,  0,  0,  0,  0,  0,  0,  0,248,253,253,253,253,253,253,253,168,143,166,253,253,253,253,253,253,253,123,  0,
  0,  0,  0,  0,  0,  0,  0,  0,174,253,253,253,253,253,253,253,253,253,253,253,249,247,247,169,117,117, 57,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,118,123,123,123,166,253,253,253,155,123,123, 41,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
================================ model stat ================================
mdl_type=2 (fp32))
out_deq=1 
input_cn
t=1, output_cnt=1, layer_cnt=6
input 3dims: (28, 28, 1)
output 1dims: (1, 1, 10)
main buf size 5840; sub buf size 0
//Note: PARAM is layer param size, include align padding

Idx	Layer	         outshape	inoft	outoft	PARAM	MEMOUT OPS
---	Input    	 28, 28,  1	-   	0    	0 	784 	0
###L69: body oft = 64
###L74: type=0, is_out=0, size=240, in_oft=0, out_oft=3136, in_dims=[3,28,28,1], out_dims=[3,13,13,4], in_s=1.000, in_zp=0, out_s=1.000, out_zp=0
###L86: Conv2d: kw=3, kh=3, sw=2, sh=2, dw=1, dh=1, act=1, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=80, b_oft=224
000	Conv2D      	 13, 13,  4	0	3136	160	676	6084
###L69: body oft = 304
###L74: type=0, is_out=0, size=1264, in_oft=3136, out_oft=0, in_dims=[3,13,13,4], out_dims=[3,6,6,8], in_s=1.000, in_zp=0, out_s=1.000, out_zp=0
###L86: Conv2d: kw=3, kh=3, sw=2, sh=2, dw=1, dh=1, act=1, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=80, b_oft=1232
001	Conv2D      	  6,  6,  8	3136	0	1184	288	10368
###L69: body oft = 1568
###L74: type=0, is_out=0, size=4752, in_oft=0, out_oft=5584, in_dims=[3,6,6,8], out_dims=[3,2,2,16], in_s=1.000, in_zp=0, out_s=1.000, out_zp=0
###L86: Conv2d: kw=3, kh=3, sw=2, sh=2, dw=1, dh=1, act=1, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=80, b_oft=4688
002	Conv2D      	  2,  2, 16	0	5584	4672	64	4608
###L69: body oft = 6320
###L74: type=1, is_out=0, size=48, in_oft=5584, out_oft=0, in_dims=[3,2,2,16], out_dims=[1,1,1,16], in_s=1.000, in_zp=0, out_s=1.000, out_zp=0
003	GAP      	  1,  1, 16	5584	0	0	16	64
###L69: body oft = 6368
###L74: type=2, is_out=0, size=744, in_oft=0, out_oft=5800, in_dims=[1,1,1,16], out_dims=[1,1,1,10], in_s=1.000, in_zp=0, out_s=1.000, out_zp=0
###L95: FC: ws_oft=64, w_oft=64, b_oft=704
004	FC      	  1,  1, 10	0	5800	680	10	160
###L69: body oft = 7112
###L74: type=3, is_out=1, size=48, in_oft=5800, out_oft=0, in_dims=[1,1,1,10], out_dims=[1,1,1,10], in_s=1.000, in_zp=0, out_s=1.000, out_zp=0
005	Softmax      	  1,  1, 10	5800	0	0	10	60

Total param ~6.5 KB, OPS ~0.02 MOPS, buffer 5.7 KB


[14:54:00.333]收←◆start

[14:54:00.348]收←◆===tm_run use 1073741824 ticks
0: 0.000
1: 0.000
2: 1.000
3: 0.000
4: -0.000
5: -0.000
6: 0.000
7: 0.000
8: 0.000
9: 0.000
### Predict output is: Number 2, prob 1.000
```

mbnet demo
```
[M0]BASE = 00003700
[M0]LEGTH = 00000032
[M0]



[M0]SYS_CLK : 160Mhz
[M0]BUS_CLK : 160Mhz
[M0]MCU_CLK : 160Mhz
[M0]DSP_CLK : 160Mhz
[M0]FLASH_CLK : 80Mhz
[M0]SDRAM Init Ok(4*16Mbit Cap)
[M0][PIC]Normal   id = 15
[M0][PIC]hdl 		= 0x0034a3
[M0]start memcpy

[M0]boot via flash:0

[M0]src_addr:0x11000 dst_addr:0x2400000


[13:29:19.603]收←◆[M0]end memcpy :0x0

[pic]init pic.
mbnet demo
================================ model stat ================================
mdl_type=0 (int8))
out_deq=1 
input_cnt=1, output_cnt=1, layer_cnt=31
input 3dims: (128, 128, 3)
output 1dims: (1, 1, 1000)
main buf size 98304; sub buf size 0
//Note: PARAM is layer param size, include align padding

Idx	Layer	         outshape	inoft	outoft	PARAM	MEMOUT OPS
---	Input    	128,128,  3	-   	0    	0 	49152 	0
###L69: body oft = 64
###L74: type=0, is_out=0, size=360, in_oft=0, out_oft=65536, in_dims=[3,128,128,3], out_dims=[3,64,64,8], in_s=0.004, in_zp=-128, out_s=0.024, out_zp=-128
###L86: Conv2d: kw=3, kh=3, sw=2, sh=2, dw=1, dh=1, act=3, pad=[0,1,0,1], dmul=0, ws_oft=80, w_oft=112, b_oft=328
000	Conv2D      	 64, 64,  8	0	65536	280	32768	884736
###L69: body oft = 424
###L74: type=5, is_out=0, size=216, in_oft=65536, out_oft=0, in_dims=[3,64,64,8], out_dims=[3,64,64,8], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L106: DWConv2d: kw=3, kh=3, sw=1, sh=1, dw=1, dh=1, act=3, pad=[1,1,1,1], dmul=1, ws_oft=80, w_oft=112, b_oft=184
001	DWConv2D      	 64, 64,  8	65536	0	136	32768	294912
###L69: body oft = 640
###L74: type=0, is_out=0, size=336, in_oft=0, out_oft=32768, in_dims=[3,64,64,8], out_dims=[3,64,64,16], in_s=0.024, in_zp=-128, out_s=0.023, out_zp=-128
###L86: Conv2d: kw=1, kh=1, sw=1, sh=1, dw=1, dh=1, act=3, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=144, b_oft=272
002	Conv2D      	 64, 64, 16	0	32768	256	65536	524288
###L69: body oft = 976
###L74: type=5, is_out=0, size=352, in_oft=32768, out_oft=0, in_dims=[3,64,64,16], out_dims=[3,32,32,16], in_s=0.023, in_zp=-128, out_s=0.024, out_zp=-128
###L106: DWConv2d: kw=3, kh=3, sw=2, sh=2, dw=1, dh=1, act=3, pad=[0,1,0,1], dmul=1, ws_oft=80, w_oft=144, b_oft=288
003	DWConv2D      	 32, 32, 16	32768	0	272	16384	147456
###L69: body oft = 1328
###L74: type=0, is_out=0, size=848, in_oft=0, out_oft=65536, in_dims=[3,32,32,16], out_dims=[3,32,32,32], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L86: Conv2d: kw=1, kh=1, sw=1, sh=1, dw=1, dh=1, act=3, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=208, b_oft=720
004	Conv2D      	 32, 32, 32	0	65536	768	32768	524288
###L69: body oft = 2176
###L74: type=5, is_out=0, size=624, in_oft=65536, out_oft=0, in_dims=[3,32,32,32], out_dims=[3,32,32,32], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L106: DWConv2d: kw=3, kh=3, sw=1, sh=1, dw=1, dh=1, act=3, pad=[1,1,1,1], dmul=1, ws_oft=80, w_oft=208, b_oft=496
005	DWConv2D      	 32, 32, 32	65536	0	544	32768	294912
###L69: body oft = 2800
###L74: type=0, is_out=0, size=1360, in_oft=0, out_oft=65536, in_dims=[3,32,32,32], out_dims=[3,32,32,32], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L86: Conv2d: kw=1, kh=1, sw=1, sh=1, dw=1, dh=1, act=3, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=208, b_oft=1232
006	Conv2D      	 32, 32, 32	0	65536	1280	32768	1048576
###L69: body oft = 4160
###L74: type=5, is_out=0, size=624, in_oft=65536, out_oft=0, in_dims=[3,32,32,32], out_dims=[3,16,16,32], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L106: DWConv2d: kw=3, kh=3, sw=2, sh=2, dw=1, dh=1, act=3, pad=[0,1,0,1], dmul=1, ws_oft=80, w_oft=208, b_oft=496
007	DWConv2D      	 16, 16, 32	65536	0	544	8192	73728
###L69: body oft = 4784
###L74: type=0, is_out=0, size=2640, in_oft=0, out_oft=81920, in_dims=[3,16,16,32], out_dims=[3,16,16,64], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L86: Conv2d: kw=1, kh=1, sw=1, sh=1, dw=1, dh=1, act=3, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=336, b_oft=2384
008	Conv2D      	 16, 16, 64	0	81920	2560	16384	524288
###L69: body oft = 7424
###L74: type=5, is_out=0, size=1168, in_oft=81920, out_oft=0, in_dims=[3,16,16,64], out_dims=[3,16,16,64], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L106: DWConv2d: kw=3, kh=3, sw=1, sh=1, dw=1, dh=1, act=3, pad=[1,1,1,1], dmul=1, ws_oft=80, w_oft=336, b_oft=912
009	DWConv2D      	 16, 16, 64	81920	0	1088	16384	147456
###L69: body oft = 8592
###L74: type=0, is_out=0, size=4688, in_oft=0, out_oft=81920, in_dims=[3,16,16,64], out_dims=[3,16,16,64], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L86: Conv2d: kw=1, kh=1, sw=1, sh=1, dw=1, dh=1, act=3, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=336, b_oft=4432
010	Conv2D      	 16, 16, 64	0	81920	4608	16384	1048576
###L69: body oft = 13280
###L74: type=5, is_out=0, size=1168, in_oft=81920, out_oft=0, in_dims=[3,16,16,64], out_dims=[3,8,8,64], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L106: DWConv2d: kw=3, kh=3, sw=2, sh=2, dw=1, dh=1, act=3, pad=[0,1,0,1], dmul=1, ws_oft=80, w_oft=336, b_oft=912
011	DWConv2D      	  8,  8, 64	81920	0	1088	4096	36864
###L69: body oft = 14448
###L74: type=0, is_out=0, size=9296, in_oft=0, out_oft=90112, in_dims=[3,8,8,64], out_dims=[3,8,8,128], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L86: Conv2d: kw=1, kh=1, sw=1, sh=1, dw=1, dh=1, act=3, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=592, b_oft=8784
012	Conv2D      	  8,  8,128	0	90112	9216	8192	524288
###L69: body oft = 23744
###L74: type=5, is_out=0, size=2256, in_oft=90112, out_oft=0, in_dims=[3,8,8,128], out_dims=[3,8,8,128], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L106: DWConv2d: kw=3, kh=3, sw=1, sh=1, dw=1, dh=1, act=3, pad=[1,1,1,1], dmul=1, ws_oft=80, w_oft=592, b_oft=1744
013	DWConv2D      	  8,  8,128	90112	0	2176	8192	73728
###L69: body oft = 26000
###L74: type=0, is_out=0, size=17488, in_oft=0, out_oft=90112, in_dims=[3,8,8,128], out_dims=[3,8,8,128], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L86: Conv2d: kw=1, kh=1, sw=1, sh=1, dw=1, dh=1, act=3, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=592, b_oft=16976
014	Conv2D      	  8,  8,128	0	90112	17408	8192	1048576
###L69: body oft = 43488
###L74: type=5, is_out=0, size=2256, in_oft=90112, out_oft=0, in_dims=[3,8,8,128], out_dims=[3,8,8,128], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L106: DWConv2d: kw=3, kh=3, sw=1, sh=1, dw=1, dh=1, act=3, pad=[1,1,1,1], dmul=1, ws_oft=80, w_oft=592, b_oft=1744
015	DWConv2D      	  8,  8,128	90112	0	2176	8192	73728
###L69: body oft = 45744
###L74: type=0, is_out=0, size=17488, in_oft=0, out_oft=90112, in_dims=[3,8,8,128], out_dims=[3,8,8,128], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L86: Conv2d: kw=1, kh=1, sw=1, sh=1, dw=1, dh=1, act=3, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=592, b_oft=16976
016	Conv2D      	  8,  8,128	0	90112	17408	8192	1048576
###L69: body oft = 63232
###L74: type=5, is_out=0, size=2256, in_oft=90112, out_oft=0, in_dims=[3,8,8,128], out_dims=[3,8,8,128], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L106: DWConv2d: kw=3, kh=3, sw=1, sh=1, dw=1, dh=1, act=3, pad=[1,1,1,1], dmul=1, ws_oft=80, w_oft=592, b_oft=1744
017	DWConv2D      	  8,  8,128	90112	0	2176	8192	73728
###L69: body oft = 65488
###L74: type=0, is_out=0, size=17488, in_oft=0, out_oft=90112, in_dims=[3,8,8,128], out_dims=[3,8,8,128], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L86: Conv2d: kw=1, kh=1, sw=1, sh=1, dw=1, dh=1, act=3, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=592, b_oft=16976
018	Conv2D      	  8,  8,128	0	90112	17408	8192	1048576
###L69: body oft = 82976
###L74: type=5, is_out=0, size=2256, in_oft=90112, out_oft=0, in_dims=[3,8,8,128], out_dims=[3,8,8,128], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L106: DWConv2d: kw=3, kh=3, sw=1, sh=1, dw=1, dh=1, act=3, pad=[1,1,1,1], dmul=1, ws_oft=80, w_oft=592, b_oft=1744
019	DWConv2D      	  8,  8,128	90112	0	2176	8192	73728
###L69: body oft = 85232
###L74: type=0, is_out=0, size=17488, in_oft=0, out_oft=90112, in_dims=[3,8,8,128], out_dims=[3,8,8,128], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L86: Conv2d: kw=1, kh=1, sw=1, sh=1, dw=1, dh=1, act=3, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=592, b_oft=16976
020	Conv2D      	  8,  8,128	0	90112	17408	8192	1048576
###L69: body oft = 102720
###L74: type=5, is_out=0, size=2256, in_oft=90112, out_oft=0, in_dims=[3,8,8,128], out_dims=[3,8,8,128], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L106: DWConv2d: kw=3, kh=3, sw=1, sh=1, dw=1, dh=1, act=3, pad=[1,1,1,1], dmul=1, ws_oft=80, w_oft=592, b_oft=1744
021	DWConv2D      	  8,  8,128	90112	0	2176	8192	73728
###L69: body oft = 104976
###L74: type=0, is_out=0, size=17488, in_oft=0, out_oft=90112, in_dims=[3,8,8,128], out_dims=[3,8,8,128], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L86: Conv2d: kw=1, kh=1, sw=1, sh=1, dw=1, dh=1, act=3, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=592, b_oft=16976
022	Conv2D      	  8,  8,128	0	90112	17408	8192	1048576
###L69: body oft = 122464
###L74: type=5, is_out=0, size=2256, in_oft=90112, out_oft=0, in_dims=[3,8,8,128], out_dims=[3,4,4,128], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L106: DWConv2d: kw=3, kh=3, sw=2, sh=2, dw=1, dh=1, act=3, pad=[0,1,0,1], dmul=1, ws_oft=80, w_oft=592, b_oft=1744
023	DWConv2D      	  4,  4,128	90112	0	2176	2048	18432
###L69: body oft = 124720
###L74: type=0, is_out=0, size=34896, in_oft=0, out_oft=94208, in_dims=[3,4,4,128], out_dims=[3,4,4,256], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L86: Conv2d: kw=1, kh=1, sw=1, sh=1, dw=1, dh=1, act=3, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=1104, b_oft=33872
024	Conv2D      	  4,  4,256	0	94208	34816	4096	524288
###L69: body oft = 159616
###L74: type=5, is_out=0, size=4432, in_oft=94208, out_oft=0, in_dims=[3,4,4,256], out_dims=[3,4,4,256], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L106: DWConv2d: kw=3, kh=3, sw=1, sh=1, dw=1, dh=1, act=3, pad=[1,1,1,1], dmul=1, ws_oft=80, w_oft=1104, b_oft=3408
025	DWConv2D      	  4,  4,256	94208	0	4352	4096	36864
###L69: body oft = 164048
###L74: type=0, is_out=0, size=67664, in_oft=0, out_oft=94208, in_dims=[3,4,4,256], out_dims=[3,4,4,256], in_s=0.024, in_zp=-128, out_s=0.024, out_zp=-128
###L86: Conv2d: kw=1, kh=1, sw=1, sh=1, dw=1, dh=1, act=3, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=1104, b_oft=66640
026	Conv2D      	  4,  4,256	0	94208	67584	4096	1048576
###L69: body oft = 231712
###L74: type=1, is_out=0, size=48, in_oft=94208, out_oft=0, in_dims=[3,4,4,256], out_dims=[3,1,1,256], in_s=0.024, in_zp=-128, out_s=0.023, out_zp=-128
027	GAP      	  1,  1,256	94208	0	0	256	4096
###L69: body oft = 231760
###L74: type=0, is_out=0, size=264080, in_oft=0, out_oft=97304, in_dims=[3,1,1,256], out_dims=[3,1,1,1000], in_s=0.023, in_zp=-128, out_s=0.124, out_zp=-27
###L86: Conv2d: kw=1, kh=1, sw=1, sh=1, dw=1, dh=1, act=0, pad=[0,0,0,0], dmul=0, ws_oft=80, w_oft=4080, b_oft=260080
028	Conv2D      	  1,  1,1000	0	97304	264000	1000	256000
###L69: body oft = 495840
###L74: type=4, is_out=0, size=48, in_oft=97304, out_oft=97304, in_dims=[3,1,1,1000], out_dims=[1,1,1,1000], in_s=0.124, in_zp=-27, out_s=0.124, out_zp=-27
029	Reshape      	  1,  1,1000	97304	97304	0	1000	0
###L69: body oft = 495888
###L74: type=3, is_out=1, size=48, in_oft=97304, out_oft=0, in_dims=[1,1,1,1000], out_dims=[1,1,1,1000], in_s=0.124, in_zp=-27, out_s=0.004, out_zp=-128
030	Softmax      	  1,  1,1000	97304	0	0	1000	6000

Total param ~481.9 KB, OPS ~13.58 MOPS, buffer 96.0 KB


[13:29:20.912]收←◆start

[13:29:21.717]收←◆===tm_run use -1073741824 ticks
### Predict output is: Class 292 (tiger, Panthera tigris), Prob 0.809
```

because of there is some error in timer or systick, so using time is calulate by serial timestamp, is may be a litte slow than real one.


| config | mnist | cifar | vww96  | mbnet128 | Note |
| ------ | ----- | ----- | ------ | -------- | ---- |
| M0 | -  | -| - | -   |  -   |
| DSP(hifi3) | 25ms(int8)/15ms(FP32)  | - | - | ~805ms   | -    |
| RISC(csky) | -  | -| - | -   |  -   |

### **Author**

[1847123212](https://github.com/1847123212/TinyMaix)  
