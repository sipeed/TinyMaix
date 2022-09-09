# Port TinyMaix to CH32V307

## Chip

| Item  | Parameter    |
| ----- | ------------ |
| Chip  | CH32V307     |
| Arch  | RISCV32 IMAC |
| Freq  | 144M         |
| Flash | 480KB        |
| RAM   | 128KB        |

## Board

CH32V307V-R0-1v0

<a href="assets/CH32V307VCT6.png"><img width=300 src="assets/CH32V307VCT6.JPG"/></a>

## Development Environment

riscv-none-embed-gcc

## Step/Project

Just simple edit `tm_port.h`

Porting Project in:   
https://github.com/dreamcmi/CH32V307-TinyMaix

## Result

| config | mnist | cifar | mbnet96 | mbnet128 | Note |
| ------ | ----- | ----- | ------- | -------- | ---- |
| O0 CPU | 1.23  | 75.77 | *       | *        |      |
| O1 CPU | 1.14  | 63.78 | *       | *        |      |

