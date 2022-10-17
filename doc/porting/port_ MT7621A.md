# Port TinyMaix to MT7621A

## Chip
|Item |Parameter|
|--   |--|
|Chip |MediaTek MT7621A|
|Arch |MIPS1004Kc|
|Freq |880MHz|
|Flash|128MB|
|RAM  |128MB|
|Acceleration|CPU|

## Board
[Xiaomi Mi Router AC2100](https://openwrt.org/toh/xiaomi/mi_router_ac2100)

<img src="https://openwrt.org/_media/media/xiaomi/mi-router-ac2100_front.jpg" style="zoom:50%;" />

## Development Environment
[openwrt-sdk-21.02.1-ramips-mt7621_gcc-8.4.0_musl.Linux-x86_64](https://downloads.openwrt.org/releases/21.02.1/targets/ramips/mt7621/)

## Step/Project

1. follow this [tutorial](https://www.jianshu.com/p/4444de10e1d7) to setup the environment.
2. git clone TinyMaix repo.
3. edit `tm_port.h` to select OPT0 or OPT1.

## Result

**TM_MDL_INT8 (ms)**

|config  |mnist|cifar|vww96|mbnet128|Note|
|---     |---  |---  |---    |---     |---|
|O0 CPU  |1.049    |41.634  |244.274    |441.256     ||
|O1 CPU  |1.163    |42.533  |267.578    |413.801     ||

**TM_MDL_FP32 (ms)**

| config | mnist | cifar   | vww96    | mbnet128 | Note |
| ------ | ----- | ------- | -------- | -------- | ---- |
| O0 CPU | 4.546 | 549.556 | 1503.767 | 2716.518 |      |
| O1 CPU | 4.556 | 548.757 | 1576.197 | 2679.357 |      |

## Author
[windfallw](https://github.com/windfallw)
