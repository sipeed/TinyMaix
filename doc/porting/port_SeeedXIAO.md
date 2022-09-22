# Port TinyMaix to XIAO

## Chip

| Item  | Parameter              |
| ----- | ---------------------- |
| Chip  | Seeed XIAO (SAMD21G18) |
| Arch  | ARM Cortex M0+         |
| Freq  | 48M                    |
| Flash | 256KB                  |
| RAM   | 32KB                   |

## Board 

<a href="assets/Seeeduino-XIAO.jpg"><img width=300 src="assets/Seeeduino-XIAO.jpg"/></a>

## Development Environment

Arduino

## Step/Project

Just simple edit `tm_port.h`

https://github.com/dreamcmi/Seeed-XIAO-Tinymaix

## Result

| config | mnist | cifar   | vww96 | mbnet128 | Note |
| ------ | ----- | ------- | ----- | -------- | ---- |
| O0 CPU | 16.64 | 1018.86 | XXX   | XXX      |      |
| O1 CPU | 14.18 | 700.21  | XXX   | XXX      |      |


## Author

[Darren Cheng](https://github.com/dreamcmi) 



