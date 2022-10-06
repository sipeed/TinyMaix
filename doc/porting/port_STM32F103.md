<!--
 * @Author: HanShuo
 * @Date: 2022-09-27 16:56:46
 * @LastEditors: HanShuo
 * @LastEditTime: 2022-09-27 17:27:28
 * @FilePath: \undefinedc:\Users\Anuo\Desktop\F103\README.md
 * @Description: 
 * Copyright (c) 2022 by HanShuo, All Rights Reserved.
-->
# Port TinyMaix to STM32F103

## Chip

| Item         | Parameter     |
| ------------ | ------------- |
| Chip         | STM32F103RC   |
| Arch         | ARM Cortex M3 |
| Freq         | 72M           |
| Flash        | 256KB         |
| RAM          | 48KB          |
| Acceleration | None      |

## Board

STM32_MINI

<a href="assets/F103_MINI.jpg"><img width=300 src="assets/F103_MINI.jpg"/></a>

## Development Environment

MDK 5.35

## Step/Project

Just simple edit `tm_port.h`

## Result

| config  | mnist | cifar | vww96  | mbnet128 | Note |
| ------- | ----- | ----- | ------ | -------- | ---- |
| O0 CPU  | 8.00  | 554 | XXX | XXX      |      |
| O1 CPU  | 7.00  | 445 | XXX | XXX      |      |
| O0 SIMD | XXX | XXX | XXX | XXX      |      |
| O1 SIMD | XXX  | XXX | XXX | XXX      |      |


## Author

[Anuo](https://github.com/Anuo-shuo/TinyMaix_STM32F103RC) 



