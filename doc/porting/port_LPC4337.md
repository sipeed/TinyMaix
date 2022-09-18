# Port TinyMaix to LPC4337

## Chip

| Item         | Parameter      |
| ------------ | -------------  | 
| Chip         | LPC4337        |
| Arch         | ARM Cortex M4F |
| Freq         | 204M           |
| Flash        | 1 MB	          |
| RAM          | 136 kB         |
| Acceleration | ARM SIMD       |

## Board

EDU-CIAA  
<a href="assets/LPC4337.jpg"><img width=300 src="assets/LPC4337.jpg"/></a>

## Development Environment

MCUXpresso IDE v11.2.0

## Step/Project

-Import to MXUXpresso as New-> Other -> Makefile Project with existing code
-Modify project.mk to select project to build
Porting Project in:   
https://github.com/juanic/TinyMaix_EDUCIAA

## Result

| config  | mnist | cifar | vww96  | mbnet96 | Note |
| ------- | ----- | ----- | ------ | ------- | ---- |
| O0 CPU  | 7.51  | 817.14| 1701.5 | 1762.5  |   *  |
| O1 CPU  | 7.51  | 817.14| 1701.5 | XXX     |   *  |
| O0 SIMD | 2.95  | 91.17 | 626.67 | 653.85  |   *  |
| O1 SIMD | 2.95  | 91.17 | 626.67 | XXX     |   *  |

*Poor performance compared to similar devices, I checked the time measuremnt 
toggling a pin and the measurements with the oscilloscope agrees with the terminal 
output, I migth be missing something in the porting.

## Author

[Juan Ignacio Cerrudo](https://github.com/juanic) 
