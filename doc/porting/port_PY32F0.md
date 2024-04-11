
# Port TinyMaix to Puya PY32F0

The tested device was a PY32F003x6.
This is practically identical to PY32F002A, PY32F030.

Note that some devices in this family has only 2kB RAM / 16 kB FLASH, which might not be sufficient.

## Chip

| Item  | Parameter              |
| ----- | ---------------------- |
| Chip  | PY32F003x6             |
| Arch  | ARM Cortex M0          |
| Freq  | 24M                    |
| Flash | 32KB                   |
| RAM   | 4KB                    |

## Board

"PY32F003F18P EVB" development board,

Using a no-name "DAPLink" programmer.

<a href="assets/PY32F003F18P-EVB.png"><img width=300 src="assets/PY32F003F18P-EVB.png"/></a>

## Development Environment

[IOsetting/py32f0-template](https://github.com/IOsetting/py32f0-template).
Used `arm-none-eabi-gcc` version `13.2.0`.
This SDK can also be used for Puya PY32F07x devices.

## Step/Project

Edited `tm_port.h` to have the following configuration.

```
#define TM_ARCH         TM_ARCH_CPU
#define TM_OPT_LEVEL    TM_OPT0
#define TM_MDL_TYPE     TM_MDL_INT8
#define TM_FASTSCALE    (1)        
#define TM_LOCAL_MATH   (0)         
```

Project can be found in https://github.com/jonnor/py32f0-template/tree/tinymaix-mnist

`TM_OPT1` failed with undefined references to `tml_conv2d_dwconv2d` etc.

## Result

Used the "Arduino" MNIST model.
Times are in milliseconds.

| config | mnist | Note |
| ------ | ----- | ---- |
| O0 CPU | 44    |      |

Memory usage:

```
Memory region         Used Size  Region Size  %age Used
             RAM:        2104 B         4 KB     51.37%
           FLASH:       25032 B        32 KB     76.39%
```

### **Author**

Jon Nordby ([jonnor](https://github.com/jonnor)).
