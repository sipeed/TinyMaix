# Port TinyMaix to BK7256

## Chip

| Item         | Parameter      |
| ------------ | -------------- |
| Chip         | BK7256         |
| Arch         | RISC-V         |
| Freq         | 320M           |
| Flash        | 4MB            |
| RAM          | 512KB          |

## Development Environment

[armino]([armino](https://github.com/bekencorp/armino))

## Step/Project

Edit  `tm_port.h`
```
...
#include <os/os.h>
#define TM_PRINTF(...) os_printf(__VA_ARGS__)
...
#include <driver/aon_rtc.h>
#include "aon_rtc_driver.h"
#define  TM_GET_US()       ((uint32_t)((uint64_t)bk_aon_rtc_get_current_tick(AON_RTC_ID_1)*1000LL/32))
...
```

https://github.com/HoGC/TinyMaix_BK7256

# Result

| config | mnist | cifar | vww96  | mbnet128 |
| ------ | ----- | ----- | ------ | -------- | 
| O0 CPU | 3.125  | 196.250 | 681.438 | 1232.938   |
| O1 CPU | 2.813  | 186.281 | 678.125 | 1036.906 |

## Author

[HoGC](https://github.com/HoGC)  
